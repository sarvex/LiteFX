#pragma once

#include <litefx/rendering.hpp>

#include "vulkan_api.hpp"
#include "vulkan_formatters.hpp"

namespace LiteFX::Rendering::Backends {
	using namespace LiteFX::Math;
	using namespace LiteFX::Rendering;

	/// <summary>
	/// Implements a Vulkan vertex buffer layout.
	/// </summary>
	/// <seealso cref="VulkanVertexBuffer" />
	/// <seealso cref="VulkanVertexBufferLayoutBuilder" />
	class LITEFX_VULKAN_API VulkanVertexBufferLayout : public virtual VulkanRuntimeObject<VulkanInputAssembler>, public IVertexBufferLayout {
		LITEFX_IMPLEMENTATION(VulkanVertexBufferLayoutImpl);
		LITEFX_BUILDER(VulkanVertexBufferLayoutBuilder);

	public:
		/// <summary>
		/// Initializes a new vertex buffer layout.
		/// </summary>
		/// <param name="inputAssembler">The parent input assembler state, the vertex buffer layout is initialized for.</param>
		/// <param name="vertexSize">The size of a single vertex.</param>
		/// <param name="binding">The binding point of the vertex buffers using this layout.</param>
		explicit VulkanVertexBufferLayout(const VulkanInputAssembler& inputAssembler, const size_t& vertexSize, const UInt32& binding = 0);
		VulkanVertexBufferLayout(VulkanVertexBufferLayout&&) = delete;
		VulkanVertexBufferLayout(const VulkanVertexBufferLayout&) = delete;
		virtual ~VulkanVertexBufferLayout() noexcept;

		// IVertexBufferLayout interface.
	public:
		/// <inheritdoc />
		virtual Array<const BufferAttribute*> attributes() const noexcept override;

		// IBufferLayout interface.
	public:
		/// <inheritdoc />
		virtual size_t elementSize() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& binding() const noexcept override;

		/// <inheritdoc />
		virtual const BufferType& type() const noexcept override;
	};

	/// <summary>
	/// Builds a see <see cref="VulkanVertexBufferLayout" />.
	/// </summary>
	/// <seealso cref="VulkanVertexBuffer" />
	/// <seealso cref="VulkanVertexBufferLayout" />
	class LITEFX_VULKAN_API VulkanVertexBufferLayoutBuilder : public VertexBufferLayoutBuilder<VulkanVertexBufferLayoutBuilder, VulkanVertexBufferLayout, VulkanInputAssemblerBuilder> {
	public:
		using VertexBufferLayoutBuilder<VulkanVertexBufferLayoutBuilder, VulkanVertexBufferLayout, VulkanInputAssemblerBuilder>::VertexBufferLayoutBuilder;

	public:
		/// <inheritdoc />
		virtual VulkanVertexBufferLayoutBuilder& addAttribute(UniquePtr<BufferAttribute>&& attribute) override;

	public:
		/// <summary>
		/// Adds an attribute to the vertex buffer layout.
		/// </summary>
		/// <remarks>
		/// This overload implicitly determines the location based on the number of attributes already defined. It should only be used if all locations can be implicitly deducted.
		/// </remarks>
		/// <param name="format">The format of the attribute.</param>
		/// <param name="offset">The offset of the attribute within a buffer element.</param>
		/// <param name="semantic">The semantic of the attribute.</param>
		/// <param name="semanticIndex">The semantic index of the attribute.</param>
		virtual VulkanVertexBufferLayoutBuilder& addAttribute(const BufferFormat& format, const UInt32& offset, const AttributeSemantic& semantic = AttributeSemantic::Unknown, const UInt32& semanticIndex = 0);

		/// <summary>
		/// Adds an attribute to the vertex buffer layout.
		/// </summary>
		/// <param name="location">The location, the attribute is bound to.</param>
		/// <param name="format">The format of the attribute.</param>
		/// <param name="offset">The offset of the attribute within a buffer element.</param>
		/// <param name="semantic">The semantic of the attribute.</param>
		/// <param name="semanticIndex">The semantic index of the attribute.</param>
		virtual VulkanVertexBufferLayoutBuilder& addAttribute(const UInt32& location, const BufferFormat& format, const UInt32& offset, const AttributeSemantic& semantic = AttributeSemantic::Unknown, const UInt32& semanticIndex = 0);
	};

	/// <summary>
	/// Implements a Vulkan index buffer layout.
	/// </summary>
	/// <seealso cref="VulkanIndexBuffer" />
	/// <seealso cref="VulkanVertexBufferLayout" />
	class LITEFX_VULKAN_API VulkanIndexBufferLayout : public virtual VulkanRuntimeObject<VulkanInputAssembler>, public IIndexBufferLayout {
		LITEFX_IMPLEMENTATION(VulkanIndexBufferLayoutImpl);

	public:
		/// <summary>
		/// Initializes a new index buffer layout
		/// </summary>
		/// <param name="inputAssembler">The parent input assembler state, the index buffer layout is initialized for.</param>
		/// <param name="type">The type of the indices within the index buffer.</param>
		explicit VulkanIndexBufferLayout(const VulkanInputAssembler& inputAssembler, const IndexType& type);
		VulkanIndexBufferLayout(VulkanIndexBufferLayout&&) = delete;
		VulkanIndexBufferLayout(const VulkanIndexBufferLayout&) = delete;
		virtual ~VulkanIndexBufferLayout() noexcept;

		// IIndexBufferLayout interface.
	public:
		/// <inheritdoc />
		virtual const IndexType& indexType() const noexcept override;

		// IBufferLayout interface.
	public:
		/// <inheritdoc />
		virtual size_t elementSize() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& binding() const noexcept override;

		/// <inheritdoc />
		virtual const BufferType& type() const noexcept override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IDescriptorLayout" />
	/// </summary>
	/// <seealso cref="IVulkanBuffer" />
	/// <seealso cref="IVulkanImage" />
	/// <seealso cref="IVulkanSampler" />
	/// <seealso cref="VulkanDescriptorSet" />
	/// <seealso cref="VulkanDescriptorSetLayout" />
	class LITEFX_VULKAN_API VulkanDescriptorLayout : public virtual VulkanRuntimeObject<VulkanDescriptorSetLayout>, public IDescriptorLayout {
		LITEFX_IMPLEMENTATION(VulkanDescriptorLayoutImpl);

	public:
		/// <summary>
		/// Initializes a new Vulkan descriptor layout.
		/// </summary>
		/// <param name="descriptorSetLayout">The parent descriptor set layout.</param>
		/// <param name="type">The type of the descriptor.</param>
		/// <param name="binding">The binding point for the descriptor.</param>
		/// <param name="elementSize">The size of the descriptor.</param>
		/// <param name="elementSize">The number of descriptors in the descriptor array.</param>
		explicit VulkanDescriptorLayout(const VulkanDescriptorSetLayout& descriptorSetLayout, const DescriptorType& type, const UInt32& binding, const size_t& elementSize, const UInt32& descriptors = 1);
		VulkanDescriptorLayout(VulkanDescriptorLayout&&) = delete;
		VulkanDescriptorLayout(const VulkanDescriptorLayout&) = delete;
		virtual ~VulkanDescriptorLayout() noexcept;

		// IDescriptorLayout interface.
	public:
		/// <inheritdoc />
		virtual const DescriptorType& descriptorType() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& descriptors() const noexcept override;

		// IBufferLayout interface.
	public:
		/// <inheritdoc />
		virtual size_t elementSize() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& binding() const noexcept override;

		/// <inheritdoc />
		virtual const BufferType& type() const noexcept override;
	};

	/// <summary>
	/// Represents the base interface for a Vulkan buffer implementation.
	/// </summary>
	/// <seealso cref="VulkanDescriptorSet" />
	/// <seealso cref="IVulkanImage" />
	/// <seealso cref="IVulkanVertexBuffer" />
	/// <seealso cref="IVulkanIndexBuffer" />
	class LITEFX_VULKAN_API IVulkanBuffer : public virtual IBuffer, public virtual IResource<VkBuffer> {
	public:
		virtual ~IVulkanBuffer() noexcept = default;
	};

	/// <summary>
	/// Represents a Vulkan vertex buffer.
	/// </summary>
	/// <seealso cref="VulkanVertexBufferLayout" />
	/// <seealso cref="IVulkanBuffer" />
	class LITEFX_VULKAN_API IVulkanVertexBuffer : public virtual IVertexBuffer<VulkanVertexBufferLayout>, public IVulkanBuffer {
	public:
		virtual ~IVulkanVertexBuffer() noexcept = default;
	};

	/// <summary>
	/// Represents a Vulkan index buffer.
	/// </summary>
	/// <seealso cref="VulkanIndexBufferLayout" />
	/// <seealso cref="IVulkanBuffer" />
	class LITEFX_VULKAN_API IVulkanIndexBuffer : public virtual IIndexBuffer<VulkanIndexBufferLayout>, public IVulkanBuffer {
	public:
		virtual ~IVulkanIndexBuffer() noexcept = default;
	};

	/// <summary>
	/// Represents a Vulkan sampled image or the base interface for a texture.
	/// </summary>
	/// <seealso cref="VulkanDescriptorLayout" />
	/// <seealso cref="VulkanDescriptorSet" />
	/// <seealso cref="VulkanDescriptorSetLayout" />
	/// <seealso cref="IVulkanBuffer" />
	/// <seealso cref="IVulkanSampler" />
	class LITEFX_VULKAN_API IVulkanImage : public virtual IImage, public virtual IResource<VkImage> {
	public:
		virtual ~IVulkanImage() noexcept = default;

	public:
		virtual VkImageAspectFlags aspectMask() const noexcept = 0;
		virtual VkImageAspectFlags aspectMask(const UInt32& plane) const = 0;
		virtual void resolveSubresource(const UInt32& subresource, UInt32& plane, UInt32& layer, UInt32& level) const = 0;
		virtual const VkImageView& imageView(const UInt32& plane = 0) const = 0;
	};

	/// <summary>
	/// Represents a Vulkan sampler.
	/// </summary>
	/// <seealso cref="VulkanDescriptorLayout" />
	/// <seealso cref="VulkanDescriptorSet" />
	/// <seealso cref="VulkanDescriptorSetLayout" />
	/// <seealso cref="IVulkanImage" />
	class LITEFX_VULKAN_API IVulkanSampler : public ISampler, public virtual IResource<VkSampler> {
	public:
		virtual ~IVulkanSampler() noexcept = default;
	};

	/// <summary>
	/// Implements a Vulkan resource barrier.
	/// </summary>
	/// <seealso cref="VulkanCommandBuffer" />
	/// <seealso cref="IVulkanBuffer" />
	/// <seealso cref="IVulkanImage" />
	/// <seealso cref="IBarrier" />
	class LITEFX_VULKAN_API VulkanBarrier : public IBarrier<IVulkanBuffer, IVulkanImage> {
		LITEFX_IMPLEMENTATION(VulkanBarrierImpl);

	public:
		explicit VulkanBarrier() noexcept;
		VulkanBarrier(const VulkanBarrier&) = delete;
		VulkanBarrier(VulkanBarrier&&) = delete;
		virtual ~VulkanBarrier() noexcept;

		// IBarrier interface.
	public:
		/// <inheritdoc />
		virtual void transition(IVulkanBuffer& buffer, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanBuffer& buffer, const UInt32& element, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanBuffer& buffer, const ResourceState& sourceState, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanBuffer& buffer, const ResourceState& sourceState, const UInt32& element, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanImage& image, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanImage& image, const UInt32& level, const UInt32& layer, const UInt32& plane, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanImage& image, const ResourceState& sourceState, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void transition(IVulkanImage& image, const ResourceState& sourceState, const UInt32& level, const UInt32& layer, const UInt32& plane, const ResourceState& targetState) override;

		/// <inheritdoc />
		virtual void waitFor(const IVulkanBuffer& buffer) override;

		/// <inheritdoc />
		virtual void waitFor(const IVulkanImage& image) override;

	public:
		/// <summary>
		/// Adds the barrier to a command buffer and updates the resource target states.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to add the barriers to.</param>
		virtual void execute(const VulkanCommandBuffer& commandBuffer) const noexcept;

		/// <summary>
		/// Adds the inverse barriers to a command buffers and updates the resource target states.
		/// </summary>
		/// <remarks>
		/// This method can be used to quickly transition all resources back to the source state without requiring to record a new barrier. It performs the opposite transitions to
		/// the ones created with <see cref="execute" />.
		/// </remarks>
		/// <param name="commandBuffer">The command buffer to add the barriers to.</param>
		virtual void executeInverse(const VulkanCommandBuffer& commandBuffer) const noexcept;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IDescriptorSet" />.
	/// </summary>
	/// <seealso cref="VulkanDescriptorSetLayout" />
	class LITEFX_VULKAN_API VulkanDescriptorSet : public virtual VulkanRuntimeObject<VulkanDescriptorSetLayout>, public IDescriptorSet<IVulkanBuffer, IVulkanImage, IVulkanSampler>, public Resource<VkDescriptorSet> {
		LITEFX_IMPLEMENTATION(VulkanDescriptorSetImpl);

	public:
		/// <summary>
		/// Initializes a new descriptor set.
		/// </summary>
		/// <param name="layout">The parent descriptor set layout.</param>
		/// <param name="descriptorSet">The descriptor set handle.</param>
		explicit VulkanDescriptorSet(const VulkanDescriptorSetLayout& layout, VkDescriptorSet descriptorSet);
		VulkanDescriptorSet(VulkanDescriptorSet&&) = delete;
		VulkanDescriptorSet(const VulkanDescriptorSet&) = delete;
		virtual ~VulkanDescriptorSet() noexcept;

	public:
		/// <inheritdoc />
		virtual void update(const UInt32& binding, const IVulkanBuffer& buffer, const UInt32& bufferElement = 0, const UInt32& elements = 1, const UInt32& firstDescriptor = 0) const override;

		/// <inheritdoc />
		virtual void update(const UInt32& binding, const IVulkanImage& texture, const UInt32& descriptor = 0, const UInt32& firstLevel = 0, const UInt32& levels = 0, const UInt32& firstLayer = 0, const UInt32& layers = 0) const override;

		/// <inheritdoc />
		virtual void update(const UInt32& binding, const IVulkanSampler& sampler, const UInt32& descriptor = 0) const override;

		/// <inheritdoc />
		virtual void attach(const UInt32& binding, const IVulkanImage& image) const override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IDescriptorSetLayout" />.
	/// </summary>
	/// <seealso cref="VulkanDescriptorSet" />
	/// <seealso cref="VulkanRenderPipelineDescriptorSetLayoutBuilder" />
	class LITEFX_VULKAN_API VulkanDescriptorSetLayout : public virtual VulkanRuntimeObject<VulkanPipelineLayout>, public IDescriptorSetLayout<VulkanDescriptorLayout, VulkanDescriptorSet>, public Resource<VkDescriptorSetLayout> {
		LITEFX_IMPLEMENTATION(VulkanDescriptorSetLayoutImpl);
		LITEFX_BUILDER(VulkanRenderPipelineDescriptorSetLayoutBuilder);
		LITEFX_BUILDER(VulkanComputePipelineDescriptorSetLayoutBuilder);

	public:
		/// <summary>
		/// Initializes a Vulkan descriptor set layout.
		/// </summary>
		/// <param name="pipelineLayout">The parent pipeline layout that contains the descriptor set layout.</param>
		/// <param name="descriptorLayouts">The descriptor layouts of the descriptors within the descriptor set.</param>
		/// <param name="space">The space or set id of the descriptor set.</param>
		/// <param name="stages">The shader stages, the descriptor sets are bound to.</param>
		/// <param name="poolSize">The size of a descriptor pool.</param>
		explicit VulkanDescriptorSetLayout(const VulkanPipelineLayout& pipelineLayout, Array<UniquePtr<VulkanDescriptorLayout>>&& descriptorLayouts, const UInt32& space, const ShaderStage& stages, const UInt32& poolSize = 1024);
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		virtual ~VulkanDescriptorSetLayout() noexcept;

	private:
		explicit VulkanDescriptorSetLayout(const VulkanPipelineLayout& pipelineLayout) noexcept;

	public:
		/// <inheritdoc />
		virtual Array<const VulkanDescriptorLayout*> descriptors() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanDescriptorLayout& descriptor(const UInt32& binding) const override;

		/// <inheritdoc />
		virtual const UInt32& space() const noexcept override;

		/// <inheritdoc />
		virtual const ShaderStage& shaderStages() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 uniforms() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 storages() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 images() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 buffers() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 samplers() const noexcept override;

		/// <inheritdoc />
		virtual UInt32 inputAttachments() const noexcept override;

	public:
		/// <inheritdoc />
		virtual UniquePtr<VulkanDescriptorSet> allocate() const noexcept override;

		/// <inheritdoc />
		virtual Array<UniquePtr<VulkanDescriptorSet>> allocate(const UInt32& descriptorSets) const noexcept override;

		/// <inheritdoc />
		virtual void free(const VulkanDescriptorSet& descriptorSet) const noexcept override;

	public:
		/// <summary>
		/// The size of each descriptor pool.
		/// </summary>
		/// <remarks>
		/// Descriptors are allocated from descriptor pools in Vulkan. Each descriptor pool has a number of descriptor sets it can hand out. Before allocating a new descriptor set
		/// the layout tries to find an unused descriptor set, that it can hand out. If there are no free descriptor sets, the layout tries to allocate a new one. This is only possible
		/// if the descriptor pool is not yet full, in which case a new pool needs to be created. All created pools are cached and destroyed, if the layout itself gets destroyed, 
		/// causing all descriptor sets allocated from the layout to be invalidated. 
		/// 
		/// In general, if the number of required descriptor sets can be pre-calculated, it should be used as a pool size. Otherwise there is a trade-off to be made, based on the 
		/// frequency of which new descriptor sets are required. A small pool size is more memory efficient, but can have a significant runtime cost, as long as new allocations happen
		/// and no descriptor sets can be reused. A large pool size on the other hand is faster, whilst it may leave a large chunk of descriptor sets unallocated. Keep in mind, that the 
		/// layout might not be the only active layout, hence a large portion of descriptor sets might end up not being used.
		/// </remarks>
		/// <returns>The size of one descriptor pool.</returns>
		/// <seealso cref="allocate" />
		/// <seealso cref="free" />
		/// <seealso cref="pools" />
		virtual const UInt32& poolSize() const noexcept;

		/// <summary>
		/// Returns the number of active descriptor pools.
		/// </summary>
		/// <returns>The number of active descriptor pools.</returns>
		/// <seealso cref="allocate" />
		/// <seealso cref="free" />
		/// <seealso cref="poolSize" />
		virtual size_t pools() const noexcept;
	};

	/// <summary>
	/// Builds a <see cref="VulkanDescriptorSetLayout" /> for a render pipeline.
	/// </summary>
	/// <seealso cref="VulkanDescriptorSetLayout" />
	/// <seealso cref="VulkanRenderPipeline" />
	class LITEFX_VULKAN_API VulkanRenderPipelineDescriptorSetLayoutBuilder : public DescriptorSetLayoutBuilder<VulkanRenderPipelineDescriptorSetLayoutBuilder, VulkanDescriptorSetLayout, VulkanRenderPipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineDescriptorSetLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan descriptor set layout builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		/// <param name="space">The space the descriptor set is bound to.</param>
		/// <param name="stages">The shader stages, the descriptor set is accessible from.</param>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocations.</param>
		explicit VulkanRenderPipelineDescriptorSetLayoutBuilder(VulkanRenderPipelineLayoutBuilder& parent, const UInt32& space = 0, const ShaderStage& stages = ShaderStage::Compute | ShaderStage::Fragment | ShaderStage::Geometry | ShaderStage::TessellationControl | ShaderStage::TessellationEvaluation | ShaderStage::Vertex, const UInt32& poolSize = 1024);
		VulkanRenderPipelineDescriptorSetLayoutBuilder(const VulkanRenderPipelineDescriptorSetLayoutBuilder&) = delete;
		VulkanRenderPipelineDescriptorSetLayoutBuilder(VulkanRenderPipelineDescriptorSetLayoutBuilder&&) = delete;
		virtual ~VulkanRenderPipelineDescriptorSetLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineLayoutBuilder& go() override;

		// DescriptorSetLayoutBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder& addDescriptor(UniquePtr<VulkanDescriptorLayout>&& layout) override;
		
		/// <inheritdoc />
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder& addDescriptor(const DescriptorType& type, const UInt32& binding, const UInt32& descriptorSize, const UInt32& descriptors = 1) override;

		// VulkanRenderPipelineDescriptorSetLayoutBuilder.
	public:
		/// <summary>
		/// Sets the space, the descriptor set is bound to.
		/// </summary>
		/// <param name="space">The space, the descriptor set is bound to.</param>
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder& space(const UInt32& space) noexcept;

		/// <summary>
		/// Sets the shader stages, the descriptor set is accessible from.
		/// </summary>
		/// <param name="stages">The shader stages, the descriptor set is accessible from.</param>
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder& shaderStages(const ShaderStage& stages) noexcept;

		/// <summary>
		/// Sets the size of the descriptor pools used for descriptor set allocations.
		/// </summary>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocations.</param>
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder& poolSize(const UInt32& poolSize) noexcept;
	};

	/// <summary>
	/// Builds a <see cref="VulkanDescriptorSetLayout" /> for a compute pipeline.
	/// </summary>
	/// <seealso cref="VulkanDescriptorSetLayout" />
	/// <seealso cref="VulkanComputePipeline" />
	class LITEFX_VULKAN_API VulkanComputePipelineDescriptorSetLayoutBuilder : public DescriptorSetLayoutBuilder<VulkanComputePipelineDescriptorSetLayoutBuilder, VulkanDescriptorSetLayout, VulkanComputePipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanComputePipelineDescriptorSetLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan descriptor set layout builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		/// <param name="space">The space the descriptor set is bound to.</param>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocations.</param>
		explicit VulkanComputePipelineDescriptorSetLayoutBuilder(VulkanComputePipelineLayoutBuilder& parent, const UInt32& space = 0, const UInt32& poolSize = 1024);
		VulkanComputePipelineDescriptorSetLayoutBuilder(const VulkanComputePipelineDescriptorSetLayoutBuilder&) = delete;
		VulkanComputePipelineDescriptorSetLayoutBuilder(VulkanComputePipelineDescriptorSetLayoutBuilder&&) = delete;
		virtual ~VulkanComputePipelineDescriptorSetLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputePipelineLayoutBuilder& go() override;

		// DescriptorSetLayoutBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputePipelineDescriptorSetLayoutBuilder& addDescriptor(UniquePtr<VulkanDescriptorLayout>&& layout) override;

		/// <inheritdoc />
		virtual VulkanComputePipelineDescriptorSetLayoutBuilder& addDescriptor(const DescriptorType& type, const UInt32& binding, const UInt32& descriptorSize, const UInt32& descriptors = 1) override;

		// VulkanComputePipelineDescriptorSetLayoutBuilder.
	public:
		/// <summary>
		/// Sets the space, the descriptor set is bound to.
		/// </summary>
		/// <param name="space">The space, the descriptor set is bound to.</param>
		virtual VulkanComputePipelineDescriptorSetLayoutBuilder& space(const UInt32& space) noexcept;

		/// <summary>
		/// Sets the size of the descriptor pools used for descriptor set allocations.
		/// </summary>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocations.</param>
		virtual VulkanComputePipelineDescriptorSetLayoutBuilder& poolSize(const UInt32& poolSize) noexcept;
	};

	/// <summary>
	/// Implements the Vulkan <see cref="IPushConstantsRange" />.
	/// </summary>
	/// <seealso cref="VulkanPushConstantsLayout" />
	class LITEFX_VULKAN_API VulkanPushConstantsRange : public IPushConstantsRange {
		LITEFX_IMPLEMENTATION(VulkanPushConstantsRangeImpl);

	public:
		/// <summary>
		/// Initializes a new push constants range.
		/// </summary>
		/// <param name="shaderStage">The shader stage, that access the push constants from the range.</param>
		/// <param name="offset">The offset relative to the parent push constants backing memory that marks the beginning of the range.</param>
		/// <param name="size">The size of the push constants range.</param>
		/// <param name="space">The space from which the push constants of the range will be accessible in the shader.</param>
		/// <param name="binding">The register from which the push constants of the range will be accessible in the shader.</param>
		explicit VulkanPushConstantsRange(const ShaderStage& shaderStage, const UInt32& offset, const UInt32& size, const UInt32& space, const UInt32& binding);
		VulkanPushConstantsRange(const VulkanPushConstantsRange&) = delete;
		VulkanPushConstantsRange(VulkanPushConstantsRange&&) = delete;
		virtual ~VulkanPushConstantsRange() noexcept;

	public:
		/// <inheritdoc />
		virtual const UInt32& space() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& binding() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& offset() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& size() const noexcept override;

		/// <inheritdoc />
		virtual const ShaderStage& stage() const noexcept override;
	};

	/// <summary>
	/// Implements the Vulkan <see cref="IPushConstantsLayout" />.
	/// </summary>
	/// <seealso cref="VulkanPushConstantsRange" />
	/// <seealso cref="VulkanRenderPipelinePushConstantsLayoutBuilder" />
	/// <seealso cref="VulkanComputePipelinePushConstantsLayoutBuilder" />
	class LITEFX_VULKAN_API VulkanPushConstantsLayout : public virtual VulkanRuntimeObject<VulkanPipelineLayout>, public IPushConstantsLayout<VulkanPushConstantsRange> {
		LITEFX_IMPLEMENTATION(VulkanPushConstantsLayoutImpl);
		LITEFX_BUILDER(VulkanRenderPipelinePushConstantsLayoutBuilder);
		LITEFX_BUILDER(VulkanComputePipelinePushConstantsLayoutBuilder);

	public:
		/// <summary>
		/// Initializes a new push constants layout.
		/// </summary>
		/// <param name="parent">The parent pipeline layout.</param>
		/// <param name="ranges">The ranges contained by the layout.</param>
		/// <param name="size">The overall size (in bytes) of the push constants backing memory.</param>
		explicit VulkanPushConstantsLayout(const VulkanPipelineLayout& parent, Array<UniquePtr<VulkanPushConstantsRange>>&& ranges, const UInt32& size);
		VulkanPushConstantsLayout(const VulkanPushConstantsLayout&) = delete;
		VulkanPushConstantsLayout(VulkanPushConstantsLayout&&) = delete;
		virtual ~VulkanPushConstantsLayout() noexcept;

	private:
		explicit VulkanPushConstantsLayout(const VulkanPipelineLayout& parent, const UInt32& size);

	public:
		/// <inheritdoc />
		virtual const UInt32& size() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanPushConstantsRange& range(const ShaderStage& stage) const override;

		/// <inheritdoc />
		virtual Array<const VulkanPushConstantsRange*> ranges() const noexcept override;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IPushConstantsLayout" /> for a <see cref="IRenderPipeline" />.
	/// </summary>
	/// <seealso cref="VulkanPushConstantsLayout" />
	class LITEFX_VULKAN_API VulkanRenderPipelinePushConstantsLayoutBuilder : public PushConstantsLayoutBuilder<VulkanRenderPipelinePushConstantsLayoutBuilder, VulkanPushConstantsLayout, VulkanRenderPipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelinePushConstantsLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan render pipeline push constants layout builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		/// <param name="size">The size of the push constants backing memory.</param>
		explicit VulkanRenderPipelinePushConstantsLayoutBuilder(VulkanRenderPipelineLayoutBuilder& parent, const UInt32& size);
		VulkanRenderPipelinePushConstantsLayoutBuilder(const VulkanRenderPipelinePushConstantsLayoutBuilder&) = delete;
		VulkanRenderPipelinePushConstantsLayoutBuilder(VulkanRenderPipelinePushConstantsLayoutBuilder&&) = delete;
		virtual ~VulkanRenderPipelinePushConstantsLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineLayoutBuilder& go() override;

		// PushConstantsLayoutBuilder interface.
	public:
		virtual VulkanRenderPipelinePushConstantsLayoutBuilder& addRange(const ShaderStage& shaderStages, const UInt32& offset, const UInt32& size, const UInt32& space, const UInt32& binding) override;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IPushConstantsLayout" /> for a <see cref="IComputePipeline" />.
	/// </summary>
	/// <seealso cref="VulkanPushConstantsLayout" />
	class LITEFX_VULKAN_API VulkanComputePipelinePushConstantsLayoutBuilder : public PushConstantsLayoutBuilder<VulkanComputePipelinePushConstantsLayoutBuilder, VulkanPushConstantsLayout, VulkanComputePipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanComputePipelinePushConstantsLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan compute pipeline push constants layout builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		/// <param name="size">The size of the push constants backing memory.</param>
		explicit VulkanComputePipelinePushConstantsLayoutBuilder(VulkanComputePipelineLayoutBuilder& parent, const UInt32& size);
		VulkanComputePipelinePushConstantsLayoutBuilder(const VulkanComputePipelinePushConstantsLayoutBuilder&) = delete;
		VulkanComputePipelinePushConstantsLayoutBuilder(VulkanComputePipelinePushConstantsLayoutBuilder&&) = delete;
		virtual ~VulkanComputePipelinePushConstantsLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputePipelineLayoutBuilder& go() override;

		// PushConstantsLayoutBuilder interface.
	public:
		virtual VulkanComputePipelinePushConstantsLayoutBuilder& addRange(const ShaderStage& shaderStages, const UInt32& offset, const UInt32& size, const UInt32& space, const UInt32& binding) override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IShaderModule" />.
	/// </summary>
	/// <seealso cref="VulkanShaderProgram" />
	class LITEFX_VULKAN_API VulkanShaderModule : public virtual VulkanRuntimeObject<VulkanDevice>, public IShaderModule, public Resource<VkShaderModule> {
		LITEFX_IMPLEMENTATION(VulkanShaderModuleImpl);

	public:
		/// <summary>
		/// Initializes a new Vulkan shader module.
		/// </summary>
		/// <param name="device">The parent device, this shader module has been created from.</param>
		/// <param name="type">The shader stage, this module is used in.</param>
		/// <param name="fileName">The file name of the module source.</param>
		/// <param name="entryPoint">The name of the module entry point.</param>
		explicit VulkanShaderModule(const VulkanDevice& device, const ShaderStage& type, const String& fileName, const String& entryPoint = "main");
		VulkanShaderModule(const VulkanShaderModule&) noexcept = delete;
		VulkanShaderModule(VulkanShaderModule&&) noexcept = delete;
		virtual ~VulkanShaderModule() noexcept;

		// IShaderModule interface.
	public:
		/// <inheritdoc />
		virtual const String& fileName() const noexcept override;

		/// <inheritdoc />
		virtual const String& entryPoint() const noexcept override;

		/// <inheritdoc />
		virtual const ShaderStage& type() const noexcept override;

	public:
		/// <summary>
		/// Returns the shader stage creation info for convenience.
		/// </summary>
		/// <returns>The shader stage creation info for convenience.</returns>
		virtual VkPipelineShaderStageCreateInfo shaderStageDefinition() const;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IShaderProgram" />.
	/// </summary>
	/// <seealso cref="VulkanShaderProgramBuilder" />
	class LITEFX_VULKAN_API VulkanShaderProgram : public virtual VulkanRuntimeObject<VulkanPipelineLayout>, public IShaderProgram<VulkanShaderModule> {
		LITEFX_IMPLEMENTATION(VulkanShaderProgramImpl);
		LITEFX_BUILDER(VulkanGraphicsShaderProgramBuilder);
		LITEFX_BUILDER(VulkanComputeShaderProgramBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan shader program.
		/// </summary>
		/// <param name="pipelineLayout">The parent pipeline layout to initialize the shader program from.</param>
		/// <param name="modules">The shader modules used by the shader program.</param>
		explicit VulkanShaderProgram(const VulkanPipelineLayout& pipelineLayout, Array<UniquePtr<VulkanShaderModule>>&& modules);
		VulkanShaderProgram(VulkanShaderProgram&&) noexcept = delete;
		VulkanShaderProgram(const VulkanShaderProgram&) noexcept = delete;
		virtual ~VulkanShaderProgram() noexcept;

	private:
		explicit VulkanShaderProgram(const VulkanPipelineLayout& pipelineLayout) noexcept;

	public:
		/// <inheritdoc />
		virtual Array<const VulkanShaderModule*> modules() const noexcept override;
	};

	/// <summary>
	/// Builds a Vulkan graphics <see cref="IShaderProgram" />.
	/// </summary>
	/// <seealso cref="VulkanShaderProgram" />
	class LITEFX_VULKAN_API VulkanGraphicsShaderProgramBuilder : public GraphicsShaderProgramBuilder<VulkanGraphicsShaderProgramBuilder, VulkanShaderProgram, VulkanRenderPipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanGraphicsShaderProgramBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan graphics shader program builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		explicit VulkanGraphicsShaderProgramBuilder(VulkanRenderPipelineLayoutBuilder& parent);
		VulkanGraphicsShaderProgramBuilder(const VulkanGraphicsShaderProgramBuilder&) = delete;
		VulkanGraphicsShaderProgramBuilder(VulkanGraphicsShaderProgramBuilder&&) = delete;
		virtual ~VulkanGraphicsShaderProgramBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineLayoutBuilder& go() override;

		// ShaderProgramBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addShaderModule(const ShaderStage& type, const String& fileName, const String& entryPoint = "main") override;

		// GraphicsShaderProgramBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addVertexShaderModule(const String& fileName, const String& entryPoint = "main") override;

		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addTessellationControlShaderModule(const String& fileName, const String& entryPoint = "main") override;

		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addTessellationEvaluationShaderModule(const String& fileName, const String& entryPoint = "main") override;

		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addGeometryShaderModule(const String& fileName, const String& entryPoint = "main") override;

		/// <inheritdoc />
		virtual VulkanGraphicsShaderProgramBuilder& addFragmentShaderModule(const String& fileName, const String& entryPoint = "main") override;
	};

	/// <summary>
	/// Builds a Vulkan compute <see cref="IShaderProgram" />.
	/// </summary>
	/// <seealso cref="VulkanShaderProgram" />
	class LITEFX_VULKAN_API VulkanComputeShaderProgramBuilder : public ComputeShaderProgramBuilder<VulkanComputeShaderProgramBuilder, VulkanShaderProgram, VulkanComputePipelineLayoutBuilder> {
		LITEFX_IMPLEMENTATION(VulkanComputeShaderProgramBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan compute shader program builder.
		/// </summary>
		/// <param name="parent">The parent pipeline layout builder.</param>
		explicit VulkanComputeShaderProgramBuilder(VulkanComputePipelineLayoutBuilder& parent);
		VulkanComputeShaderProgramBuilder(const VulkanComputeShaderProgramBuilder&) = delete;
		VulkanComputeShaderProgramBuilder(VulkanComputeShaderProgramBuilder&&) = delete;
		virtual ~VulkanComputeShaderProgramBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputePipelineLayoutBuilder& go() override;

		// ShaderProgramBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputeShaderProgramBuilder& addShaderModule(const ShaderStage& type, const String& fileName, const String& entryPoint = "main") override;

		// ComputeShaderProgramBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputeShaderProgramBuilder& addComputeShaderModule(const String& fileName, const String& entryPoint = "main") override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IPipelineLayout" />.
	/// </summary>
	/// <seealso cref="VulkanRenderPipelineLayoutBuilder" />
	class LITEFX_VULKAN_API VulkanPipelineLayout : public virtual VulkanRuntimeObject<VulkanPipelineState>, public IPipelineLayout<VulkanDescriptorSetLayout, VulkanPushConstantsLayout, VulkanShaderProgram>, public Resource<VkPipelineLayout> {
		LITEFX_IMPLEMENTATION(VulkanPipelineLayoutImpl);
		LITEFX_BUILDER(VulkanRenderPipelineLayoutBuilder);
		LITEFX_BUILDER(VulkanComputePipelineLayoutBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan render pipeline layout.
		/// </summary>
		/// <param name="pipeline">The parent pipeline state the layout describes.</param>
		/// <param name="shaderProgram">The shader program used by the pipeline.</param>
		/// <param name="descriptorSetLayouts">The descriptor set layouts used by the pipeline.</param>
		/// <param name="pushConstantsLayout">The push constants layout used by the pipeline.</param>
		explicit VulkanPipelineLayout(const VulkanRenderPipeline& pipeline, UniquePtr<VulkanShaderProgram>&& shaderProgram, Array<UniquePtr<VulkanDescriptorSetLayout>>&& descriptorSetLayouts, UniquePtr<VulkanPushConstantsLayout>&& pushConstantsLayout);

		/// <summary>
		/// Initializes a new Vulkan compute pipeline layout.
		/// </summary>
		/// <param name="pipeline">The parent pipeline state the layout describes.</param>
		/// <param name="shaderProgram">The shader program used by the pipeline.</param>
		/// <param name="descriptorSetLayouts">The descriptor set layouts used by the pipeline.</param>
		/// <param name="pushConstantsLayout">The push constants layout used by the pipeline.</param>
		explicit VulkanPipelineLayout(const VulkanComputePipeline& pipeline, UniquePtr<VulkanShaderProgram>&& shaderProgram, Array<UniquePtr<VulkanDescriptorSetLayout>>&& descriptorSetLayouts, UniquePtr<VulkanPushConstantsLayout>&& pushConstantsLayout);

		VulkanPipelineLayout(VulkanPipelineLayout&&) noexcept = delete;
		VulkanPipelineLayout(const VulkanPipelineLayout&) noexcept = delete;
		virtual ~VulkanPipelineLayout() noexcept;

	private:
		explicit VulkanPipelineLayout(const VulkanRenderPipeline& pipeline) noexcept;
		explicit VulkanPipelineLayout(const VulkanComputePipeline& pipeline) noexcept;

		// IPipelineLayout interface.
	public:
		/// <inheritdoc />
		virtual const VulkanShaderProgram& program() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanDescriptorSetLayout& descriptorSet(const UInt32& space) const override;

		/// <inheritdoc />
		virtual Array<const VulkanDescriptorSetLayout*> descriptorSets() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanPushConstantsLayout* pushConstants() const noexcept override;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IPipelineLayout" /> for a render pipeline.
	/// </summary>
	/// <seealso cref="VulkanPipelineLayout" />
	/// <seealso cref="VulkanRenderPipeline" />
	class LITEFX_VULKAN_API VulkanRenderPipelineLayoutBuilder : public PipelineLayoutBuilder<VulkanRenderPipelineLayoutBuilder, VulkanPipelineLayout, VulkanRenderPipelineBuilder> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a new Vulkan render pipeline layout builder.
		/// </summary>
		VulkanRenderPipelineLayoutBuilder(VulkanRenderPipelineBuilder& parent);
		VulkanRenderPipelineLayoutBuilder(VulkanRenderPipelineLayoutBuilder&&) = delete;
		VulkanRenderPipelineLayoutBuilder(const VulkanRenderPipelineLayoutBuilder&) = delete;
		virtual ~VulkanRenderPipelineLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineBuilder& go() override;

		// RenderPipelineBuilder interface.
	public:
		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanShaderProgram>&& program) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanDescriptorSetLayout>&& layout) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanPushConstantsLayout>&& layout) override;

		// VulkanRenderPipelineBuilder.
	public:
		/// <summary>
		/// Builds a shader program for the render pipeline layout.
		/// </summary>
		virtual VulkanGraphicsShaderProgramBuilder shaderProgram();

		/// <summary>
		/// Builds a new descriptor set for the render pipeline layout.
		/// </summary>
		/// <param name="space">The space, the descriptor set is bound to.</param>
		/// <param name="stages">The stages, the descriptor set will be accessible from.</param>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocation.</param>
		virtual VulkanRenderPipelineDescriptorSetLayoutBuilder addDescriptorSet(const UInt32& space = 0, const ShaderStage& stages = ShaderStage::Fragment | ShaderStage::Geometry | ShaderStage::TessellationControl | ShaderStage::TessellationEvaluation | ShaderStage::Vertex, const UInt32& poolSize = 1024);

		/// <summary>
		/// Builds a new push constants layout for the render pipeline layout.
		/// </summary>
		/// <param name="size">The size of the push constants backing memory.</param>
		virtual VulkanRenderPipelinePushConstantsLayoutBuilder addPushConstants(const UInt32& size);
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IPipelineLayout" /> for a compute pipeline.
	/// </summary>
	/// <seealso cref="VulkanPipelineLayout" />
	/// <seealso cref="VulkanComputePipeline" />
	class LITEFX_VULKAN_API VulkanComputePipelineLayoutBuilder : public PipelineLayoutBuilder<VulkanComputePipelineLayoutBuilder, VulkanPipelineLayout, VulkanComputePipelineBuilder> {
		LITEFX_IMPLEMENTATION(VulkanComputePipelineLayoutBuilderImpl);

	public:
		/// <summary>
		/// Initializes a new Vulkan compute pipeline layout builder.
		/// </summary>
		VulkanComputePipelineLayoutBuilder(VulkanComputePipelineBuilder& parent);
		VulkanComputePipelineLayoutBuilder(VulkanComputePipelineLayoutBuilder&&) = delete;
		VulkanComputePipelineLayoutBuilder(const VulkanComputePipelineLayoutBuilder&) = delete;
		virtual ~VulkanComputePipelineLayoutBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanComputePipelineBuilder& go() override;

		// ComputePipelineBuilder interface.
	public:
		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanShaderProgram>&& program) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanDescriptorSetLayout>&& layout) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanPushConstantsLayout>&& layout) override;

		// VulkanComputePipelineBuilder.
	public:
		/// <summary>
		/// Builds a shader program for the render pipeline layout.
		/// </summary>
		virtual VulkanComputeShaderProgramBuilder shaderProgram();

		/// <summary>
		/// Builds a new descriptor set for the render pipeline layout.
		/// </summary>
		/// <param name="space">The space, the descriptor set is bound to.</param>
		/// <param name="poolSize">The size of the descriptor pools used for descriptor set allocation.</param>
		virtual VulkanComputePipelineDescriptorSetLayoutBuilder addDescriptorSet(const UInt32& space = 0, const UInt32& poolSize = 1024);

		/// <summary>
		/// Builds a new push constants layout for the render pipeline layout.
		/// </summary>
		/// <param name="size">The size of the push constants backing memory.</param>
		virtual VulkanComputePipelinePushConstantsLayoutBuilder addPushConstants(const UInt32& size);
	};

	/// <summary>
	/// Implements the Vulkan input assembler state.
	/// </summary>
	/// <seealso cref="VulkanInputAssemblerBuilder" />
	class LITEFX_VULKAN_API VulkanInputAssembler : public virtual VulkanRuntimeObject<VulkanDevice>, public IInputAssembler<VulkanVertexBufferLayout, VulkanIndexBufferLayout> {
		LITEFX_IMPLEMENTATION(VulkanInputAssemblerImpl);
		LITEFX_BUILDER(VulkanInputAssemblerBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan input assembler state.
		/// </summary>
		/// <param name="device">The parent device.</param>
		/// <param name="vertexBufferLayouts">The vertex buffer layouts supported by the input assembler state. Each layout must have a unique binding.</param>
		/// <param name="indexBufferLayout">The index buffer layout.</param>
		/// <param name="primitiveTopology">The primitive topology.</param>
		explicit VulkanInputAssembler(const VulkanDevice& device, Array<UniquePtr<VulkanVertexBufferLayout>>&& vertexBufferLayouts, UniquePtr<VulkanIndexBufferLayout>&& indexBufferLayout, const PrimitiveTopology& primitiveTopology = PrimitiveTopology::TriangleList);
		VulkanInputAssembler(VulkanInputAssembler&&) noexcept = delete;
		VulkanInputAssembler(const VulkanInputAssembler&) noexcept = delete;
		virtual ~VulkanInputAssembler() noexcept;

	private:
		explicit VulkanInputAssembler(const VulkanDevice& device) noexcept;

	public:
		/// <inheritdoc />
		virtual Array<const VulkanVertexBufferLayout*> vertexBufferLayouts() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanVertexBufferLayout& vertexBufferLayout(const UInt32& binding) const override;

		/// <inheritdoc />
		virtual const VulkanIndexBufferLayout& indexBufferLayout() const override;

		/// <inheritdoc />
		virtual const PrimitiveTopology& topology() const noexcept override;
	};

	/// <summary>
	/// Builds a <see cref="VulkanInputAssembler" />.
	/// </summary>
	/// <seealso cref="VulkanInputAssembler" />
	class LITEFX_VULKAN_API VulkanInputAssemblerBuilder : public InputAssemblerBuilder<VulkanInputAssemblerBuilder, VulkanInputAssembler, VulkanRenderPipelineBuilder> {
		LITEFX_IMPLEMENTATION(VulkanInputAssemblerBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan input assembler builder.
		/// </summary>
		/// <param name="parent">The parent render pipeline builder.</param>
		explicit VulkanInputAssemblerBuilder(VulkanRenderPipelineBuilder& parent) noexcept;
		VulkanInputAssemblerBuilder(const VulkanInputAssemblerBuilder&) noexcept = delete;
		VulkanInputAssemblerBuilder(VulkanInputAssemblerBuilder&&) noexcept = delete;
		virtual ~VulkanInputAssemblerBuilder() noexcept;

	public:
		/// <summary>
		/// Starts building a vertex buffer layout.
		/// </summary>
		/// <param name="elementSize">The size of a vertex within the vertex buffer.</param>
		/// <param name="binding">The binding point to bind the vertex buffer to.</param>
		virtual VulkanVertexBufferLayoutBuilder addVertexBuffer(const size_t& elementSize, const UInt32& binding = 0);

		/// <summary>
		/// Starts building an index buffer layout.
		/// </summary>
		/// <param name="type">The type of the index buffer.</param>
		virtual VulkanInputAssemblerBuilder& withIndexType(const IndexType& type);

		// IInputAssemblerBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanInputAssemblerBuilder& withTopology(const PrimitiveTopology& topology) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanVertexBufferLayout>&& layout) override;

		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanIndexBufferLayout>&& layout) override;

		// Builder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineBuilder& go() override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IRasterizer" />.
	/// </summary>
	/// <seealso cref="VulkanRasterizerBuilder" />
	class LITEFX_VULKAN_API VulkanRasterizer : public virtual VulkanRuntimeObject<VulkanRenderPipeline>, public Rasterizer {
		LITEFX_BUILDER(VulkanRasterizerBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan rasterizer state.
		/// </summary>
		/// <param name="pipeline">The parent pipeline, the rasterizer state is created for.</param>
		/// <param name="polygonMode">The polygon mode used by the pipeline.</param>
		/// <param name="cullMode">The cull mode used by the pipeline.</param>
		/// <param name="cullOrder">The cull order used by the pipeline.</param>
		/// <param name="lineWidth">The line width used by the pipeline.</param>
		/// <param name="depthStencilState">The rasterizer depth/stencil state.</param>
		explicit VulkanRasterizer(const VulkanRenderPipeline& pipeline, const PolygonMode& polygonMode, const CullMode& cullMode, const CullOrder& cullOrder, const Float& lineWidth = 1.f, const DepthStencilState& depthStencilState = {}) noexcept;
		VulkanRasterizer(VulkanRasterizer&&) noexcept = delete;
		VulkanRasterizer(const VulkanRasterizer&) noexcept = delete;
		virtual ~VulkanRasterizer() noexcept;

	private:
		explicit VulkanRasterizer(const VulkanRenderPipeline& pipeline) noexcept;

	public:
		/// <summary>
		/// Sets the line width on the rasterizer.
		/// </summary>
		/// <remarks>
		/// Note that updating the line width requires the "wide lines" feature to be available. If it is not, the line width **must** be `1.0`. This
		/// constraint is not enforced by the engine and you are responsible of making sure that it is fulfilled.
		/// 
		/// Furthermore, note that the DirectX 12 back-end does have any representation for the line width concept. Thus you should only use the line 
		/// width, if you plan to only support Vulkan.
		/// </remarks>
		/// <returns>A reference to the line width.</returns>
		/// <seealso href="https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features-wideLines" />
		virtual void updateLineWidth(const Float& lineWidth) noexcept;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IRasterizer" />.
	/// </summary>
	/// <seealso cref="VulkanRasterizer" />
	class LITEFX_VULKAN_API VulkanRasterizerBuilder : public RasterizerBuilder<VulkanRasterizerBuilder, VulkanRasterizer, VulkanRenderPipelineBuilder> {
		LITEFX_IMPLEMENTATION(VulkanRasterizerBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan input assembler builder.
		/// </summary>
		/// <param name="parent">The parent render pipeline builder.</param>
		explicit VulkanRasterizerBuilder(VulkanRenderPipelineBuilder& parent) noexcept;
		VulkanRasterizerBuilder(const VulkanRasterizerBuilder&) noexcept = delete;
		VulkanRasterizerBuilder(VulkanRasterizerBuilder&&) noexcept = delete;
		virtual ~VulkanRasterizerBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRenderPipelineBuilder& go() override;

		// RasterizerBuilder interface.
	public:
		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withPolygonMode(const PolygonMode& mode = PolygonMode::Solid) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withCullMode(const CullMode& cullMode = CullMode::BackFaces) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withCullOrder(const CullOrder& cullOrder = CullOrder::CounterClockWise) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withLineWidth(const Float& lineWidth = 1.f) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withDepthBias(const DepthStencilState::DepthBias& depthBias) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withDepthState(const DepthStencilState::DepthState& depthState) noexcept override;

		/// <inheritdoc />
		virtual VulkanRasterizerBuilder& withStencilState(const DepthStencilState::StencilState& stencilState) noexcept override;
	};

	/// <summary>
	/// Defines the base class for Vulkan pipeline state objects.
	/// </summary>
	class LITEFX_VULKAN_API VulkanPipelineState : public virtual IPipeline<VulkanPipelineLayout>, public Resource<VkPipeline> {
	public:
		using Resource<VkPipeline>::Resource;
		virtual ~VulkanPipelineState() noexcept = default;

	public:
		/// <summary>
		/// Sets the current pipeline state on the <paramref name="commandBuffer" />.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to set the current pipeline state on.</param>
		virtual void use(const VulkanCommandBuffer& commandBuffer) const noexcept = 0;

		/// <summary>
		/// Binds a descriptor set on a command buffer.
		/// </summary>
		/// <param name="commandBuffer">The command buffer to issue the bind command on.</param>
		/// <param name="descriptorSet">The descriptor set to bind.</param>
		virtual void bind(const VulkanCommandBuffer& commandBuffer, const VulkanDescriptorSet& descriptorSet) const noexcept = 0;
	};

	/// <summary>
	/// Records commands for a <see cref="VulkanCommandQueue" />
	/// </summary>
	/// <seealso cref="VulkanQueue" />
	class LITEFX_VULKAN_API VulkanCommandBuffer : public virtual VulkanRuntimeObject<VulkanQueue>, public ICommandBuffer<IVulkanBuffer, IVulkanVertexBuffer, IVulkanIndexBuffer, IVulkanImage, VulkanBarrier, VulkanPipelineState>, public Resource<VkCommandBuffer> {
		LITEFX_IMPLEMENTATION(VulkanCommandBufferImpl);

	public:
		/// <summary>
		/// Initializes a command buffer from a command queue.
		/// </summary>
		/// <param name="queue">The parent command queue, the buffer gets submitted to.</param>
		/// <param name="begin">If set to <c>true</c>, the command buffer automatically starts recording by calling <see cref="begin" />.</param>
		/// <param name="primary"><c>true</c>, if the command buffer is a primary command buffer.</param>
		explicit VulkanCommandBuffer(const VulkanQueue& queue, const bool& begin = false, const bool& primary = true);
		VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer(VulkanCommandBuffer&&) = delete;
		virtual ~VulkanCommandBuffer() noexcept;

		// Vulkan Command Buffer interface.
	public:
		/// <summary>
		/// Begins the command buffer as a secondary command buffer that inherits the state of <paramref name="renderPass" />.
		/// </summary>
		/// <param name="renderPass">The render pass state to inherit.</param>
		virtual void begin(const VulkanRenderPass& renderPass) const noexcept;

		// ICommandBuffer interface.
	public:
		/// <inheritdoc />
		virtual void begin() const override;

		/// <inheritdoc />
		virtual void end() const override;

		/// <inheritdoc />
		virtual void generateMipMaps(IVulkanImage& image) noexcept override;

		/// <inheritdoc />
		virtual void barrier(const VulkanBarrier& barrier, const bool& invert = false) const noexcept override;

		/// <inheritdoc />
		virtual void transfer(const IVulkanBuffer& source, const IVulkanBuffer& target, const UInt32& sourceElement = 0, const UInt32& targetElement = 0, const UInt32& elements = 1) const override;

		/// <inheritdoc />
		virtual void transfer(const IVulkanBuffer& source, const IVulkanImage& target, const UInt32& sourceElement = 0, const UInt32& firstSubresource = 0, const UInt32& elements = 1) const override;

		/// <inheritdoc />
		virtual void transfer(const IVulkanImage& source, const IVulkanImage& target, const UInt32& sourceSubresource = 0, const UInt32& targetSubresource = 0, const UInt32& subresources = 1) const override;

		/// <inheritdoc />
		virtual void transfer(const IVulkanImage& source, const IVulkanBuffer& target, const UInt32& firstSubresource = 0, const UInt32& targetElement = 0, const UInt32& subresources = 1) const override;

		/// <inheritdoc />
		virtual void use(const VulkanPipelineState& pipeline) const noexcept override;

		/// <inheritdoc />
		virtual void bind(const VulkanDescriptorSet& descriptorSet) const noexcept override;

		/// <inheritdoc />
		virtual void bind(const IVulkanVertexBuffer& buffer) const noexcept override;

		/// <inheritdoc />
		virtual void bind(const IVulkanIndexBuffer& buffer) const noexcept override;

		/// <inheritdoc />
		virtual void dispatch(const Vector3u& threadCount) const noexcept override;

		/// <inheritdoc />
		virtual void draw(const UInt32& vertices, const UInt32& instances = 1, const UInt32& firstVertex = 0, const UInt32& firstInstance = 0) const noexcept override;

		/// <inheritdoc />
		virtual void drawIndexed(const UInt32& indices, const UInt32& instances = 1, const UInt32& firstIndex = 0, const Int32& vertexOffset = 0, const UInt32& firstInstance = 0) const noexcept override;

		/// <inheritdoc />
		virtual void pushConstants(const VulkanPushConstantsLayout& layout, const void* const memory) const noexcept override;
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IRenderPipeline" />.
	/// </summary>
	/// <seealso cref="VulkanRenderPipelineBuilder" />
	class LITEFX_VULKAN_API VulkanRenderPipeline : public virtual VulkanRuntimeObject<VulkanRenderPass>, public IRenderPipeline<VulkanPipelineLayout, VulkanInputAssembler, IVulkanVertexBuffer, IVulkanIndexBuffer>, public VulkanPipelineState {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineImpl);
		LITEFX_BUILDER(VulkanRenderPipelineBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan render pipeline.
		/// </summary>
		/// <param name="renderPass">The parent render pass.</param>
		/// <param name="id">The unique ID of the pipeline within the render pass.</param>
		/// <param name="name">The optional debug name of the render pipeline.</param>
		/// <param name="enableAlphaToCoverage">Whether or not to enable Alpha-to-Coverage multi-sampling.</param>
		explicit VulkanRenderPipeline(const VulkanRenderPass& renderPass, const UInt32& id, UniquePtr<VulkanPipelineLayout>&& layout, SharedPtr<VulkanInputAssembler>&& inputAssembler, SharedPtr<VulkanRasterizer>&& rasterizer, Array<SharedPtr<IViewport>>&& viewports, Array<SharedPtr<IScissor>>&& scissors, const bool& enableAlphaToCoverage = false, const String& name = "");
		VulkanRenderPipeline(VulkanRenderPipeline&&) noexcept = delete;
		VulkanRenderPipeline(const VulkanRenderPipeline&) noexcept = delete;
		virtual ~VulkanRenderPipeline() noexcept;

	private:
		VulkanRenderPipeline(const VulkanRenderPass& renderPass) noexcept;

		// IPipeline interface.
	public:
		/// <inheritdoc />
		virtual const String& name() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanPipelineLayout& layout() const noexcept override;

		// IRenderPipeline interface.
	public:
		/// <inheritdoc />
		virtual const UInt32& id() const noexcept override;

		/// <inheritdoc />
		virtual SharedPtr<VulkanInputAssembler> inputAssembler() const noexcept override;

		/// <inheritdoc />
		virtual SharedPtr<IRasterizer> rasterizer() const noexcept override;

		/// <inheritdoc />
		virtual Array<const IViewport*> viewports() const noexcept override;

		/// <inheritdoc />
		virtual Array<const IScissor*> scissors() const noexcept override;

		/// <inheritdoc />
		virtual UInt32& stencilRef() const noexcept override;

		/// <inheritdoc />
		virtual Vector4f& blendFactors() const noexcept override;

		/// <inheritdoc />
		virtual const bool& alphaToCoverage() const noexcept override;

		// VulkanPipelineState interface.
	public:
		/// <inheritdoc />
		virtual void use(const VulkanCommandBuffer& commandBuffer) const noexcept override;

		/// <inheritdoc />
		virtual void bind(const VulkanCommandBuffer& commandBuffer, const VulkanDescriptorSet& descriptorSet) const noexcept override;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IRenderPipeline" />.
	/// </summary>
	/// <seealso cref="VulkanRenderPipeline" />
	class LITEFX_VULKAN_API VulkanRenderPipelineBuilder : public RenderPipelineBuilder<VulkanRenderPipelineBuilder, VulkanRenderPipeline> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan render pipeline builder.
		/// </summary>
		/// <param name="renderPass">The parent render pass</param>
		/// <param name="id">A unique identifier for the render pipeline.</param>
		/// <param name="name">A debug name for the render pipeline.</param>
		explicit VulkanRenderPipelineBuilder(const VulkanRenderPass& renderPass, const UInt32& id, const String& name = "");
		VulkanRenderPipelineBuilder(VulkanRenderPipelineBuilder&&) = delete;
		VulkanRenderPipelineBuilder(const VulkanRenderPipelineBuilder&) = delete;
		virtual ~VulkanRenderPipelineBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		[[nodiscard]] virtual UniquePtr<VulkanRenderPipeline> go() override;

		// RenderPipelineBuilder interface.
	public:
		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanPipelineLayout>&& layout) override;

		/// <inheritdoc />
		virtual void use(SharedPtr<IRasterizer> rasterizer) override;

		/// <inheritdoc />
		virtual void use(SharedPtr<VulkanInputAssembler> inputAssembler) override;

		/// <inheritdoc />
		virtual void use(SharedPtr<IViewport> viewport) override;

		/// <inheritdoc />
		virtual void use(SharedPtr<IScissor> scissor) override;

		/// <inheritdoc />
		virtual VulkanRenderPipelineBuilder& enableAlphaToCoverage(const bool& enable = true) override;

		// VulkanRenderPipelineBuilder.
	public:
		/// <summary>
		/// Builds a <see cref="VulkanPipelineLayout" /> for the render pipeline.
		/// </summary>
		virtual VulkanRenderPipelineLayoutBuilder layout();

		/// <summary>
		/// Builds a <see cref="VulkanRasterizer" /> for the render pipeline.
		/// </summary>
		virtual VulkanRasterizerBuilder rasterizer();

		/// <summary>
		/// Builds a <see cref="VulkanInputAssembler" /> for the render pipeline.
		/// </summary>
		virtual VulkanInputAssemblerBuilder inputAssembler();

		/// <summary>
		/// Uses the provided rasterizer state for the render pipeline.
		/// </summary>
		/// <param name="rasterizer">The rasterizer state used by the render pipeline.</param>
		virtual VulkanRenderPipelineBuilder& withRasterizer(SharedPtr<IRasterizer> rasterizer);

		/// <summary>
		/// Uses the provided input assembler state for the render pipeline.
		/// </summary>
		/// <param name="inputAssembler">The input assembler state used by the render pipeline.</param>
		virtual VulkanRenderPipelineBuilder& withInputAssembler(SharedPtr<VulkanInputAssembler> inputAssembler);

		/// <summary>
		/// Adds the provided viewport to the render pipeline.
		/// </summary>
		/// <param name="viewport">The viewport to add to the render pipeline.</param>
		virtual VulkanRenderPipelineBuilder& withViewport(SharedPtr<IViewport> viewport);

		/// <summary>
		/// Adds the provided scissor to the render pipeline.
		/// </summary>
		/// <param name="scissor">The scissor to add to the render pipeline.</param>
		virtual VulkanRenderPipelineBuilder& withScissor(SharedPtr<IScissor> scissor);
	};

	/// <summary>
	/// Implements a Vulkan <see cref="IComputePipeline" />.
	/// </summary>
	/// <seealso cref="VulkanComputePipelineBuilder" />
	class LITEFX_VULKAN_API VulkanComputePipeline : public virtual VulkanRuntimeObject<VulkanDevice>, public IComputePipeline<VulkanPipelineLayout>, public VulkanPipelineState {
		LITEFX_IMPLEMENTATION(VulkanComputePipelineImpl);
		LITEFX_BUILDER(VulkanComputePipelineBuilder);

	public:
		/// <summary>
		/// Initializes a new Vulkan compute pipeline.
		/// </summary>
		/// <param name="device">The parent device.</param>
		/// <param name="name">The optional debug name of the render pipeline.</param>
		explicit VulkanComputePipeline(const VulkanDevice& device, UniquePtr<VulkanPipelineLayout>&& layout, const String& name = "");
		VulkanComputePipeline(VulkanComputePipeline&&) noexcept = delete;
		VulkanComputePipeline(const VulkanComputePipeline&) noexcept = delete;
		virtual ~VulkanComputePipeline() noexcept;

	private:
		VulkanComputePipeline(const VulkanDevice& device) noexcept;

		// IPipeline interface.
	public:
		/// <inheritdoc />
		virtual const String& name() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanPipelineLayout& layout() const noexcept override;

		// VulkanPipelineState interface.
	public:
		/// <inheritdoc />
		virtual void use(const VulkanCommandBuffer& commandBuffer) const noexcept override;

		/// <inheritdoc />
		virtual void bind(const VulkanCommandBuffer& commandBuffer, const VulkanDescriptorSet& descriptorSet) const noexcept override;
	};

	/// <summary>
	/// Builds a Vulkan <see cref="IComputePipeline" />.
	/// </summary>
	/// <seealso cref="VulkanComputePipeline" />
	class LITEFX_VULKAN_API VulkanComputePipelineBuilder : public ComputePipelineBuilder<VulkanComputePipelineBuilder, VulkanComputePipeline> {
		LITEFX_IMPLEMENTATION(VulkanComputePipelineBuilderImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan compute pipeline builder.
		/// </summary>
		/// <param name="device">The parent device</param>
		/// <param name="name">A debug name for the compute pipeline.</param>
		explicit VulkanComputePipelineBuilder(const VulkanDevice& device, const String& name = "");
		VulkanComputePipelineBuilder(VulkanComputePipelineBuilder&&) = delete;
		VulkanComputePipelineBuilder(const VulkanComputePipelineBuilder&) = delete;
		virtual ~VulkanComputePipelineBuilder() noexcept;

		// IBuilder interface.
	public:
		/// <inheritdoc />
		[[nodiscard]] virtual UniquePtr<VulkanComputePipeline> go() override;

		// ComputePipelineBuilder interface.
	public:
		/// <inheritdoc />
		virtual void use(UniquePtr<VulkanPipelineLayout>&& layout) override;

		// VulkanComputePipelineBuilder.
	public:
		/// <summary>
		/// Builds a <see cref="VulkanPipelineLayout" /> for the compute pipeline.
		/// </summary>
		virtual VulkanComputePipelineLayoutBuilder layout();
	};

	/// <summary>
	/// Implements a Vulkan frame buffer.
	/// </summary>
	class LITEFX_VULKAN_API VulkanFrameBuffer : public virtual VulkanRuntimeObject<VulkanRenderPass>, public IFrameBuffer<VulkanCommandBuffer>, public Resource<VkFramebuffer> {
		LITEFX_IMPLEMENTATION(VulkanFrameBufferImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan frame buffer.
		/// </summary>
		/// <param name="renderPass">The parent render pass of the frame buffer.</param>
		/// <param name="bufferIndex">The index of the frame buffer within the parent render pass.</param>
		/// <param name="renderArea">The initial size of the render area.</param>
		/// <param name="commandBuffers">The number of command buffers, the frame buffer stores.</param>
		VulkanFrameBuffer(const VulkanRenderPass& renderPass, const UInt32& bufferIndex, const Size2d& renderArea, const UInt32& commandBuffers = 1);
		VulkanFrameBuffer(const VulkanFrameBuffer&) noexcept = delete;
		VulkanFrameBuffer(VulkanFrameBuffer&&) noexcept = delete;
		virtual ~VulkanFrameBuffer() noexcept;

		// Vulkan frame buffer interface.
	public:
		/// <summary>
		/// Returns a reference of the semaphore, that can be used to signal, that the frame buffer is finished.
		/// </summary>
		/// <returns>A reference of the semaphore, that can be used to signal, that the frame buffer is finished.</returns>
		virtual const VkSemaphore& semaphore() const noexcept;

		/// <summary>
		/// Returns a reference of the last fence value for the frame buffer.
		/// </summary>
		/// <remarks>
		/// The frame buffer must only be re-used, if this fence is reached in the graphics queue.
		/// </remarks>
		/// <returns>A reference of the last fence value for the frame buffer.</returns>
		virtual UInt64& lastFence() const noexcept;

		// IFrameBuffer interface.
	public:
		/// <inheritdoc />
		virtual const UInt32& bufferIndex() const noexcept override;

		/// <inheritdoc />
		virtual const Size2d& size() const noexcept override;

		/// <inheritdoc />
		virtual size_t getWidth() const noexcept override;

		/// <inheritdoc />
		virtual size_t getHeight() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanCommandBuffer& commandBuffer(const UInt32& index) const override;

		/// <inheritdoc />
		virtual Array<const VulkanCommandBuffer*> commandBuffers() const noexcept override;

		/// <inheritdoc />
		virtual Array<const IVulkanImage*> images() const noexcept override;

		/// <inheritdoc />
		virtual const IVulkanImage& image(const UInt32& location) const override;

	public:
		/// <inheritdoc />
		virtual void resize(const Size2d& renderArea) override;
	};

	/// <summary>
	/// Implements a Vulkan render pass.
	/// </summary>
	/// <seealso cref="VulkanRenderPassBuilder" />
	class LITEFX_VULKAN_API VulkanRenderPass : public virtual VulkanRuntimeObject<VulkanDevice>, public IRenderPass<VulkanRenderPipeline, VulkanFrameBuffer, VulkanInputAttachmentMapping>, public Resource<VkRenderPass> {
		LITEFX_IMPLEMENTATION(VulkanRenderPassImpl);
		LITEFX_BUILDER(VulkanRenderPassBuilder);

	public:
		/// <summary>
		/// Creates and initializes a new Vulkan render pass instance.
		/// </summary>
		/// <param name="device">The parent device instance.</param>
		/// <param name="commandBuffers">The number of command buffers in each frame buffer.</param>
		/// <param name="renderTargets">The render targets that are output by the render pass.</param>
		/// <param name="samples">The number of samples for the render targets in this render pass.</param>
		/// <param name="inputAttachments">The input attachments that are read by the render pass.</param>
		explicit VulkanRenderPass(const VulkanDevice& device, Span<RenderTarget> renderTargets, const UInt32& commandBuffers = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1, Span<VulkanInputAttachmentMapping> inputAttachments = { });
		VulkanRenderPass(const VulkanRenderPass&) = delete;
		VulkanRenderPass(VulkanRenderPass&&) = delete;
		virtual ~VulkanRenderPass() noexcept;

	private:
		/// <summary>
		/// Creates an uninitialized Vulkan render pass instance.
		/// </summary>
		/// <remarks>
		/// This constructor is called by the <see cref="VulkanRenderPassBuilder" /> in order to create a render pass instance without initializing it. The instance 
		/// is only initialized after calling <see cref="VulkanRenderPassBuilder::go" />.
		/// </remarks>
		/// <param name="device">The parent device of the render pass.</param>
		explicit VulkanRenderPass(const VulkanDevice& device) noexcept;

		// IInputAttachmentMappingSource interface.
	public:
		/// <inheritdoc />
		virtual const VulkanFrameBuffer& frameBuffer(const UInt32& buffer) const override;

		// IRenderPass interface.
	public:
		/// <inheritdoc />
		virtual const VulkanFrameBuffer& activeFrameBuffer() const override;

		/// <inheritdoc />
		virtual Array<const VulkanFrameBuffer*> frameBuffers() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanRenderPipeline& pipeline(const UInt32& id) const override;

		/// <inheritdoc />
		virtual Array<const VulkanRenderPipeline*> pipelines() const noexcept override;

		/// <inheritdoc />
		virtual const RenderTarget& renderTarget(const UInt32& location) const override;

		/// <inheritdoc />
		virtual Span<const RenderTarget> renderTargets() const noexcept override;

		/// <inheritdoc />
		virtual bool hasPresentTarget() const noexcept override;

		/// <inheritdoc />
		virtual Span<const VulkanInputAttachmentMapping> inputAttachments() const noexcept override;

		/// <inheritdoc />
		virtual const MultiSamplingLevel& multiSamplingLevel() const noexcept override;

	public:
		/// <inheritdoc />
		virtual void begin(const UInt32& buffer) override;
		
		/// <inheritdoc />
		virtual void end() const override;

		/// <inheritdoc />
		virtual void resizeFrameBuffers(const Size2d& renderArea) override;

		/// <inheritdoc />
		virtual void changeMultiSamplingLevel(const MultiSamplingLevel& samples) override;

		/// <inheritdoc />
		virtual void updateAttachments(const VulkanDescriptorSet& descriptorSet) const override;

		// VulkanRenderPass.
	public:
		/// <summary>
		/// Starts building a render pipeline.
		/// </summary>
		/// <param name="id">A unique ID for the render pipeline.</param>
		/// <param name="name">A debug name for the render pipeline.</param>
		/// <seealso cref="VulkanRenderPipeline" />
		/// <seealso cref="VulkanRenderPipelineBuilder" />
		virtual VulkanRenderPipelineBuilder makePipeline(const UInt32& id, const String& name = "") const noexcept;
	};

	/// <summary>
	/// Implements the Vulkan <see cref="RenderPassBuilder" />.
	/// </summary>
	/// <seealso cref="VulkanRenderPass" />
	class LITEFX_VULKAN_API VulkanRenderPassBuilder : public RenderPassBuilder<VulkanRenderPassBuilder, VulkanRenderPass> {
		LITEFX_IMPLEMENTATION(VulkanRenderPassBuilderImpl)

	public:
		explicit VulkanRenderPassBuilder(const VulkanDevice& device, const MultiSamplingLevel& samples = MultiSamplingLevel::x1) noexcept;
		explicit VulkanRenderPassBuilder(const VulkanDevice& device, const UInt32& commandBuffers) noexcept;
		explicit VulkanRenderPassBuilder(const VulkanDevice& device, const UInt32& commandBuffers, const MultiSamplingLevel& multiSamplingLevel) noexcept;
		VulkanRenderPassBuilder(const VulkanRenderPassBuilder&) noexcept = delete;
		VulkanRenderPassBuilder(VulkanRenderPassBuilder&&) noexcept = delete;
		virtual ~VulkanRenderPassBuilder() noexcept;
	public:
		virtual void use(RenderTarget&& target) override;
		virtual void use(VulkanInputAttachmentMapping&& inputAttachment) override;

	public:
		virtual VulkanRenderPassBuilder& commandBuffers(const UInt32& count) override;
		virtual VulkanRenderPassBuilder& renderTarget(const RenderTargetType& type, const Format& format, const Vector4f& clearValues = { 0.0f, 0.0f, 0.0f, 0.0f }, bool clearColor = true, bool clearStencil = true, bool isVolatile = false) override;
		virtual VulkanRenderPassBuilder& renderTarget(const UInt32& location, const RenderTargetType& type, const Format& format, const Vector4f& clearValues = { 0.0f, 0.0f, 0.0f, 0.0f }, bool clearColor = true, bool clearStencil = true, bool isVolatile = false) override;
		virtual VulkanRenderPassBuilder& renderTarget(VulkanInputAttachmentMapping& output, const RenderTargetType& type, const Format& format, const Vector4f& clearValues = { 0.0f, 0.0f, 0.0f, 0.0f }, bool clearColor = true, bool clearStencil = true, bool isVolatile = false) override;
		virtual VulkanRenderPassBuilder& renderTarget(VulkanInputAttachmentMapping& output, const UInt32& location, const RenderTargetType& type, const Format& format, const Vector4f& clearValues = { 0.0f, 0.0f, 0.0f, 0.0f }, bool clearColor = true, bool clearStencil = true, bool isVolatile = false) override;
		virtual VulkanRenderPassBuilder& setMultiSamplingLevel(const MultiSamplingLevel& samples = MultiSamplingLevel::x4) override;
		virtual VulkanRenderPassBuilder& inputAttachment(const VulkanInputAttachmentMapping& inputAttachment) override;
		virtual VulkanRenderPassBuilder& inputAttachment(const UInt32& inputLocation, const VulkanRenderPass& renderPass, const UInt32& outputLocation) override;
		virtual VulkanRenderPassBuilder& inputAttachment(const UInt32& inputLocation, const VulkanRenderPass& renderPass, const RenderTarget& renderTarget) override;

	public:
		virtual UniquePtr<VulkanRenderPass> go() override;
	};

	/// <summary>
	/// Implements a <see cref="IInputAttachmentMapping" />.
	/// </summary>
	/// <seealso cref="VulkanRenderPass" />
	/// <seealso cref="VulkanRenderPassBuilder" />
	class LITEFX_VULKAN_API VulkanInputAttachmentMapping : public IInputAttachmentMapping<VulkanRenderPass> {
		LITEFX_IMPLEMENTATION(VulkanInputAttachmentMappingImpl);

	public:
		VulkanInputAttachmentMapping() noexcept;

		/// <summary>
		/// Creates a new Vulkan input attachment mapping.
		/// </summary>
		/// <param name="renderPass">The render pass to fetch the input attachment from.</param>
		/// <param name="renderTarget">The render target of the <paramref name="renderPass"/> that is used for the input attachment.</param>
		/// <param name="location">The location to bind the input attachment to.</param>
		VulkanInputAttachmentMapping(const VulkanRenderPass& renderPass, const RenderTarget& renderTarget, const UInt32& location);
		VulkanInputAttachmentMapping(const VulkanInputAttachmentMapping&) noexcept;
		VulkanInputAttachmentMapping(VulkanInputAttachmentMapping&&) noexcept;
		virtual ~VulkanInputAttachmentMapping() noexcept;

	public:
		inline VulkanInputAttachmentMapping& operator=(const VulkanInputAttachmentMapping&) noexcept;
		inline VulkanInputAttachmentMapping& operator=(VulkanInputAttachmentMapping&&) noexcept;

	public:
		/// <inheritdoc />
		virtual const VulkanRenderPass* inputAttachmentSource() const noexcept override;

		/// <inheritdoc />
		virtual const RenderTarget& renderTarget() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& location() const noexcept override;
	};

	/// <summary>
	/// Implements a Vulkan swap chain.
	/// </summary>
	class LITEFX_VULKAN_API VulkanSwapChain : public virtual VulkanRuntimeObject<VulkanDevice>, public ISwapChain<IVulkanImage>, public Resource<VkSwapchainKHR> {
		LITEFX_IMPLEMENTATION(VulkanSwapChainImpl);

	public:
		/// <summary>
		/// Initializes a Vulkan swap chain.
		/// </summary>
		/// <param name="device">The device that owns the swap chain.</param>
		/// <param name="format">The initial surface format.</param>
		/// <param name="renderArea">The initial size of the render area.</param>
		/// <param name="buffers">The initial number of buffers.</param>
		explicit VulkanSwapChain(const VulkanDevice& device, const Format& surfaceFormat = Format::B8G8R8A8_SRGB, const Size2d& renderArea = { 800, 600 }, const UInt32& buffers = 3);
		VulkanSwapChain(const VulkanSwapChain&) = delete;
		VulkanSwapChain(VulkanSwapChain&&) = delete;
		virtual ~VulkanSwapChain() noexcept;

		// Vulkan Swap Chain interface.
	public:
		/// <summary>
		/// Returns a reference of the current swap semaphore, a command queue can wait on for presenting.
		/// </summary>
		/// <returns>A reference of the current swap semaphore, a command queue can wait on for presenting.</returns>
		virtual const VkSemaphore& semaphore() const noexcept;

		// ISwapChain interface.
	public:
		/// <inheritdoc />
		virtual const Format& surfaceFormat() const noexcept override;

		/// <inheritdoc />
		virtual const UInt32& buffers() const noexcept override;

		/// <inheritdoc />
		virtual const Size2d& renderArea() const noexcept override;

		/// <inheritdoc />
		virtual Array<const IVulkanImage*> images() const noexcept override;

	public:
		/// <inheritdoc />
		virtual Array<Format> getSurfaceFormats() const noexcept override;

		/// <inheritdoc />
		virtual void reset(const Format& surfaceFormat, const Size2d& renderArea, const UInt32& buffers) override;

		/// <inheritdoc />
		[[nodiscard]] virtual UInt32 swapBackBuffer() const override;
	};

	/// <summary>
	/// Implements a Vulkan command queue.
	/// </summary>
	/// <seealso cref="VulkanCommandBuffer" />
	class LITEFX_VULKAN_API VulkanQueue : public virtual VulkanRuntimeObject<VulkanDevice>, public ICommandQueue<VulkanCommandBuffer>, public Resource<VkQueue> {
		LITEFX_IMPLEMENTATION(VulkanQueueImpl);
	
	public:
		/// <summary>
		/// Initializes the Vulkan command queue.
		/// </summary>
		/// <param name="device">The device, commands get send to.</param>
		/// <param name="type">The type of the command queue.</param>
		/// <param name="priority">The priority, of which commands are issued on the device.</param>
		/// <param name="familyId">The ID of the queue family.</param>
		/// <param name="queueId">The ID of the queue.</param>
		explicit VulkanQueue(const VulkanDevice& device, const QueueType& type, const QueuePriority& priority, const UInt32& familyId, const UInt32& queueId);
		VulkanQueue(const VulkanQueue&) = delete;
		VulkanQueue(VulkanQueue&&) = delete;
		virtual ~VulkanQueue() noexcept;

		// VulkanQueue interface.
	public:
		/// <summary>
		/// Returns a reference of the command pool that is used to allocate commands.
		/// </summary>
		/// <remarks>
		/// Note that the command pool does only exist, if the queue is bound on a device.
		/// </remarks>
		/// <seealso cref="isBound" />
		/// <seealso cref="bind" />
		/// <seealso cref="release" />
		/// <returns>A reference of the command pool that is used to allocate commands</returns>
		virtual const VkCommandPool& commandPool() const noexcept;

		/// <summary>
		/// Returns the queue family ID.
		/// </summary>
		/// <returns>The queue family ID.</returns>
		virtual const UInt32& familyId() const noexcept;

		/// <summary>
		/// Returns the queue ID.
		/// </summary>
		/// <returns>The queue ID.</returns>
		virtual const UInt32& queueId() const noexcept;

		/// <summary>
		/// Returns the internal timeline semaphore used to synchronize the queue execution.
		/// </summary>
		/// <returns>The internal timeline semaphore.</returns>
		virtual const VkSemaphore& timelineSemaphore() const noexcept;

		/// <summary>
		/// Submits a single command buffer and inserts a fence to wait for it.
		/// </summary>
		/// <remarks>
		/// Note that submitting a command buffer that is currently recording will implicitly close the command buffer.
		/// </remarks>
		/// <param name="commandBuffer">The command buffer to submit to the command queue.</param>
		/// <param name="waitForSemaphores">The semaphores to wait for on each pipeline stage. There must be a semaphore for each entry in the <see cref="waitForStages" /> array.</param>
		/// <param name="waitForStages">The pipeline stages of the current render pass to wait for before submitting the command buffer.</param>
		/// <param name="signalSemaphores">The semaphores to signal, when the command buffer is executed.</param>
		/// <returns>The value of the fence, inserted after the command buffer.</returns>
		/// <seealso cref="waitFor" />
		virtual UInt64 submit(const VulkanCommandBuffer& commandBuffer, Span<VkSemaphore> waitForSemaphores, Span<VkPipelineStageFlags> waitForStages, Span<VkSemaphore> signalSemaphores = { }) const;

		/// <summary>
		/// Submits a set of command buffers and inserts a fence to wait for them.
		/// </summary>
		/// <remarks>
		/// Note that submitting a command buffer that is currently recording will implicitly close the command buffer.
		/// </remarks>
		/// <param name="commandBuffers">The command buffers to submit to the command queue.</param>
		/// <param name="waitForSemaphores">The semaphores to wait for on each pipeline stage. There must be a semaphore for each entry in the <see cref="waitForStages" /> array.</param>
		/// <param name="waitForStages">The pipeline stages of the current render pass to wait for before submitting the command buffer.</param>
		/// <param name="signalSemaphores">The semaphores to signal, when the command buffer is executed.</param>
		/// <returns>The value of the fence, inserted after the command buffers.</returns>
		/// <seealso cref="waitFor" />
		virtual UInt64 submit(const Array<const VulkanCommandBuffer*>& commandBuffers, Span<VkSemaphore> waitForSemaphores, Span<VkPipelineStageFlags> waitForStages, Span<VkSemaphore> signalSemaphores = { }) const;

		/// <summary>
		/// Creates a command buffer that can be used to allocate commands on the queue.
		/// </summary>
		/// <param name="secondary">If set to <c>true</c>, the queue will create a secondary command buffer instance.</param>
		/// <param name="beginRecording">If set to <c>true</c>, the command buffer will be initialized in recording state and can receive commands straight away.</param>
		/// <returns>The instance of the command buffer.</returns>
		virtual UniquePtr<VulkanCommandBuffer> createCommandBuffer(const bool& secondary, const bool& beginRecording) const;

		// ICommandQueue interface.
	public:
		/// <inheritdoc />
		virtual bool isBound() const noexcept override;

		/// <inheritdoc />
		virtual const QueuePriority& priority() const noexcept override;

		/// <inheritdoc />
		virtual const QueueType& type() const noexcept override;

	public:
		/// <inheritdoc />
		virtual void bind() override;

		/// <inheritdoc />
		virtual void release() override;

		/// <inheritdoc />
		virtual UniquePtr<VulkanCommandBuffer> createCommandBuffer(const bool& beginRecording = false) const override;

		/// <inheritdoc />
		virtual UInt64 submit(const VulkanCommandBuffer& commandBuffer) const override;

		/// <inheritdoc />
		virtual UInt64 submit(const Array<const VulkanCommandBuffer*>& commandBuffers) const override;

		/// <inheritdoc />
		virtual void waitFor(const UInt64& fence) const noexcept override;

		/// <inheritdoc />
		virtual UInt64 currentFence() const noexcept override;
	};

	/// <summary>
	/// A graphics factory that produces objects for a <see cref="VulkanDevice" />.
	/// </summary>
	/// <remarks>
	/// Internally this factory implementation is based on <a href="https://gpuopen.com/vulkan-memory-allocator/" target="_blank">Vulkan Memory Allocator</a>.
	/// </remarks>
	class LITEFX_VULKAN_API VulkanGraphicsFactory : public IGraphicsFactory<VulkanDescriptorLayout, IVulkanBuffer, IVulkanVertexBuffer, IVulkanIndexBuffer, IVulkanImage, IVulkanSampler> {
		LITEFX_IMPLEMENTATION(VulkanGraphicsFactoryImpl);

	public:
		/// <summary>
		/// Creates a new graphics factory.
		/// </summary>
		/// <param name="device">The device the factory should produce objects for.</param>
		explicit VulkanGraphicsFactory(const VulkanDevice& device);
		VulkanGraphicsFactory(const VulkanGraphicsFactory&) = delete;
		VulkanGraphicsFactory(VulkanGraphicsFactory&&) = delete;
		virtual ~VulkanGraphicsFactory() noexcept;

	public:
		/// <inheritdoc />
		virtual UniquePtr<IVulkanBuffer> createBuffer(const BufferType& type, const BufferUsage& usage, const size_t& elementSize, const UInt32& elements = 1, const bool& allowWrite = false) const override;

		/// <inheritdoc />
		virtual UniquePtr<IVulkanVertexBuffer> createVertexBuffer(const VulkanVertexBufferLayout& layout, const BufferUsage& usage, const UInt32& elements = 1) const override;

		/// <inheritdoc />
		virtual UniquePtr<IVulkanIndexBuffer> createIndexBuffer(const VulkanIndexBufferLayout& layout, const BufferUsage& usage, const UInt32& elements) const override;

		/// <inheritdoc />
		virtual UniquePtr<IVulkanImage> createAttachment(const Format& format, const Size2d& size, const MultiSamplingLevel& samples = MultiSamplingLevel::x1) const override;

		/// <inheritdoc />
		virtual UniquePtr<IVulkanImage> createTexture(const Format& format, const Size3d& size, const ImageDimensions& dimension = ImageDimensions::DIM_2, const UInt32& levels = 1, const UInt32& layers = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1, const bool& allowWrite = false) const override;

		/// <inheritdoc />
		virtual Array<UniquePtr<IVulkanImage>> createTextures(const UInt32& elements, const Format& format, const Size3d& size, const ImageDimensions& dimension = ImageDimensions::DIM_2, const UInt32& levels = 1, const UInt32& layers = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1, const bool& allowWrite = false) const override;

		/// <inheritdoc />
		virtual UniquePtr<IVulkanSampler> createSampler(const FilterMode& magFilter = FilterMode::Nearest, const FilterMode& minFilter = FilterMode::Nearest, const BorderMode& borderU = BorderMode::Repeat, const BorderMode& borderV = BorderMode::Repeat, const BorderMode& borderW = BorderMode::Repeat, const MipMapMode& mipMapMode = MipMapMode::Nearest, const Float& mipMapBias = 0.f, const Float& maxLod = std::numeric_limits<Float>::max(), const Float& minLod = 0.f, const Float& anisotropy = 0.f) const override;

		/// <inheritdoc />
		virtual Array<UniquePtr<IVulkanSampler>> createSamplers(const UInt32& elements, const FilterMode& magFilter = FilterMode::Nearest, const FilterMode& minFilter = FilterMode::Nearest, const BorderMode& borderU = BorderMode::Repeat, const BorderMode& borderV = BorderMode::Repeat, const BorderMode& borderW = BorderMode::Repeat, const MipMapMode& mipMapMode = MipMapMode::Nearest, const Float& mipMapBias = 0.f, const Float& maxLod = std::numeric_limits<Float>::max(), const Float& minLod = 0.f, const Float& anisotropy = 0.f) const override;
	};

	/// <summary>
	/// Implements a Vulkan graphics device.
	/// </summary>
	class LITEFX_VULKAN_API VulkanDevice : public IGraphicsDevice<VulkanGraphicsFactory, VulkanSurface, VulkanGraphicsAdapter, VulkanSwapChain, VulkanQueue, VulkanRenderPass>, public Resource<VkDevice> {
		LITEFX_IMPLEMENTATION(VulkanDeviceImpl);

	public:
		/// <summary>
		/// Creates a new device instance.
		/// </summary>
		/// <param name="adapter">The adapter the device uses for drawing.</param>
		/// <param name="surface">The surface, the device should draw to.</param>
		/// <param name="extensions">The required extensions the device gets initialized with.</param>
		explicit VulkanDevice(const VulkanGraphicsAdapter& adapter, const VulkanSurface& surface, Span<String> extensions = { });

		/// <summary>
		/// Creates a new device instance.
		/// </summary>
		/// <param name="adapter">The adapter the device uses for drawing.</param>
		/// <param name="surface">The surface, the device should draw to.</param>
		/// <param name="format">The initial surface format, device uses for drawing.</param>
		/// <param name="frameBufferSize">The initial size of the frame buffers.</param>
		/// <param name="frameBuffers">The initial number of frame buffers.</param>
		/// <param name="extensions">The required extensions the device gets initialized with.</param>
		explicit VulkanDevice(const VulkanGraphicsAdapter& adapter, const VulkanSurface& surface, const Format& format, const Size2d& frameBufferSize, const UInt32& frameBuffers, Span<String> extensions = { });

		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&&) = delete;
		virtual ~VulkanDevice() noexcept;

		// Vulkan Device interface.
	public:
		/// <summary>
		/// Returns the array that stores the extensions that were used to initialize the device.
		/// </summary>
		/// <returns>A reference to the array that stores the extensions that were used to initialize the device.</returns>
		virtual Span<const String> enabledExtensions() const noexcept;

		/// <summary>
		/// Returns a reference of the swap chain.
		/// </summary>
		/// <returns>A reference of the swap chain.</returns>
		virtual VulkanSwapChain& swapChain() noexcept;

	public:
		/// <summary>
		/// Returns a builder for a <see cref="VulkanRenderPass" />.
		/// </summary>
		/// <param name="samples">The number of samples, the render targets of the render pass should be sampled with.</param>
		/// <param name="commandBuffers">The number of command buffers in each frame buffer.</param>
		/// <returns>An instance of a builder that is used to create a new render pass.</returns>
		/// <seealso cref="IGraphicsDevice::build" />
		[[nodiscard]] VulkanRenderPassBuilder buildRenderPass(const MultiSamplingLevel& samples = MultiSamplingLevel::x1, const UInt32& commandBuffers = 1) const;

		/// <summary>
		/// Returns a builder for a <see cref="VulkanComputePipelineBuilder" />.
		/// </summary>
		/// <returns>An instance of a builder that is used to create a new compute pipeline.</returns>
		[[nodiscard]] VulkanComputePipelineBuilder buildComputePipeline() const;

		// IGraphicsDevice interface.
	public:
		/// <inheritdoc />
		virtual const VulkanSwapChain& swapChain() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanSurface& surface() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanGraphicsAdapter& adapter() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanGraphicsFactory& factory() const noexcept override;
		
		/// <inheritdoc />
		virtual const VulkanQueue& graphicsQueue() const noexcept override;
		
		/// <inheritdoc />
		virtual const VulkanQueue& transferQueue() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanQueue& bufferQueue() const noexcept override;

		/// <inheritdoc />
		virtual const VulkanQueue& computeQueue() const noexcept override;

		/// <inheritdoc />
		virtual MultiSamplingLevel maximumMultiSamplingLevel(const Format& format) const noexcept override;

	public:
		/// <inheritdoc />
		virtual void wait() const override;
	};

	/// <summary>
	/// Defines a rendering backend that creates a Vulkan device.
	/// </summary>
	class LITEFX_VULKAN_API VulkanBackend : public IRenderBackend<VulkanDevice>, public Resource<VkInstance> {
		LITEFX_IMPLEMENTATION(VulkanBackendImpl);

	public:
		/// <summary>
		/// Initializes a new vulkan rendering backend.
		/// </summary>
		/// <param name="app">An instance of the app that owns the backend.</param>
		/// <param name="extensions">A set of instance extensions to enable on the backend instance.</param>
		/// <param name="validationLayers">A set of validation layers to enable on the rendering backend.</param>
		explicit VulkanBackend(const App& app, const Span<String> extensions = { }, const Span<String> validationLayers = { });
		VulkanBackend(const VulkanBackend&) noexcept = delete;
		VulkanBackend(VulkanBackend&&) noexcept = delete;
		virtual ~VulkanBackend() noexcept;

		// Vulkan Backend interface.
	public:
		/// <summary>
		/// A callback that creates a surface from a Vulkan instance.
		/// </summary>
		typedef std::function<VkSurfaceKHR(const VkInstance&)> surface_callback;

	public:
		/// <summary>
		/// Returns the validation layers that are enabled on the backend.
		/// </summary>
		/// <returns>An array of validation layers that are enabled on the backend.</returns>
		virtual Span<const String> getEnabledValidationLayers() const noexcept;

		/// <summary>
		/// Creates a surface using the <paramref name="predicate" /> callback.
		/// </summary>
		/// <param name="predicate">A callback that gets called with the backend instance handle and creates the surface instance</param>
		/// <returns>The instance of the created surface.</returns>
		/// <seealso cref="surface_callback" />
		UniquePtr<VulkanSurface> createSurface(surface_callback predicate) const;

#ifdef VK_USE_PLATFORM_WIN32_KHR
		/// <summary>
		/// Creates a surface on a window handle.
		/// </summary>
		/// <param name="hwnd">The window handle on which the surface should be created.</param>
		/// <returns>The instance of the created surface.</returns>
		UniquePtr<VulkanSurface> createSurface(const HWND& hwnd) const;
#endif

	public:
		/// <summary>
		/// Returns <c>true</c>, if all elements of <paramref cref="extensions" /> are contained by the a list of available extensions.
		/// </summary>
		/// <returns><c>true</c>, if all elements of <paramref cref="extensions" /> are contained by the a list of available extensions.</returns>
		/// <seealso cref="getAvailableInstanceExtensions" />
		static bool validateInstanceExtensions(Span<const String> extensions) noexcept;

		/// <summary>
		/// Returns a list of available extensions.
		/// </summary>
		/// <returns>A list of available extensions.</returns>
		/// <seealso cref="validateInstanceExtensions" />
		static Array<String> getAvailableInstanceExtensions() noexcept;

		/// <summary>
		/// Returns <c>true</c>, if all elements of <paramref cref="validationLayers" /> are contained by the a list of available validation layers.
		/// </summary>
		/// <returns><c>true</c>, if all elements of <paramref cref="validationLayers" /> are contained by the a list of available validation layers.</returns>
		/// <seealso cref="getInstanceValidationLayers" />
		static bool validateInstanceLayers(const Span<const String> validationLayers) noexcept;

		/// <summary>
		/// Returns a list of available validation layers.
		/// </summary>
		/// <returns>A list of available validation layers.</returns>
		/// <seealso cref="validateInstanceLayers" />
		static Array<String> getInstanceValidationLayers() noexcept;

		// IBackend interface.
	public:
		/// <inheritdoc />
		virtual BackendType type() const noexcept override;

		// IRenderBackend interface.
	public:
		/// <inheritdoc />
		virtual Array<const VulkanGraphicsAdapter*> listAdapters() const override;

		/// <inheritdoc />
		virtual const VulkanGraphicsAdapter* findAdapter(const Optional<UInt32>& adapterId = std::nullopt) const override;
	};
}