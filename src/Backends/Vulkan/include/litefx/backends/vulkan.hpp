#pragma once

#include <litefx/rendering.hpp>

#include "vulkan_api.hpp"
#include "vulkan_platform.hpp"
#include "vulkan_builders.hpp"
#include "vulkan_formatters.hpp"

namespace LiteFX::Rendering::Backends {
	using namespace LiteFX::Math;
	using namespace LiteFX::Rendering;

	class LITEFX_VULKAN_API VulkanRuntimeObject {
		LITEFX_IMPLEMENTATION(VulkanRuntimeObjectImpl);

	public:
		VulkanRuntimeObject(const VulkanDevice* device);
		VulkanRuntimeObject(VulkanRuntimeObject&&) = delete;
		VulkanRuntimeObject(const VulkanRuntimeObject&) = delete;
		virtual ~VulkanRuntimeObject() noexcept;

	public:
		virtual const VulkanDevice* getDevice() const noexcept;
	};

	class LITEFX_VULKAN_API VulkanBuffer : public Buffer, public IResource<VkBuffer> {
	public:
		VulkanBuffer(VkBuffer buffer, const IBufferLayout* layout, const UInt32& elements) :
			Buffer(layout, elements), IResource(buffer) { }
		VulkanBuffer(VulkanBuffer&&) = delete;
		VulkanBuffer(const VulkanBuffer&) = delete;
		virtual ~VulkanBuffer() noexcept = default;
	};

	class LITEFX_VULKAN_API VulkanBufferPool : public virtual VulkanRuntimeObject, public IBufferPool, public IResource<VkDescriptorPool> {
		LITEFX_IMPLEMENTATION(VulkanBufferPoolImpl);

	public:
		VulkanBufferPool(const VulkanDescriptorSetLayout& bufferSet);
		VulkanBufferPool(VulkanBufferPool&&) = delete;
		VulkanBufferPool(const VulkanBufferPool&) = delete;
		virtual ~VulkanBufferPool() noexcept;

	public:
		virtual const IDescriptorSetLayout* getDescriptorSetLayout() const noexcept override;
		virtual UniquePtr<IBuffer> makeBuffer(const UInt32& binding, const BufferUsage& usage, const UInt32& elements = 1) const noexcept override;
		virtual UniquePtr<ITexture> makeTexture(const UInt32& binding, const Format& format, const Size2d& size, const UInt32& levels = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1) const noexcept override;
		virtual void update(const IBuffer* buffer) const override;
		virtual void update(const ITexture* texture) const override;
		virtual void update(const UInt32& bindingPoint, const ISampler* sampler) const override;

	public:
		virtual const VkDescriptorSet getDescriptorSet() const noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanVertexBufferLayout : public virtual VulkanRuntimeObject, public IVertexBufferLayout {
		LITEFX_IMPLEMENTATION(VulkanVertexBufferLayoutImpl);
		LITEFX_BUILDER(VulkanVertexBufferLayoutBuilder);

	public:
		VulkanVertexBufferLayout(const VulkanInputAssembler& inputAssembler, const size_t& vertexSize, const UInt32& binding = 0);
		VulkanVertexBufferLayout(VulkanVertexBufferLayout&&) = delete;
		VulkanVertexBufferLayout(const VulkanVertexBufferLayout&) = delete;
		virtual ~VulkanVertexBufferLayout() noexcept;

	public:
		virtual size_t getElementSize() const noexcept override;
		virtual UInt32 getBinding() const noexcept override;
		virtual BufferType getType() const noexcept override;
	
	public:
		virtual Array<const BufferAttribute*> getAttributes() const noexcept override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanIndexBufferLayout : public virtual VulkanRuntimeObject, public IIndexBufferLayout {
		LITEFX_IMPLEMENTATION(VulkanIndexBufferLayoutImpl);
		LITEFX_BUILDER(VulkanIndexBufferLayoutBuilder);

	public:
		VulkanIndexBufferLayout(const VulkanInputAssembler& inputAssembler, const IndexType& type);
		VulkanIndexBufferLayout(VulkanIndexBufferLayout&&) = delete;
		VulkanIndexBufferLayout(const VulkanIndexBufferLayout&) = delete;
		virtual ~VulkanIndexBufferLayout() noexcept;

	public:
		virtual size_t getElementSize() const noexcept override;
		virtual UInt32 getBinding() const noexcept override;
		virtual BufferType getType() const noexcept override;

	public:
		virtual const IndexType& getIndexType() const noexcept override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanDescriptorLayout : public virtual VulkanRuntimeObject, public IDescriptorLayout {
		LITEFX_IMPLEMENTATION(VulkanDescriptorLayoutImpl);
		LITEFX_BUILDER(VulkanDescriptorLayoutBuilder);

	public:
		VulkanDescriptorLayout(const VulkanDescriptorSetLayout& descriptorSetLayout, const DescriptorType& type, const UInt32& binding, const size_t& elementSize);
		VulkanDescriptorLayout(VulkanDescriptorLayout&&) = delete;
		VulkanDescriptorLayout(const VulkanDescriptorLayout&) = delete;
		virtual ~VulkanDescriptorLayout() noexcept;

	public:
		virtual size_t getElementSize() const noexcept override;
		virtual UInt32 getBinding() const noexcept override;
		virtual BufferType getType() const noexcept override;

	public:
		virtual DescriptorType getDescriptorType() const noexcept override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanSwapChainImageLayout : public virtual VulkanRuntimeObject, public IDescriptorLayout {
	public:
		VulkanSwapChainImageLayout(const VulkanDevice& device);
		VulkanSwapChainImageLayout(VulkanSwapChainImageLayout&&) = delete;
		VulkanSwapChainImageLayout(const VulkanSwapChainImageLayout&) = delete;
		virtual ~VulkanSwapChainImageLayout() noexcept;

	public:
		virtual size_t getElementSize() const noexcept override;
		virtual UInt32 getBinding() const noexcept override;
		virtual BufferType getType() const noexcept override;

	public:
		virtual DescriptorType getDescriptorType() const noexcept override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanDescriptorSetLayout : public virtual VulkanRuntimeObject, public IDescriptorSetLayout, public IResource<VkDescriptorSetLayout> {
		LITEFX_IMPLEMENTATION(VulkanDescriptorSetLayoutImpl);
		LITEFX_BUILDER(VulkanDescriptorSetLayoutBuilder);

	public:
		VulkanDescriptorSetLayout(const VulkanShaderProgram& shaderProgram, const UInt32& id, const ShaderStage& stages);
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		virtual ~VulkanDescriptorSetLayout() noexcept;

	public:
		virtual Array<const IDescriptorLayout*> getLayouts() const noexcept override;
		virtual const IDescriptorLayout* getLayout(const UInt32& binding) const noexcept override;
		virtual const UInt32& getSetId() const noexcept override;
		virtual const ShaderStage& getShaderStages() const noexcept override;
		virtual UniquePtr<IBufferPool> createBufferPool() const noexcept override;

	public:
		virtual UInt32 uniforms() const noexcept override;
		virtual UInt32 storages() const noexcept override;
		virtual UInt32 images() const noexcept override;
		virtual UInt32 samplers() const noexcept override;
		virtual UInt32 inputAttachments() const noexcept override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanCommandBuffer : public ICommandBuffer, public IResource<VkCommandBuffer> {
		LITEFX_IMPLEMENTATION(VulkanCommandBufferImpl);

	public:
		VulkanCommandBuffer(const VulkanQueue* queue);
		VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer(VulkanCommandBuffer&&) = delete;
		virtual ~VulkanCommandBuffer() noexcept;

	public:
		virtual const ICommandQueue* getQueue() const noexcept override;

	public:
		virtual void begin() const override;
		virtual void end() const override;
		virtual void submit(const bool& waitForQueue = false) const override;
		virtual void submit(const Array<VkSemaphore>& waitForSemaphores, const Array<VkPipelineStageFlags>& waitForStages, const Array<VkSemaphore>& signalSemaphores = { }, const bool& waitForQueue = false) const;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanRenderPass : public virtual VulkanRuntimeObject, public IRenderPass, public IResource<VkRenderPass> {
		LITEFX_IMPLEMENTATION(VulkanRenderPassImpl);
		LITEFX_BUILDER(VulkanRenderPassBuilder);

	public:
		VulkanRenderPass(const VulkanRenderPipeline& pipeline);
		VulkanRenderPass(const VulkanRenderPass&) = delete;
		VulkanRenderPass(VulkanRenderPass&&) = delete;
		virtual ~VulkanRenderPass() noexcept;

	public:
		virtual const ICommandBuffer* getCommandBuffer() const noexcept override;

	public:
		virtual void addTarget(UniquePtr<IRenderTarget>&& target) override;
		virtual const Array<const IRenderTarget*> getTargets() const noexcept override;
		virtual UniquePtr<IRenderTarget> removeTarget(const IRenderTarget* target) override;
		virtual void begin() const override;
		virtual void end(const bool& present = false) override;
		virtual void reset() override;
		virtual void draw(const UInt32& vertices, const UInt32& instances = 1, const UInt32& firstVertex = 0, const UInt32& firstInstance = 0) const override;
		virtual void drawIndexed(const UInt32& indices, const UInt32& instances = 1, const UInt32& firstIndex = 0, const Int32& vertexOffset = 0, const UInt32& firstInstance = 0) const override;

	public:
		virtual void create();
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanInputAssembler : public virtual VulkanRuntimeObject, public InputAssembler {
		LITEFX_BUILDER(VulkanInputAssemblerBuilder);

	public:
		VulkanInputAssembler(const VulkanRenderPipelineLayout& layout) noexcept;
		VulkanInputAssembler(VulkanInputAssembler&&) noexcept = delete;
		VulkanInputAssembler(const VulkanInputAssembler&) noexcept = delete;
		virtual ~VulkanInputAssembler() noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanViewport : public virtual VulkanRuntimeObject, public Viewport {
		LITEFX_BUILDER(VulkanViewportBuilder);

	public:
		VulkanViewport(const VulkanRenderPipelineLayout& layout) noexcept;
		VulkanViewport(VulkanViewport&&) noexcept = delete;
		VulkanViewport(const VulkanViewport&) noexcept = delete;
		virtual ~VulkanViewport() noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanRasterizer : public virtual VulkanRuntimeObject, public Rasterizer {
		LITEFX_BUILDER(VulkanRasterizerBuilder);

	public:
		VulkanRasterizer(const VulkanRenderPipelineLayout& layout) noexcept;
		VulkanRasterizer(VulkanRasterizer&&) noexcept = delete;
		VulkanRasterizer(const VulkanRasterizer&) noexcept = delete;
		virtual ~VulkanRasterizer() noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanRenderPipelineLayout : public virtual VulkanRuntimeObject, public RenderPipelineLayout, public IResource<VkPipelineLayout> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineLayoutImpl);
		LITEFX_BUILDER(VulkanRenderPipelineLayoutBuilder);

	public:
		VulkanRenderPipelineLayout(const VulkanRenderPipeline& pipeline);
		VulkanRenderPipelineLayout(VulkanRenderPipelineLayout&&) noexcept = delete;
		VulkanRenderPipelineLayout(const VulkanRenderPipelineLayout&) noexcept = delete;
		virtual ~VulkanRenderPipelineLayout() noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanRenderPipeline : public virtual VulkanRuntimeObject, public RenderPipeline, public IResource<VkPipeline> {
		LITEFX_IMPLEMENTATION(VulkanRenderPipelineImpl);
		LITEFX_BUILDER(VulkanRenderPipelineBuilder);

	public:
		VulkanRenderPipeline(const IGraphicsDevice* device);	// Adapter for builder interface.
		VulkanRenderPipeline(const VulkanDevice* device);
		explicit VulkanRenderPipeline(UniquePtr<IRenderPipelineLayout>&& layout, const VulkanDevice* device);
		VulkanRenderPipeline(VulkanRenderPipeline&&) noexcept = delete;
		VulkanRenderPipeline(const VulkanRenderPipeline&) noexcept = delete;
		virtual ~VulkanRenderPipeline() noexcept;

	public:
		virtual void reset() override;
		virtual void bind(const IBuffer* buffer) const override;
		virtual void bind(const IBufferPool* buffer) const override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanShaderModule : public virtual VulkanRuntimeObject, public IShaderModule, public IResource<VkShaderModule> {
		LITEFX_IMPLEMENTATION(VulkanShaderModuleImpl);

	public:
		VulkanShaderModule(const VulkanDevice* device, const ShaderStage& type, const String& fileName, const String& entryPoint = "main");
		virtual ~VulkanShaderModule() noexcept;

	public:
		virtual const String& getFileName() const noexcept override;
		virtual const String& getEntryPoint() const noexcept override;
		virtual const ShaderStage& getType() const noexcept override;

	public:
		virtual VkPipelineShaderStageCreateInfo getShaderStageDefinition() const;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanShaderProgram : public virtual VulkanRuntimeObject, public IShaderProgram {
		LITEFX_IMPLEMENTATION(VulkanShaderProgramImpl);
		LITEFX_BUILDER(VulkanShaderProgramBuilder);

	public:
		VulkanShaderProgram(const VulkanRenderPipelineLayout& pipelineLayout);
		VulkanShaderProgram(VulkanShaderProgram&&) noexcept = delete;
		VulkanShaderProgram(const VulkanShaderProgram&) noexcept = delete;
		virtual ~VulkanShaderProgram() noexcept;

	public:
		virtual Array<const IShaderModule*> getModules() const noexcept override;
		virtual Array<const IDescriptorSetLayout*> getLayouts() const noexcept override;
		virtual void use(UniquePtr<IShaderModule>&& module) override;
		virtual void use(UniquePtr<IDescriptorSetLayout>&& layout) override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanTexture : public VulkanRuntimeObject, public virtual ITexture, public Buffer, public IResource<VkImage> {
		LITEFX_IMPLEMENTATION(VulkanTextureImpl);

	public:
		VulkanTexture(const VulkanDevice* device, const IBufferLayout* layout, VkImage image, const Format& format, const Size2d& size, const UInt32& levels = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1);
		VulkanTexture(VulkanTexture&&) = delete;
		VulkanTexture(const VulkanTexture&) = delete;
		virtual ~VulkanTexture() noexcept;

	public:
		virtual UInt32 getSize() const noexcept override;
		virtual Size2d getExtent() const noexcept override;
		virtual Format getFormat() const noexcept override;
		virtual MultiSamplingLevel getSamples() const noexcept override;
		virtual const UInt32& getLevels() const noexcept override;
		virtual void map(const void* const data, const size_t& size) override;
		virtual void transfer(const ICommandQueue* commandQueue, IBuffer* target, const size_t& size, const size_t& offset = 0, const size_t& targetOffset = 0) const override;

	public:
		virtual VkImageView getView() const noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanSampler : public VulkanRuntimeObject, public virtual Sampler, public IResource<VkSampler> {
		LITEFX_IMPLEMENTATION(VulkanSamplerImpl);

	public:
		VulkanSampler(
			const VulkanDevice* device,
			const IDescriptorLayout* layout,
			const FilterMode& magFilter = FilterMode::Nearest, 
			const FilterMode& minFilter = FilterMode::Nearest, 
			const BorderMode& borderU = BorderMode::Repeat, 
			const BorderMode& borderV = BorderMode::Repeat, 
			const BorderMode& borderW = BorderMode::Repeat, 
			const MipMapMode& mipMapMode = MipMapMode::Nearest, 
			const Float& mipMapBias = 0.f, 
			const Float& maxLod = std::numeric_limits<Float>::max(), 
			const Float& minLod = 0.f, 
			const Float& anisotropy = 0.f);
		VulkanSampler(VulkanSampler&&) = delete;
		VulkanSampler(const VulkanSampler&) = delete;
		virtual ~VulkanSampler() noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanSwapChain : public virtual VulkanRuntimeObject, public ISwapChain, public IResource<VkSwapchainKHR> {
		LITEFX_IMPLEMENTATION(VulkanSwapChainImpl);

	public:
		VulkanSwapChain(const VulkanDevice* device, const Format& format = Format::B8G8R8A8_SRGB);
		virtual ~VulkanSwapChain() noexcept;

	public:
		virtual const Size2d& getBufferSize() const noexcept override;
		virtual size_t getWidth() const noexcept override;
		virtual size_t getHeight() const noexcept override;
		virtual const Format& getFormat() const noexcept override;
		virtual Array<const ITexture*> getFrames() const noexcept override;
		virtual UInt32 swapBackBuffer() const override;
		virtual void reset() override;

	public:
		virtual VkSemaphore getSemaphore() const noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanQueue : public ICommandQueue, public IResource<VkQueue> {
		LITEFX_IMPLEMENTATION(VulkanQueueImpl);
	
	public:
		VulkanQueue(const QueueType& type, const uint32_t id);
		virtual ~VulkanQueue() noexcept;

	public:
		virtual VkCommandPool getCommandPool() const noexcept;

	public:
		virtual UInt32 getId() const noexcept override;
		virtual QueueType getType() const noexcept override;
		virtual const IGraphicsDevice* getDevice() const noexcept override;

	public:
		virtual void bindDevice(const IGraphicsDevice* device) override;
		virtual void release() override;
		virtual UniquePtr<ICommandBuffer> createCommandBuffer() const override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanDevice : public GraphicsDevice, public IResource<VkDevice> {
		LITEFX_IMPLEMENTATION(VulkanDeviceImpl);

	public:
		explicit VulkanDevice(const IRenderBackend* backend, const Format& format, const Array<String>& extensions = { });
		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&&) = delete;
		virtual ~VulkanDevice() noexcept;

	public:
		virtual size_t getBufferWidth() const noexcept override;
		virtual size_t getBufferHeight() const noexcept override;
		virtual void wait() override;
		virtual void resize(int width, int height) override;
		virtual UniquePtr<IBuffer> createBuffer(const IBufferLayout* layout, const BufferUsage& usage, const UInt32& elements) const override;
		virtual UniquePtr<ITexture> createTexture(const IBufferLayout* layout, const Format& format, const Size2d& size, const UInt32& levels = 1, const MultiSamplingLevel& samples = MultiSamplingLevel::x1) const override;
		virtual UniquePtr<IShaderModule> loadShaderModule(const ShaderStage& type, const String& fileName, const String& entryPoint = "main") const override;
		virtual Array<UniquePtr<ITexture>> createSwapChainImages(const ISwapChain* swapChain) const override;

	public:
		virtual const Array<String>& getExtensions() const noexcept;
		virtual Array<Format> getSurfaceFormats() const override;
		virtual const ISwapChain* getSwapChain() const noexcept override;

	public:
		virtual bool validateDeviceExtensions(const Array<String>& extensions) const noexcept;
		virtual Array<String> getAvailableDeviceExtensions() const noexcept;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanGraphicsAdapter : public IGraphicsAdapter, public IResource<VkPhysicalDevice> {
		LITEFX_IMPLEMENTATION(VulkanGraphicsAdapterImpl);

	public:
		VulkanGraphicsAdapter(VkPhysicalDevice adapter);
		VulkanGraphicsAdapter(const VulkanGraphicsAdapter&) = delete;
		VulkanGraphicsAdapter(VulkanGraphicsAdapter&&) = delete;
		virtual ~VulkanGraphicsAdapter() noexcept;

	public:
		virtual String getName() const noexcept override;
		virtual uint32_t getVendorId() const noexcept override;
		virtual uint32_t getDeviceId() const noexcept override;
		virtual GraphicsAdapterType getType() const noexcept override;
		virtual uint32_t getDriverVersion() const noexcept override;
		virtual uint32_t getApiVersion() const noexcept override;

	public:
		virtual ICommandQueue* findQueue(const QueueType& queueType) const override;
		virtual ICommandQueue* findQueue(const QueueType& queueType, const ISurface* forSurface) const override;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_VULKAN_API VulkanBackend : public RenderBackend, public IResource<VkInstance> {
		LITEFX_IMPLEMENTATION(VulkanBackendImpl);
		LITEFX_BUILDER(VulkanBackendBuilder);

	public:
		explicit VulkanBackend(const App& app, const Array<String>& extensions = { }, const Array<String>& validationLayers = { });
		VulkanBackend(const VulkanBackend&) noexcept = delete;
		VulkanBackend(VulkanBackend&&) noexcept = delete;
		virtual ~VulkanBackend();

	public:
		virtual Array<const IGraphicsAdapter*> listAdapters() const override;
		virtual const IGraphicsAdapter* findAdapter(const Optional<uint32_t>& adapterId = std::nullopt) const override;
		virtual const ISurface* getSurface() const noexcept override;
		virtual const IGraphicsAdapter* getAdapter() const noexcept override;
		virtual const Array<String>& getEnabledValidationLayers() const noexcept;

	public:
		virtual void use(const IGraphicsAdapter* adapter) override;
		virtual void use(UniquePtr<ISurface>&& surface) override;

	public:
		static bool validateExtensions(const Array<String>& extensions) noexcept;
		static Array<String> getAvailableExtensions() noexcept;
		static bool validateLayers(const Array<String>& validationLayers) noexcept;
		static Array<String> getValidationLayers() noexcept;
	};

}