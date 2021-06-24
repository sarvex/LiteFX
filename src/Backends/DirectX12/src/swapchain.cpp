#include <litefx/backends/dx12.hpp>
#include "image.h"

using namespace LiteFX::Rendering::Backends;

// ------------------------------------------------------------------------------------------------
// Implementation.
// ------------------------------------------------------------------------------------------------

class DirectX12SwapChain::DirectX12SwapChainImpl : public Implement<DirectX12SwapChain> {
public:
	friend class DirectX12SwapChain;

private:
	Size2d m_renderArea{ };
	Format m_format{ Format::None };
	UInt32 m_buffers{ };
	std::atomic_uint32_t m_currentImage{ };
	Array<UniquePtr<IDirectX12Image>> m_presentImages{ };
	bool m_supportsVariableRefreshRates{ false };
	MultiSamplingLevel m_multiSampleLevel;

public:
	DirectX12SwapChainImpl(DirectX12SwapChain* parent) : 
		base(parent) 
	{
	}

private:
	bool supportsVariableRefreshRates(const DirectX12Backend& backend) const
	{
		BOOL allowTearing = FALSE;
		
		if (FAILED(backend.handle()->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
			return false;
		
		return static_cast<bool>(allowTearing);
	}

	MultiSamplingLevel findSupportedMultiSamplingLevel(const MultiSamplingLevel& inputLevel, const Format& format) const
	{
		auto maxLevel = this->m_parent->getDevice()->maximumMultisamplingLevel(format);
		return std::min(maxLevel, inputLevel);
	}

public:
	[[nodiscard]]
	ComPtr<IDXGISwapChain4> initialize(const Format& format, const Size2d& frameBufferSize, const MultiSamplingLevel& multiSampleLevel, const UInt32& frameBuffers)
	{
		if (!std::ranges::any_of(m_parent->getSurfaceFormats(), [&format](const Format& surfaceFormat) { return surfaceFormat == format; }))
			throw InvalidArgumentException("The provided surface format {0} it not a supported. Must be one of the following: {1}.", format, this->joinSupportedSurfaceFormats());

		auto adapter = m_parent->getDevice()->adapter().handle();
		auto surface = m_parent->getDevice()->surface().handle();
		auto graphicsQueue = m_parent->getDevice()->graphicsQueue().handle();
		const auto& backend = m_parent->getDevice()->backend();
		MultiSamplingLevel samples = this->findSupportedMultiSamplingLevel(format);

		// Create the swap chain.
		LITEFX_TRACE(DIRECTX12_LOG, "Creating swap chain for device {0} {{ Images: {1}, Extent: {2}x{3} Px }}...", fmt::ptr(m_parent->getDevice()), frameBuffers, frameBufferSize.width(), frameBufferSize.height());
		
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = static_cast<UInt32>(frameBufferSize.width());
		swapChainDesc.Height = static_cast<UInt32>(frameBufferSize.height());
		swapChainDesc.Format = ::getFormat(format);
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = std::max<UInt32>(2, frameBuffers);
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = (m_supportsVariableRefreshRates = supportsVariableRefreshRates(backend)) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		swapChainDesc.SampleDesc = samples == MultiSamplingLevel::x1 ? DXGI_SAMPLE_DESC{ 1, 0 } : DXGI_SAMPLE_DESC{ static_cast<UInt32>(samples), DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN };

		ComPtr<IDXGISwapChain1> swapChainBase;
		ComPtr<IDXGISwapChain4> swapChain;
		raiseIfFailed<RuntimeException>(backend.handle()->CreateSwapChainForHwnd(graphicsQueue.Get(), surface, &swapChainDesc, nullptr, nullptr, &swapChainBase), "Unable to create swap chain for device.");
		raiseIfFailed<RuntimeException>(swapChainBase.As(&swapChain), "The swap chain does not implement the IDXGISwapChain4 interface.");

		// Acquire the swap chain images.
		m_presentImages.resize(swapChainDesc.BufferCount);
		std::ranges::generate(m_presentImages, [this, &frameBufferSize, &format, &swapChain, i = 0]() mutable {
			ComPtr<ID3D12Resource> resource;
			raiseIfFailed<RuntimeException>(swapChain->GetBuffer(i++, IID_PPV_ARGS(&resource)), "Unable to acquire image resource from swap chain back buffer {0}.", i);
			return makeUnique<DirectX12Image>(m_parent->parent(), std::move(resource), frameBufferSize, format, D3D12_RESOURCE_STATE_PRESENT);
		});

		// Disable Alt+Enter shortcut for fullscreen-toggle.
		backend.handle()->MakeWindowAssociation(surface, DXGI_MWA_NO_ALT_ENTER);

		m_format = format;
		m_renderArea = frameBufferSize;
		m_buffers = swapChainDesc.BufferCount;
		m_multiSampleLevel = samples;

		return swapChain;
	}

	void reset(const Format& format, const Size2d& frameBufferSize, const MultiSamplingLevel& multiSampleLevel, const UInt32& frameBuffers)
	{
		if (!std::ranges::any_of(m_parent->getSurfaceFormats(), [&format](const Format& surfaceFormat) { return surfaceFormat == format; }))
			throw InvalidArgumentException("The provided surface format {0} it not a supported. Must be one of the following: {1}.", format, this->joinSupportedSurfaceFormats());

		auto samples = this->findSupportedMultiSamplingLevel(multiSampleLevel, format);

		// Release all back buffers.
		m_presentImages.clear();

		if (samples == m_multiSampleLevel)
		{
			// Store a backend reference.
			const auto& backend = m_parent->getDevice()->backend();

			// Resize the buffers.
			UInt32 buffers = std::max<UInt32>(2, frameBuffers);
			raiseIfFailed<RuntimeException>(m_parent->handle()->ResizeBuffers(buffers, static_cast<UInt32>(frameBufferSize.width()), static_cast<UInt32>(frameBufferSize.height()), ::getFormat(format), (m_supportsVariableRefreshRates = supportsVariableRefreshRates(backend)) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0), "Unable to resize swap chain back buffers.");

			// Acquire the swap chain images.
			m_presentImages.resize(buffers);
			std::ranges::generate(m_presentImages, [this, &frameBufferSize, &format, i = 0]() mutable {
				ComPtr<ID3D12Resource> resource;
				raiseIfFailed<RuntimeException>(m_parent->handle()->GetBuffer(i++, IID_PPV_ARGS(&resource)), "Unable to acquire image resource from swap chain back buffer {0}.", i);
				return makeUnique<DirectX12Image>(m_parent->parent(), std::move(resource), frameBufferSize, format, D3D12_RESOURCE_STATE_PRESENT);
			});

			m_format = format;
			m_renderArea = frameBufferSize;
			m_buffers = buffers;
		}
		else
		{
			// We need to re-create the swap chain to support more or less samples.
			m_parent->handle() = this->initialize(format, frameBufferSize, samples, frameBuffers);
		}
	}

	UInt32 swapBackBuffer()
	{
		// NOTE: Swap is happening when presenting in a render pass. When acquiring a new back buffer, we need to wait for the swap to occur first, though.
		// TODO: Check, if we can solve this with another fence, that is queued on present and waited for in here.
		m_parent->getDevice()->wait();
		return m_parent->handle()->GetCurrentBackBufferIndex();
	}

private:
	String joinSupportedSurfaceFormats() const noexcept 
	{
		auto formats = m_parent->getSurfaceFormats();

		return Join(formats |
			std::views::transform([](const Format& format) { return fmt::to_string(format); }) |
			ranges::to<Array<String>>(), ", ");
	}
};

// ------------------------------------------------------------------------------------------------
// Shared interface.
// ------------------------------------------------------------------------------------------------

DirectX12SwapChain::DirectX12SwapChain(const DirectX12Device& device, const Format& format, const Size2d& frameBufferSize, const MultiSamplingLevel& multiSampleLevel, const UInt32& frameBuffers) :
	m_impl(makePimpl<DirectX12SwapChainImpl>(this)), DirectX12RuntimeObject(device, &device), ComResource<IDXGISwapChain4>(nullptr)
{
	this->handle() = m_impl->initialize(format, frameBufferSize, multiSampleLevel, frameBuffers);
}

DirectX12SwapChain::~DirectX12SwapChain() noexcept = default;

const bool& DirectX12SwapChain::supportsVariableRefreshRate() const noexcept
{
	return m_impl->m_supportsVariableRefreshRates;
}

const Format& DirectX12SwapChain::surfaceFormat() const noexcept
{
	return m_impl->m_format;
}

const UInt32& DirectX12SwapChain::buffers() const noexcept
{
	return m_impl->m_buffers;
}

const Size2d& DirectX12SwapChain::renderArea() const noexcept
{
	return m_impl->m_renderArea;
}

Array<const IDirectX12Image*> DirectX12SwapChain::images() const noexcept
{
	return m_impl->m_presentImages | std::views::transform([](const UniquePtr<IDirectX12Image>& image) { return image.get(); }) | ranges::to<Array<const IDirectX12Image*>>();
}

const MultiSamplingLevel& DirectX12SwapChain::multiSamplingLevel() const noexcept
{
	return m_impl->m_multiSampleLevel;
}

Array<Format> DirectX12SwapChain::getSurfaceFormats() const noexcept
{
	// NOTE: Those formats are actually the only ones that are supported for flip-model swap chains, which is currently the only 
	//       supported swap effect. If other swap effects are used, this function may require redesign. For more information see: 
	//       https://docs.microsoft.com/en-us/windows/win32/api/dxgi1_2/ns-dxgi1_2-dxgi_swap_chain_desc1#remarks.
	return Array<Format> {
		::getFormat(DXGI_FORMAT_R16G16B16A16_FLOAT),
		::getFormat(DXGI_FORMAT_R10G10B10A2_UNORM),
		::getFormat(DXGI_FORMAT_B8G8R8A8_UNORM),
		::getFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
	};
}

void DirectX12SwapChain::reset(const Format& surfaceFormat, const Size2d& renderArea, const MultiSamplingLevel& multiSampleLevel, const UInt32& buffers)
{
	m_impl->reset(surfaceFormat, renderArea, multiSampleLevel, buffers);
}

UInt32 DirectX12SwapChain::swapBackBuffer() const
{
	return m_impl->swapBackBuffer();
}