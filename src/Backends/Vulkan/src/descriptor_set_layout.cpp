#include <litefx/backends/vulkan.hpp>
#include <litefx/backends/vulkan_builders.hpp>
#include "image.h"

using namespace LiteFX::Rendering::Backends;

// ------------------------------------------------------------------------------------------------
// Implementation.
// ------------------------------------------------------------------------------------------------

class VulkanDescriptorSetLayout::VulkanDescriptorSetLayoutImpl : public Implement<VulkanDescriptorSetLayout> {
public:
    friend class VulkanDescriptorSetLayoutBuilder;
    friend class VulkanDescriptorSetLayout;

private:
    Array<UniquePtr<VulkanDescriptorLayout>> m_descriptorLayouts;
    Array<VkDescriptorPool> m_descriptorPools;
    Queue<VkDescriptorSet> m_freeDescriptorSets;
    Array<VkDescriptorPoolSize> m_poolSizes {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 0 },
        { VK_DESCRIPTOR_TYPE_SAMPLER, 0 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 0 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 0 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 0 }
    };
    Dictionary<VkDescriptorType, UInt32> m_poolSizeMapping {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2 },
        { VK_DESCRIPTOR_TYPE_SAMPLER, 3 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 4 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 5 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 6 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 7 }
    };
    ShaderStage m_stages;
    UInt32 m_space, m_poolSize;
    mutable std::mutex m_mutex;
    const VulkanDevice& m_device;
    bool m_usesDescriptorIndexing = false;
    Dictionary<const VkDescriptorSet*, const VkDescriptorPool*> m_descriptorSetSources;

public:
    VulkanDescriptorSetLayoutImpl(VulkanDescriptorSetLayout* parent, const VulkanDevice& device, Enumerable<UniquePtr<VulkanDescriptorLayout>>&& descriptorLayouts, UInt32 space, ShaderStage stages) :
        base(parent), m_device(device), m_space(space), m_stages(stages), m_poolSize(0)
    {
        m_descriptorLayouts = descriptorLayouts | std::views::as_rvalue | std::ranges::to<std::vector>();
    }

    VulkanDescriptorSetLayoutImpl(VulkanDescriptorSetLayout* parent, const VulkanDevice& device) :
        base(parent), m_device(device), m_poolSize(0)
    {
    }

public:
    VkDescriptorSetLayout initialize(UInt32 poolSize, UInt32 maxUnboundedArraySize)
    {
        LITEFX_TRACE(VULKAN_LOG, "Defining layout for descriptor set {0} {{ Stages: {1}, Pool Size: {2} }}...", m_space, m_stages, poolSize);
        m_poolSize = poolSize;

        // Parse the shader stage descriptor.
        VkShaderStageFlags shaderStages = {};

        if ((m_stages & ShaderStage::Vertex) == ShaderStage::Vertex)
            shaderStages |= VK_SHADER_STAGE_VERTEX_BIT;
        if ((m_stages & ShaderStage::Geometry) == ShaderStage::Geometry)
            shaderStages |= VK_SHADER_STAGE_GEOMETRY_BIT;
        if ((m_stages & ShaderStage::Fragment) == ShaderStage::Fragment)
            shaderStages |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if ((m_stages & ShaderStage::TessellationEvaluation) == ShaderStage::TessellationEvaluation)
            shaderStages |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if ((m_stages & ShaderStage::TessellationControl) == ShaderStage::TessellationControl)
            shaderStages |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if ((m_stages & ShaderStage::Compute) == ShaderStage::Compute)
            shaderStages |= VK_SHADER_STAGE_COMPUTE_BIT;

        // Parse descriptor set layouts.
        Array<VkDescriptorSetLayoutBinding> bindings;
        Array<VkDescriptorBindingFlags> bindingFlags;
        Array<VkDescriptorSetLayoutBindingFlagsCreateInfo> bindingFlagCreateInfo;

        std::ranges::for_each(m_descriptorLayouts, [&, i = 0](const UniquePtr<VulkanDescriptorLayout>& layout) mutable {
            auto bindingPoint = layout->binding();
            auto type = layout->descriptorType();

            LITEFX_TRACE(VULKAN_LOG, "\tWith descriptor {0}/{1} {{ Type: {2}, Element size: {3} bytes, Array size: {6}, Offset: {4}, Binding point: {5} }}...", ++i, m_descriptorLayouts.size(), type, layout->elementSize(), 0, bindingPoint, layout->descriptors());

            VkDescriptorSetLayoutBinding binding = {};
            binding.binding = bindingPoint;
            binding.descriptorCount = layout->descriptors();
            binding.pImmutableSamplers = nullptr;
            binding.stageFlags = shaderStages;

            if (type == DescriptorType::InputAttachment && m_stages != ShaderStage::Fragment)
                throw RuntimeException("Unable to bind input attachment at {0} to a descriptor set that is accessible from other stages, than the fragment shader.", bindingPoint);

            switch (type)
            {
            case DescriptorType::ConstantBuffer:     binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;       break;
            case DescriptorType::ByteAddressBuffer:
            case DescriptorType::RWByteAddressBuffer:
            case DescriptorType::StructuredBuffer:
            case DescriptorType::RWStructuredBuffer: binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;       break;
            case DescriptorType::RWTexture:          binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;        break;
            case DescriptorType::Texture:            binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;        break;
            case DescriptorType::RWBuffer:           binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER; break;
            case DescriptorType::Buffer:             binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER; break;
            case DescriptorType::InputAttachment:    binding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;     break;
            case DescriptorType::Sampler:            binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;              break;
            default: LITEFX_WARNING(VULKAN_LOG, "The descriptor type is unsupported. Binding will be skipped.");       return;
            }

            if (type != DescriptorType::Sampler || (type == DescriptorType::Sampler && layout->staticSampler() == nullptr))
                m_poolSizes[m_poolSizeMapping[binding.descriptorType]].descriptorCount++;
            else
                binding.pImmutableSamplers = &layout->staticSampler()->handle();
            
            // If the descriptor is an unbounded runtime array, disable validation warnings about partially bound elements.
            if (binding.descriptorCount != std::numeric_limits<UInt32>::max())
                bindingFlags.push_back({ });
            else
            {
                // Unbounded arrays must be the only descriptor within a descriptor set.
                if (m_descriptorLayouts.size() != 1) [[unlikely]]
                    throw InvalidArgumentException("If an unbounded runtime array descriptor is used, it must be the only descriptor in the descriptor set, however the current descriptor set specifies {0} descriptors", m_descriptorLayouts.size());

                bindingFlags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT | VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT_EXT);
                m_usesDescriptorIndexing = true;
                binding.descriptorCount = maxUnboundedArraySize;
            }

            bindings.push_back(binding);
        });

        LITEFX_TRACE(VULKAN_LOG, "Creating descriptor set {0} layout with {1} bindings {{ Uniform: {2}, Storage: {3}, Images: {4}, Sampler: {5}, Input Attachments: {6}, Writable Images: {7}, Texel Buffers: {8} }}...", m_space, m_descriptorLayouts.size(), m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_SAMPLER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER]].descriptorCount);

        // Create the descriptor set layout.
        VkDescriptorSetLayoutBindingFlagsCreateInfo extendedInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
            .bindingCount = static_cast<UInt32>(bindingFlags.size()),
            .pBindingFlags = bindingFlags.data()
        };

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = &extendedInfo,
            .bindingCount = static_cast<UInt32>(bindings.size()),
            .pBindings = bindings.data()
        };

        // Allow for descriptors to update after they have been bound. This also means, we have to manually take care of not to update a descriptor before it got used.
        if (m_usesDescriptorIndexing)
            descriptorSetLayoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

        VkDescriptorSetLayout layout;
        raiseIfFailed<RuntimeException>(::vkCreateDescriptorSetLayout(m_device.handle(), &descriptorSetLayoutInfo, nullptr, &layout), "Unable to create descriptor set layout.");

        // Create the initial descriptor pool.
        this->addDescriptorPool();

        return layout;
    }

    void addDescriptorPool()
    {
        LITEFX_TRACE(VULKAN_LOG, "Allocating descriptor pool with {5} sets {{ Uniforms: {0}, Storages: {1}, Images: {2}, Samplers: {3}, Input attachments: {4} }}...", m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_SAMPLER]].descriptorCount, m_poolSizes[m_poolSizeMapping[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT]].descriptorCount, m_poolSize);

        // Filter pool sizes, since descriptorCount must be greater than 0, according to the specs.
        auto poolSizes = m_poolSizes |
            std::views::filter([](const VkDescriptorPoolSize& poolSize) { return poolSize.descriptorCount > 0; }) |
            std::ranges::to<std::vector>();

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = m_poolSize;

        if (m_usesDescriptorIndexing)
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

        VkDescriptorPool descriptorPool;
        raiseIfFailed<RuntimeException>(::vkCreateDescriptorPool(m_device.handle(), &poolInfo, nullptr, &descriptorPool), "Unable to create buffer pool.");
        m_descriptorPools.push_back(descriptorPool);
    }

    VkDescriptorSet tryAllocate(UInt32 descriptors)
    {
        VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo;
        VkDescriptorSetAllocateInfo descriptorSetInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = m_descriptorPools.back(),
            .descriptorSetCount = 1,
            .pSetLayouts = &m_parent->handle()
        };

        if (m_usesDescriptorIndexing)
        {
            variableCountInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
                .descriptorSetCount = 1,
                .pDescriptorCounts = &descriptors
            };

            descriptorSetInfo.pNext = &variableCountInfo;
        }

        // Try to allocate a new descriptor set.
        VkDescriptorSet descriptorSet;
        auto result = ::vkAllocateDescriptorSets(m_device.handle(), &descriptorSetInfo, &descriptorSet);

        if (result == VK_SUCCESS) [[likely]]
        {
            m_descriptorSetSources.insert(std::make_pair(&descriptorSet, &m_descriptorPools.back()));
            return descriptorSet;
        }
        else if (result != VK_ERROR_OUT_OF_POOL_MEMORY)
        {
            raiseIfFailed<RuntimeException>(result, "Unable to allocate descriptor set.");
        }

        // The pool is full, so we have to create a new one and retry.
        this->addDescriptorPool();
        return this->tryAllocate(descriptors);
    }
};

// ------------------------------------------------------------------------------------------------
// Shared interface.
// ------------------------------------------------------------------------------------------------

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDevice& device, Enumerable<UniquePtr<VulkanDescriptorLayout>>&& descriptorLayouts, UInt32 space, ShaderStage stages, UInt32 poolSize, UInt32 maxUnboundedArraySize) :
    m_impl(makePimpl<VulkanDescriptorSetLayoutImpl>(this, device, std::move(descriptorLayouts), space, stages)), Resource<VkDescriptorSetLayout>(VK_NULL_HANDLE)
{
    this->handle() = m_impl->initialize(poolSize, maxUnboundedArraySize);
}

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDevice& device) noexcept :
    m_impl(makePimpl<VulkanDescriptorSetLayoutImpl>(this, device)), Resource<VkDescriptorSetLayout>(VK_NULL_HANDLE)
{
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() noexcept
{
    // Release descriptor pools and destroy the descriptor set layouts. Releasing the pools also frees the descriptor sets allocated from it.
    std::ranges::for_each(m_impl->m_descriptorPools, [this](const VkDescriptorPool& pool) { ::vkDestroyDescriptorPool(m_impl->m_device.handle(), pool, nullptr); });
    ::vkDestroyDescriptorSetLayout(m_impl->m_device.handle(), this->handle(), nullptr);
}

const VulkanDevice& VulkanDescriptorSetLayout::device() const noexcept
{
    return m_impl->m_device;
}

Enumerable<const VulkanDescriptorLayout*> VulkanDescriptorSetLayout::descriptors() const noexcept
{
    return m_impl->m_descriptorLayouts | std::views::transform([](const UniquePtr<VulkanDescriptorLayout>& layout) { return layout.get(); });
}

const VulkanDescriptorLayout& VulkanDescriptorSetLayout::descriptor(UInt32 binding) const
{
    if (auto match = std::ranges::find_if(m_impl->m_descriptorLayouts, [&binding](const UniquePtr<VulkanDescriptorLayout>& layout) { return layout->binding() == binding; }); match != m_impl->m_descriptorLayouts.end())
        return *match->get();

    throw ArgumentOutOfRangeException("No layout has been provided for the binding {0}.", binding);
}

UInt32 VulkanDescriptorSetLayout::space() const noexcept
{
    return m_impl->m_space;
}

ShaderStage VulkanDescriptorSetLayout::shaderStages() const noexcept
{
    return m_impl->m_stages;
}

UInt32 VulkanDescriptorSetLayout::uniforms() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER].descriptorCount;
}

UInt32 VulkanDescriptorSetLayout::storages() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].descriptorCount;
}

UInt32 VulkanDescriptorSetLayout::buffers() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER].descriptorCount + m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER].descriptorCount;
}

UInt32 VulkanDescriptorSetLayout::images() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE].descriptorCount + m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE].descriptorCount;
}

UInt32 VulkanDescriptorSetLayout::samplers() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_SAMPLER].descriptorCount;
}

UInt32 VulkanDescriptorSetLayout::staticSamplers() const noexcept
{
    return std::ranges::count_if(m_impl->m_descriptorLayouts, [](const UniquePtr<VulkanDescriptorLayout>& layout) { return layout->descriptorType() == DescriptorType::Sampler && layout->staticSampler() != nullptr; });
}

UInt32 VulkanDescriptorSetLayout::inputAttachments() const noexcept
{
    return m_impl->m_poolSizes[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT].descriptorCount;
}

UniquePtr<VulkanDescriptorSet> VulkanDescriptorSetLayout::allocate(const Enumerable<DescriptorBinding>& bindings) const
{
    return this->allocate(0, bindings);
}

UniquePtr<VulkanDescriptorSet> VulkanDescriptorSetLayout::allocate(UInt32 descriptors, const Enumerable<DescriptorBinding>& bindings) const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);

    // If no descriptor sets are free, or the descriptor set contains an unbounded descriptor array, allocate a new descriptor set.
    UniquePtr<VulkanDescriptorSet> descriptorSet;

    if (m_impl->m_usesDescriptorIndexing || m_impl->m_freeDescriptorSets.empty())
        descriptorSet = makeUnique<VulkanDescriptorSet>(*this, m_impl->tryAllocate(descriptors));
    else
    {
        // Otherwise, pick and remove one from the list.
        descriptorSet = makeUnique<VulkanDescriptorSet>(*this, m_impl->m_freeDescriptorSets.front());
        m_impl->m_freeDescriptorSets.pop();
    }

    // Apply the default bindings.
    for (auto& binding : bindings)
        std::visit(type_switch{
            [&descriptorSet, &binding](const ISampler& sampler) { descriptorSet->update(binding.binding, sampler, binding.firstDescriptor); },
            [&descriptorSet, &binding](const IBuffer& buffer) { descriptorSet->update(binding.binding, buffer, binding.firstElement, binding.elements, binding.firstDescriptor); },
            [&descriptorSet, &binding](const IImage& image) { descriptorSet->update(binding.binding, image, binding.firstDescriptor, binding.firstLevel, binding.levels, binding.firstElement, binding.elements); }
        }, binding.resource);

    // Return the descriptor set.
    return descriptorSet;
}

Enumerable<UniquePtr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(UInt32 descriptorSets, const Enumerable<Enumerable<DescriptorBinding>>& bindings) const
{
    return this->allocateMultiple(descriptorSets, 0, bindings);
}

Enumerable<UniquePtr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(UInt32 descriptorSets, std::function<Enumerable<DescriptorBinding>(UInt32)> bindingFactory) const
{
    return this->allocateMultiple(descriptorSets, 0, bindingFactory);
}

Enumerable<UniquePtr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(UInt32 count, UInt32 descriptors, const Enumerable<Enumerable<DescriptorBinding>>& bindings) const
{
    return [this, descriptors, &bindings, &count]() -> std::generator<UniquePtr<VulkanDescriptorSet>> {
        for (auto& binding : bindings)
            co_yield this->allocate(descriptors, binding);
        
        for (int i = static_cast<int>(bindings.size()); i < count; ++i)
            co_yield this->allocate(descriptors);
    }() | std::views::as_rvalue;
}

Enumerable<UniquePtr<VulkanDescriptorSet>> VulkanDescriptorSetLayout::allocateMultiple(UInt32 count, UInt32 descriptors, std::function<Enumerable<DescriptorBinding>(UInt32)> bindingFactory) const
{
    return [this, descriptors, &bindingFactory, &count]() -> std::generator<UniquePtr<VulkanDescriptorSet>> {
        for (int i = 0; i < count; ++i)
            co_yield this->allocate(descriptors, bindingFactory(i));
    }() | std::views::as_rvalue;
}

void VulkanDescriptorSetLayout::free(const VulkanDescriptorSet& descriptorSet) const noexcept
{
    if (!m_impl->m_usesDescriptorIndexing)
    {
        // Keep the descriptor set around (get's automatically released when the pool gets destroyed).
        std::lock_guard<std::mutex> lock(m_impl->m_mutex);
        m_impl->m_freeDescriptorSets.push(descriptorSet.handle());
    }
    else
    {
        // Unbounded descriptor sets must be destroyed, because every set may have different descriptor counts.
        auto handle = &descriptorSet.handle();

        if (m_impl->m_descriptorSetSources.contains(handle))
        {
            auto pool = m_impl->m_descriptorSetSources[handle];
            auto result = ::vkFreeDescriptorSets(m_impl->m_device.handle(), *pool, 1, handle);
            
            if (result != VK_SUCCESS) [[unlikely]]
                LITEFX_WARNING(VULKAN_LOG, "Unable to properly release descriptor set: {0}.", result);

            m_impl->m_descriptorSetSources.erase(handle);

            // We can even release the pool, if it isn't the current active one and no descriptor sets are in use anymore.
            if (pool != &m_impl->m_descriptorPools.back() && std::ranges::count_if(m_impl->m_descriptorSetSources, [&](const auto& sourceMapping) { return sourceMapping.second == pool; }) == 0)
            {
                ::vkDestroyDescriptorPool(m_impl->m_device.handle(), *pool, nullptr);

                if (auto match = std::ranges::find(m_impl->m_descriptorPools, *pool); match != m_impl->m_descriptorPools.end()) [[likely]]
                    m_impl->m_descriptorPools.erase(match);
            }
        }
    }
}

UInt32 VulkanDescriptorSetLayout::poolSize() const noexcept
{
    return m_impl->m_poolSize;
}

size_t VulkanDescriptorSetLayout::pools() const noexcept
{
    return m_impl->m_descriptorPools.size();
}

#if defined(BUILD_DEFINE_BUILDERS)
// ------------------------------------------------------------------------------------------------
// Descriptor set layout builder shared interface.
// ------------------------------------------------------------------------------------------------

constexpr VulkanDescriptorSetLayoutBuilder::VulkanDescriptorSetLayoutBuilder(VulkanPipelineLayoutBuilder& parent, UInt32 space, ShaderStage stages, UInt32 poolSize, UInt32 maxUnboundedArraySize) :
    DescriptorSetLayoutBuilder(parent, UniquePtr<VulkanDescriptorSetLayout>(new VulkanDescriptorSetLayout(parent.device())))
{
    m_state.poolSize = poolSize;
    m_state.maxUnboundedArraySize = maxUnboundedArraySize;
}

constexpr VulkanDescriptorSetLayoutBuilder::~VulkanDescriptorSetLayoutBuilder() noexcept = default;

void VulkanDescriptorSetLayoutBuilder::build()
{
    auto instance = this->instance();
    instance->m_impl->m_descriptorLayouts = std::move(m_state.descriptorLayouts);
    instance->m_impl->m_space = std::move(m_state.space);
    instance->m_impl->m_stages = std::move(m_state.stages);
    instance->m_impl->initialize(m_state.poolSize, m_state.maxUnboundedArraySize);
}

constexpr UniquePtr<VulkanDescriptorLayout> VulkanDescriptorSetLayoutBuilder::makeDescriptor(DescriptorType type, UInt32 binding, UInt32 descriptorSize, UInt32 descriptors)
{
    return makeUnique<VulkanDescriptorLayout>(type, binding, descriptorSize, descriptors);
}

constexpr UniquePtr<VulkanDescriptorLayout> VulkanDescriptorSetLayoutBuilder::makeDescriptor(UInt32 binding, FilterMode magFilter, FilterMode minFilter, BorderMode borderU, BorderMode borderV, BorderMode borderW, MipMapMode mipMapMode, Float mipMapBias, Float minLod, Float maxLod, Float anisotropy)
{
    return makeUnique<VulkanDescriptorLayout>(makeUnique<VulkanSampler>(this->parent().device(), magFilter, minFilter, borderU, borderV, borderW, mipMapMode, mipMapBias, minLod, maxLod, anisotropy), binding);
}
#endif // defined(BUILD_DEFINE_BUILDERS)