#include "buffer.h"

using namespace LiteFX::Rendering::Backends;

// ------------------------------------------------------------------------------------------------
// Buffer Base.
// ------------------------------------------------------------------------------------------------
_VMABufferBase::_VMABufferBase(VkBuffer buffer, VmaAllocator& allocator, VmaAllocation allocation) :
	IResource(buffer), m_allocator(allocator), m_allocationInfo(allocation) 
{
}

_VMABufferBase::~_VMABufferBase() noexcept
{
	::vmaDestroyBuffer(m_allocator, this->handle(), m_allocationInfo);
	LITEFX_TRACE(VULKAN_LOG, "Destroyed buffer {0}", fmt::ptr(this->handle()));
}

void _VMABufferBase::map(const void* const data, const size_t& size)
{
	void* buffer;

	raiseIfFailed<RuntimeException>(::vmaMapMemory(m_allocator, m_allocationInfo, &buffer), "Unable to map buffer memory.");

	auto result = ::memcpy_s(buffer, this->size(), data, size);

	if (result != 0) 
		throw RuntimeException("Error mapping buffer to device memory: {#X}.", result);

	::vmaUnmapMemory(m_allocator, m_allocationInfo);
}

void _VMABufferBase::transferFrom(const ICommandBuffer* commandBuffer, IBuffer* source, const size_t& size, const size_t& sourceOffset, const size_t& targetOffset)
{
	auto transferBuffer = dynamic_cast<const VulkanCommandBuffer*>(commandBuffer);
	auto sourceBuffer = dynamic_cast<const IResource<VkBuffer>*>(source);

	if (sourceBuffer == nullptr)
		throw std::invalid_argument("The transfer source buffer must be initialized and a valid Vulkan buffer.");

	if (transferBuffer == nullptr)
		throw std::invalid_argument("The command buffer must be initialized and a valid Vulkan command buffer.");
	
	// Create a copy command and add it to the command buffer.
	VkBufferCopy copyInfo{};
	copyInfo.size = size;
	copyInfo.srcOffset = sourceOffset;
	copyInfo.dstOffset = targetOffset;
	::vkCmdCopyBuffer(transferBuffer->handle(), sourceBuffer->handle(), this->handle(), 1, &copyInfo);
}

void _VMABufferBase::transferTo(const ICommandBuffer* commandBuffer, IBuffer* target, const size_t& size, const size_t& sourceOffset, const size_t& targetOffset) const
{
	auto transferBuffer = dynamic_cast<const VulkanCommandBuffer*>(commandBuffer);
	auto targetBuffer = dynamic_cast<const IResource<VkBuffer>*>(target);

	if (targetBuffer == nullptr)
		throw std::invalid_argument("The transfer target buffer must be initialized and a valid Vulkan buffer.");

	if (transferBuffer == nullptr)
		throw std::invalid_argument("The command buffer must be initialized and a valid Vulkan command buffer.");

	// Create a copy command and add it to the command buffer.
	VkBufferCopy copyInfo{};
	copyInfo.size = size;
	copyInfo.srcOffset = sourceOffset;
	copyInfo.dstOffset = targetOffset;
	::vkCmdCopyBuffer(transferBuffer->handle(), this->handle(), targetBuffer->handle(), 1, &copyInfo);
}

// ------------------------------------------------------------------------------------------------
// Generic Buffer.
// ------------------------------------------------------------------------------------------------

_VMABuffer::_VMABuffer(VkBuffer buffer, const BufferType& type, const UInt32& elements, const size_t& size, VmaAllocator& allocator, VmaAllocation allocation) :
	_VMABufferBase(buffer, allocator, allocation), Buffer(type, elements, size)
{
}

_VMABuffer::~_VMABuffer() noexcept = default;

UniquePtr<IBuffer> _VMABuffer::allocate(const BufferType& type, const UInt32& elements, const size_t& size, VmaAllocator& allocator, const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo, VmaAllocationInfo* allocationResult)
{
	VkBuffer buffer;
	VmaAllocation allocation;

	raiseIfFailed<RuntimeException>(::vmaCreateBuffer(allocator, &createInfo, &allocationInfo, &buffer, &allocation, allocationResult), "Unable to allocate buffer.");
	LITEFX_DEBUG(VULKAN_LOG, "Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}", fmt::ptr(buffer), type, elements, size / elements, size);

	return makeUnique<_VMABuffer>(buffer, type, elements, size, allocator, allocation);
}

// ------------------------------------------------------------------------------------------------
// Vertex Buffer.
// ------------------------------------------------------------------------------------------------

_VMAVertexBuffer::_VMAVertexBuffer(VkBuffer buffer, const VulkanVertexBufferLayout& layout, const UInt32& elements, VmaAllocator& allocator, VmaAllocation allocation) :
	_VMABufferBase(buffer, allocator, allocation), VertexBuffer(layout, elements)
{
}

_VMAVertexBuffer::~_VMAVertexBuffer() noexcept = default;

UniquePtr<VulkanVertexBuffer> _VMAVertexBuffer::allocate(const VulkanVertexBufferLayout& layout, const UInt32& elements, VmaAllocator& allocator, const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo, VmaAllocationInfo* allocationResult)
{
	VkBuffer buffer;
	VmaAllocation allocation;

	raiseIfFailed<RuntimeException>(::vmaCreateBuffer(allocator, &createInfo, &allocationInfo, &buffer, &allocation, allocationResult), "Unable to allocate vertex buffer.");
	LITEFX_DEBUG(VULKAN_LOG, "Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}", fmt::ptr(buffer), BufferType::Vertex, elements, layout.elementSize(), layout.elementSize() * elements);

	return makeUnique<_VMAVertexBuffer>(buffer, layout, elements, allocator, allocation);
}

// ------------------------------------------------------------------------------------------------
// Index Buffer.
// ------------------------------------------------------------------------------------------------

_VMAIndexBuffer::_VMAIndexBuffer(VkBuffer buffer, const VulkanIndexBufferLayout& layout, const UInt32& elements, VmaAllocator& allocator, VmaAllocation allocation) :
	_VMABufferBase(buffer, allocator, allocation), IndexBuffer(layout, elements)
{
}

_VMAIndexBuffer::~_VMAIndexBuffer() noexcept = default;

UniquePtr<VulkanIndexBuffer> _VMAIndexBuffer::allocate(const VulkanIndexBufferLayout& layout, const UInt32& elements, VmaAllocator& allocator, const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo, VmaAllocationInfo* allocationResult)
{
	VkBuffer buffer;
	VmaAllocation allocation;

	raiseIfFailed<RuntimeException>(::vmaCreateBuffer(allocator, &createInfo, &allocationInfo, &buffer, &allocation, allocationResult), "Unable to allocate index buffer.");
	LITEFX_DEBUG(VULKAN_LOG, "Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}", fmt::ptr(buffer), BufferType::Index, elements, layout.elementSize(), layout.elementSize() * elements);

	return makeUnique<_VMAIndexBuffer>(buffer, layout, elements, allocator, allocation);
}

// ------------------------------------------------------------------------------------------------
// Constant Buffer.
// ------------------------------------------------------------------------------------------------

_VMAConstantBuffer::_VMAConstantBuffer(VkBuffer buffer, const IDescriptorLayout* layout, const UInt32& elements, VmaAllocator& allocator, VmaAllocation allocation) :
	_VMABufferBase(buffer, allocator, allocation), ConstantBuffer(layout, elements)
{
}

_VMAConstantBuffer::~_VMAConstantBuffer() noexcept = default;

UniquePtr<IConstantBuffer> _VMAConstantBuffer::allocate(const IDescriptorLayout* layout, const UInt32& elements, VmaAllocator& allocator, const VkBufferCreateInfo& createInfo, const VmaAllocationCreateInfo& allocationInfo, VmaAllocationInfo* allocationResult)
{
	if (layout == nullptr)
		throw std::invalid_argument("The layout must be initialized.");

	// Allocate the buffer.
	VkBuffer buffer;
	VmaAllocation allocation;

	if (::vmaCreateBuffer(allocator, &createInfo, &allocationInfo, &buffer, &allocation, allocationResult) != VK_SUCCESS)
		throw std::runtime_error("Unable to allocate buffer.");

	LITEFX_DEBUG(VULKAN_LOG, "Allocated buffer {0} with {4} bytes {{ Type: {1}, Elements: {2}, Element Size: {3} }}", fmt::ptr(buffer), layout->type(), elements, layout->elementSize(), layout->elementSize() * elements);

	return makeUnique<_VMAConstantBuffer>(buffer, layout, elements, allocator, allocation);
}