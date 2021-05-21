#include <litefx/backends/vulkan.hpp>

using namespace LiteFX::Rendering::Backends;

// ------------------------------------------------------------------------------------------------
// Implementation.
// ------------------------------------------------------------------------------------------------

class VulkanVertexBufferLayout::VulkanVertexBufferLayoutImpl : public Implement<VulkanVertexBufferLayout> {
public:
    friend class VulkanVertexBufferLayoutBuilder;
    friend class VulkanVertexBufferLayout;

private:
    Array<UniquePtr<BufferAttribute>> m_attributes;
    size_t m_vertexSize;
    UInt32 m_binding;

public:
    VulkanVertexBufferLayoutImpl(VulkanVertexBufferLayout* parent, const size_t& vertexSize, const UInt32& binding) : 
        base(parent), m_vertexSize(vertexSize), m_binding(binding) { }

public:
    Array<const BufferAttribute*> getAttributes() const noexcept
    {
        Array<const BufferAttribute*> attributes(m_attributes.size());
        std::generate(std::begin(attributes), std::end(attributes), [&, i = 0]() mutable { return m_attributes[i++].get(); });
        
        return attributes;
    }
};

// ------------------------------------------------------------------------------------------------
// Shared interface.
// ------------------------------------------------------------------------------------------------

VulkanVertexBufferLayout::VulkanVertexBufferLayout(const VulkanInputAssembler& inputAssembler, const size_t& vertexSize, const UInt32& binding) :
    m_impl(makePimpl<VulkanVertexBufferLayoutImpl>(this, vertexSize, binding)), VulkanRuntimeObject<VulkanInputAssembler>(inputAssembler, inputAssembler.getDevice())
{
}

VulkanVertexBufferLayout::~VulkanVertexBufferLayout() noexcept = default;

size_t VulkanVertexBufferLayout::elementSize() const noexcept
{
    return m_impl->m_vertexSize;
}

const UInt32& VulkanVertexBufferLayout::binding() const noexcept
{
    return m_impl->m_binding;
}

const BufferType& VulkanVertexBufferLayout::type() const noexcept
{
    return BufferType::Vertex;
}

Array<const BufferAttribute*> VulkanVertexBufferLayout::attributes() const noexcept
{
    return m_impl->getAttributes();
}

// ------------------------------------------------------------------------------------------------
// Builder interface.
// ------------------------------------------------------------------------------------------------

VulkanVertexBufferLayoutBuilder& VulkanVertexBufferLayoutBuilder::addAttribute(UniquePtr<BufferAttribute>&& attribute)
{
    this->instance()->m_impl->m_attributes.push_back(std::move(attribute));
    return *this;
}

VulkanVertexBufferLayoutBuilder& VulkanVertexBufferLayoutBuilder::addAttribute(const BufferFormat& format, const UInt32& offset, const AttributeSemantic& semantic, const UInt32& semanticIndex)
{
    return this->addAttribute(std::move(makeUnique<BufferAttribute>(static_cast<UInt32>(this->instance()->attributes().size()), offset, format, semantic, semanticIndex)));
}

VulkanVertexBufferLayoutBuilder& VulkanVertexBufferLayoutBuilder::addAttribute(const UInt32& location, const BufferFormat& format, const UInt32& offset, const AttributeSemantic& semantic, const UInt32& semanticIndex)
{
    return this->addAttribute(std::move(makeUnique<BufferAttribute>(location, offset, format, semantic, semanticIndex)));
}