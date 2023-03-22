#include <litefx/backends/dx12.hpp>

using namespace LiteFX::Rendering::Backends;

// ------------------------------------------------------------------------------------------------
// Implementation.
// ------------------------------------------------------------------------------------------------

class DirectX12CommandBuffer::DirectX12CommandBufferImpl : public Implement<DirectX12CommandBuffer> {
public:
	friend class DirectX12CommandBuffer;

private:
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	bool m_recording{ false };
	const DirectX12Queue& m_queue;

public:
	DirectX12CommandBufferImpl(DirectX12CommandBuffer* parent, const DirectX12Queue& queue) :
		base(parent), m_queue(queue)
	{
	}

public:
	ComPtr<ID3D12GraphicsCommandList4> initialize(const bool& begin)
	{
		// Create a command allocator.
		D3D12_COMMAND_LIST_TYPE type;

		switch (m_queue.type())
		{
		case QueueType::Compute: type = D3D12_COMMAND_LIST_TYPE_COMPUTE; break;
		case QueueType::Transfer: type = D3D12_COMMAND_LIST_TYPE_COPY; break;
		default:
		case QueueType::Graphics: type = D3D12_COMMAND_LIST_TYPE_DIRECT; break;
		}

		raiseIfFailed<RuntimeException>(m_queue.device().handle()->CreateCommandAllocator(type, IID_PPV_ARGS(&m_commandAllocator)), "Unable to create command allocator for command buffer.");

		// Create the actual command list.
		ComPtr<ID3D12GraphicsCommandList4> commandList;

		if (m_recording = begin)
			raiseIfFailed<RuntimeException>(m_queue.device().handle()->CreateCommandList(0, type, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)), "Unable to create command list for command buffer.");
		else
			raiseIfFailed<RuntimeException>(m_queue.device().handle()->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&commandList)), "Unable to create command list for command buffer.");

		return commandList;
	}

	void reset()
	{
		raiseIfFailed<RuntimeException>(m_commandAllocator->Reset(), "Unable to reset command allocator.");
		raiseIfFailed<RuntimeException>(m_parent->handle()->Reset(m_commandAllocator.Get(), nullptr), "Unable to reset command list.");
		m_recording = true;
	}

	void bindDescriptorHeaps()
	{
		if (m_queue.type() == QueueType::Compute || m_queue.type() == QueueType::Graphics)
			m_queue.device().bindGlobalDescriptorHeaps(*m_parent);
	}
};

// ------------------------------------------------------------------------------------------------
// Shared interface.
// ------------------------------------------------------------------------------------------------

DirectX12CommandBuffer::DirectX12CommandBuffer(const DirectX12Queue& queue, const bool& begin) :
	m_impl(makePimpl<DirectX12CommandBufferImpl>(this, queue)), ComResource<ID3D12GraphicsCommandList4>(nullptr)
{
	this->handle() = m_impl->initialize(begin);

	if (begin)
		m_impl->bindDescriptorHeaps();
}

DirectX12CommandBuffer::~DirectX12CommandBuffer() noexcept = default;

void DirectX12CommandBuffer::begin() const
{
	// Reset the command buffer.
	m_impl->reset();

	// Bind the descriptor heaps.
	m_impl->bindDescriptorHeaps();
}

void DirectX12CommandBuffer::end() const
{
	// Close the command list, so that it does not longer record any commands.
	if (m_impl->m_recording)
		raiseIfFailed<RuntimeException>(this->handle()->Close(), "Unable to close command buffer for recording.");

	m_impl->m_recording = false;
}

void DirectX12CommandBuffer::generateMipMaps(IDirectX12Image& image) noexcept
{
	struct Parameters {
		Float sizeX;
		Float sizeY;
		Float sRGB;
		Float padding;
	};

	// Create the array of parameter data.
	Array<Parameters> parametersData(image.levels());

	std::ranges::generate(parametersData, [this, &image, i = 0]() mutable {
		auto level = i++;

		return Parameters {
			.sizeX = 1.f / static_cast<Float>(std::max<size_t>(image.extent(level).width(), 1)),
			.sizeY = 1.f / static_cast<Float>(std::max<size_t>(image.extent(level).height(), 1)),
			.sRGB = DX12::isSRGB(image.format()) ? 1.f : 0.f
		};
	});

	auto parametersBlock = parametersData |
		std::views::transform([](const Parameters& parameters) { return reinterpret_cast<const void*>(&parameters); }) |
		ranges::to<Array<const void*>>();

	// Set the active pipeline state.
	auto& pipeline = m_impl->m_queue.device().blitPipeline();
	this->use(pipeline);

	// Create and bind the parameters.
	const auto& resourceBindingsLayout = pipeline.layout()->descriptorSet(0);
	auto resourceBindings = resourceBindingsLayout.allocateMultiple(image.levels() * image.layers());
	const auto& parametersLayout = resourceBindingsLayout.descriptor(0);
	auto parameters = m_impl->m_queue.device().factory().createBuffer(parametersLayout.type(), BufferUsage::Dynamic, parametersLayout.elementSize(), image.levels());
	parameters->map(parametersBlock, sizeof(Parameters));

	// Create and bind the sampler.
	const auto& samplerBindingsLayout = pipeline.layout()->descriptorSet(1);
	auto samplerBindings = samplerBindingsLayout.allocate();
	auto sampler = m_impl->m_queue.device().factory().createSampler(FilterMode::Linear, FilterMode::Linear, BorderMode::ClampToEdge, BorderMode::ClampToEdge, BorderMode::ClampToEdge);
	samplerBindings->update(0, *sampler);
	this->bind(*samplerBindings, pipeline);

	// Transition the texture into a read/write state.
	DirectX12Barrier barrier, waitBarrier;
	barrier.transition(image, ResourceState::ReadWrite);
	waitBarrier.waitFor(image);
	this->barrier(barrier);
	auto size = image.extent();
	int resource = 0;

	for (int l(0); l < image.layers(); ++l)
	{
		for (UInt32 i(1); i < image.levels(); ++i, size /= 2)
		{
			// Update the invocation parameters.
			resourceBindings[resource]->update(parametersLayout.binding(), *parameters, i, 1);

			// Bind the previous mip map level to the SRV at binding point 1.
			resourceBindings[resource]->update(1, image, 0, i - 1, 1, l, 1);

			// Bind the current level to the UAV at binding point 2.
			resourceBindings[resource]->update(2, image, 0, i, 1, l, 1);

			// Dispatch the pipeline.
			this->bind(*resourceBindings[resource], pipeline);
			this->dispatch({ std::max<UInt32>(size.width() / 8, 1), std::max<UInt32>(size.height() / 8, 1), 1 });

			// Wait for all writes.
			this->barrier(waitBarrier);
			resource++;
		}
	}

	// Transition back into a shader resource.
	this->barrier(barrier, true);
}

void DirectX12CommandBuffer::barrier(const DirectX12Barrier& barrier, const bool& invert) const noexcept
{
	if (invert)
		barrier.executeInverse(*this);
	else
		barrier.execute(*this);
}

void DirectX12CommandBuffer::transfer(const IDirectX12Buffer& source, const IDirectX12Buffer& target, const UInt32& sourceElement, const UInt32& targetElement, const UInt32& elements) const
{
	if (source.elements() < sourceElement + elements) [[unlikely]]
		throw ArgumentOutOfRangeException("The source buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.", source.elements(), elements, sourceElement);

	if (target.elements() < targetElement + elements) [[unlikely]]
		throw ArgumentOutOfRangeException("The target buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.", target.elements(), elements, targetElement);

	this->handle()->CopyBufferRegion(target.handle().Get(), targetElement * target.alignedElementSize(), source.handle().Get(), sourceElement * source.alignedElementSize(), elements * source.alignedElementSize());
}

void DirectX12CommandBuffer::transfer(const IDirectX12Buffer& source, const IDirectX12Image& target, const UInt32& sourceElement, const UInt32& firstSubresource, const UInt32& elements) const
{
	if (source.elements() < sourceElement + elements) [[unlikely]]
		throw ArgumentOutOfRangeException("The source buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.", source.elements(), elements, sourceElement);

	if (target.elements() < firstSubresource + elements) [[unlikely]]
		throw ArgumentOutOfRangeException("The target image has only {0} sub-resources, but a transfer for {1} elements starting from element {2} has been requested.", target.elements(), elements, firstSubresource);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	const auto& targetDesc = target.handle()->GetDesc();

	for (int sr(0); sr < elements; ++sr)
	{
		m_impl->m_queue.device().handle()->GetCopyableFootprints(&targetDesc, sourceElement + sr, 1, 0, &footprint, nullptr, nullptr, nullptr);
		CD3DX12_TEXTURE_COPY_LOCATION sourceLocation(source.handle().Get(), footprint), targetLocation(target.handle().Get(), firstSubresource + sr);
		this->handle()->CopyTextureRegion(&targetLocation, 0, 0, 0, &sourceLocation, nullptr);
	}
}

void DirectX12CommandBuffer::transfer(const IDirectX12Image& source, const IDirectX12Image& target, const UInt32& sourceSubresource, const UInt32& targetSubresource, const UInt32& subresources) const
{
	if (source.elements() < sourceSubresource + subresources) [[unlikely]]
		throw ArgumentOutOfRangeException("The source image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resource {2} has been requested.", source.elements(), subresources, sourceSubresource);

	if (target.elements() < targetSubresource + subresources) [[unlikely]]
		throw ArgumentOutOfRangeException("The target image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resources {2} has been requested.", target.elements(), subresources, targetSubresource);

	// TODO: Check if we can possibly do this more efficiently by copying multiple sub-resources at once.
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	const auto& targetDesc = target.handle()->GetDesc();

	for (int sr(0); sr < subresources; ++sr)
	{
		m_impl->m_queue.device().handle()->GetCopyableFootprints(&targetDesc, sourceSubresource + sr, 1, 0, &footprint, nullptr, nullptr, nullptr);
		CD3DX12_TEXTURE_COPY_LOCATION sourceLocation(source.handle().Get(), footprint), targetLocation(target.handle().Get(), targetSubresource + sr);
		this->handle()->CopyTextureRegion(&targetLocation, 0, 0, 0, &sourceLocation, nullptr);
	}
}

void DirectX12CommandBuffer::transfer(const IDirectX12Image& source, const IDirectX12Buffer& target, const UInt32& firstSubresource, const UInt32& targetElement, const UInt32& subresources) const
{
	if (source.elements() < firstSubresource + subresources) [[unlikely]]
		throw ArgumentOutOfRangeException("The source image has only {0} sub-resources, but a transfer for {1} sub-resources starting from sub-resource {2} has been requested.", source.elements(), subresources, firstSubresource);

	if (target.elements() <= targetElement + subresources) [[unlikely]]
		throw ArgumentOutOfRangeException("The target buffer has only {0} elements, but a transfer for {1} elements starting from element {2} has been requested.", target.elements(), subresources, targetElement);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	const auto& targetDesc = target.handle()->GetDesc();

	for (int sr(0); sr < subresources; ++sr)
	{
		m_impl->m_queue.device().handle()->GetCopyableFootprints(&targetDesc, firstSubresource + sr, 1, 0, &footprint, nullptr, nullptr, nullptr);
		CD3DX12_TEXTURE_COPY_LOCATION sourceLocation(source.handle().Get(), footprint), targetLocation(target.handle().Get(), targetElement + sr);
		this->handle()->CopyTextureRegion(&targetLocation, 0, 0, 0, &sourceLocation, nullptr);
	}
}

void DirectX12CommandBuffer::use(const DirectX12PipelineState& pipeline) const noexcept
{
	pipeline.use(*this);
}

void DirectX12CommandBuffer::bind(const DirectX12DescriptorSet& descriptorSet, const DirectX12PipelineState& pipeline) const noexcept
{
	m_impl->m_queue.device().bindDescriptorSet(*this, descriptorSet, pipeline);
}

void DirectX12CommandBuffer::bind(const IDirectX12VertexBuffer& buffer) const noexcept 
{
	this->handle()->IASetVertexBuffers(buffer.layout().binding(), 1, &buffer.view());
}

void DirectX12CommandBuffer::bind(const IDirectX12IndexBuffer& buffer) const noexcept 
{
	this->handle()->IASetIndexBuffer(&buffer.view());
}

void DirectX12CommandBuffer::dispatch(const Vector3u& threadCount) const noexcept
{
	this->handle()->Dispatch(threadCount.x(), threadCount.y(), threadCount.z());
}

void DirectX12CommandBuffer::draw(const UInt32& vertices, const UInt32& instances, const UInt32& firstVertex, const UInt32& firstInstance) const noexcept
{
	this->handle()->DrawInstanced(vertices, instances, firstVertex, firstInstance);
}

void DirectX12CommandBuffer::drawIndexed(const UInt32& indices, const UInt32& instances, const UInt32& firstIndex, const Int32& vertexOffset, const UInt32& firstInstance) const noexcept
{
	this->handle()->DrawIndexedInstanced(indices, instances, firstIndex, vertexOffset, firstInstance);
}

void DirectX12CommandBuffer::pushConstants(const DirectX12PushConstantsLayout& layout, const void* const memory) const noexcept
{
	std::ranges::for_each(layout.ranges(), [this, &layout, &memory](const DirectX12PushConstantsRange* range) { this->handle()->SetGraphicsRoot32BitConstants(range->rootParameterIndex(), range->size() / 4, reinterpret_cast<const char* const>(memory) + range->offset(), 0); });
}