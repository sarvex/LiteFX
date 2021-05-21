#pragma once

#if !defined (LITEFX_VULKAN_API)
#  if defined(LiteFX_Backends_Vulkan_EXPORTS) && (defined _WIN32 || defined WINCE)
#    define LITEFX_VULKAN_API __declspec(dllexport)
#  elif (defined(LiteFX_Backends_Vulkan_EXPORTS) || defined(__APPLE__)) && defined __GNUC__ && __GNUC__ >= 4
#    define LITEFX_VULKAN_API __attribute__ ((visibility ("default")))
#  elif !defined(LiteFX_Backends_Vulkan_EXPORTS) && (defined _WIN32 || defined WINCE)
#    define LITEFX_VULKAN_API __declspec(dllimport)
#  endif
#endif

#ifndef LITEFX_VULKAN_API
#  define LITEFX_VULKAN_API
#endif

#if (defined _WIN32 || defined WINCE)
#  define VK_USE_PLATFORM_WIN32_KHR
#else 
#  pragma message ("Vulkan: No supported surface platform detected.")
#endif

#include <vulkan/vulkan.h>

namespace LiteFX::Rendering::Backends {
    using namespace LiteFX::Math;
    using namespace LiteFX::Rendering;

    constexpr char VULKAN_LOG[] = "Backend::Vulkan";

    // API declarations.
    class VulkanTexture;
    class VulkanSwapChain;
    class VulkanQueue;
    class VulkanDevice;
    class VulkanBackend;
    class VulkanRenderPipeline;
    class VulkanRenderPipelineLayout;
    class VulkanRenderPass;
    class VulkanInputAttachmentMapping;
    class VulkanRasterizer;
    class VulkanInputAssembler;
    class VulkanShaderModule;
    class VulkanShaderProgram;
    class VulkanCommandBuffer;
    class VulkanDescriptorSetLayout;
    class VulkanVertexBufferLayout;
    class VulkanIndexBufferLayout;
    class VulkanDescriptorLayout;
    class VulkanVertexBuffer;
    class VulkanIndexBuffer;
    class VulkanConstantBuffer;
    class VulkanSampler;

    // Builder declarations.
    class VulkanInputAssemblerBuilder;
    class VulkanRenderPassBuilder;

    // Conversion helpers.
    /// <summary>
    /// 
    /// </summary>
    Format LITEFX_VULKAN_API getFormat(const VkFormat& format);

    /// <summary>
    /// 
    /// </summary>
    VkFormat LITEFX_VULKAN_API getFormat(const Format& format);

    /// <summary>
    /// 
    /// </summary>
    //BufferFormat LITEFX_VULKAN_API getFormat(const VkFormat& format);

    /// <summary>
    /// 
    /// </summary>
    VkFormat LITEFX_VULKAN_API getFormat(const BufferFormat& format);

    /// <summary>
    /// 
    /// </summary>
    PolygonMode LITEFX_VULKAN_API getPolygonMode(const VkPolygonMode& mode);

    /// <summary>
    /// 
    /// </summary>
    VkPolygonMode LITEFX_VULKAN_API getPolygonMode(const PolygonMode& mode);

    /// <summary>
    /// 
    /// </summary>
    CullMode LITEFX_VULKAN_API getCullMode(const VkCullModeFlags& mode);

    /// <summary>
    /// 
    /// </summary>
    VkCullModeFlags LITEFX_VULKAN_API getCullMode(const CullMode& mode);

    /// <summary>
    /// 
    /// </summary>
    PrimitiveTopology LITEFX_VULKAN_API getPrimitiveTopology(const VkPrimitiveTopology& topology);

    /// <summary>
    /// 
    /// </summary>
    VkPrimitiveTopology LITEFX_VULKAN_API getPrimitiveTopology(const PrimitiveTopology& topology);

    /// <summary>
    /// 
    /// </summary>
    ShaderStage LITEFX_VULKAN_API getShaderStage(const VkShaderStageFlagBits& shaderType);

    /// <summary>
    /// 
    /// </summary>
    VkShaderStageFlagBits LITEFX_VULKAN_API getShaderStage(const ShaderStage& shaderType);

    /// <summary>
    /// 
    /// </summary>
    MultiSamplingLevel LITEFX_VULKAN_API getSamples(const VkSampleCountFlagBits& samples);

    /// <summary>
    /// 
    /// </summary>
    VkSampleCountFlagBits LITEFX_VULKAN_API getSamples(const MultiSamplingLevel& samples);

    /// <summary>
    /// Represents a Vulkan graphics adapter.
    /// </summary>
    class LITEFX_VULKAN_API VulkanGraphicsAdapter : public IGraphicsAdapter, public IResource<VkPhysicalDevice> {
        LITEFX_IMPLEMENTATION(VulkanGraphicsAdapterImpl);

    public:
        /// <summary>
        /// Initializes a graphics adapter instance with a physical device.
        /// </summary>
        /// <param name="adapter">The phyiscal device to initialize the instance with.</param>
        explicit VulkanGraphicsAdapter(VkPhysicalDevice adapter);
        VulkanGraphicsAdapter(const VulkanGraphicsAdapter&) = delete;
        VulkanGraphicsAdapter(VulkanGraphicsAdapter&&) = delete;
        virtual ~VulkanGraphicsAdapter() noexcept;

    public:
        /// <inheritdoc />
        virtual String getName() const noexcept override;

        /// <inheritdoc />
        virtual UInt32 getVendorId() const noexcept override;

        /// <inheritdoc />
        virtual UInt32 getDeviceId() const noexcept override;

        /// <inheritdoc />
        virtual GraphicsAdapterType getType() const noexcept override;

        /// <inheritdoc />
        virtual UInt32 getDriverVersion() const noexcept override;

        /// <inheritdoc />
        virtual UInt32 getApiVersion() const noexcept override;

        /// <inheritdoc />
        virtual UInt64 getDedicatedMemory() const noexcept override;
    };

    /// <summary>
    /// Represents a Vulkan surface.
    /// </summary>
    class LITEFX_VULKAN_API VulkanSurface : public ISurface, public IResource<VkSurfaceKHR> {
        LITEFX_IMPLEMENTATION(VulkanSurfaceImpl)

    public:
        /// <summary>
        /// Initializes the surface from a surface and instance handle.
        /// </summary>
        /// <param name="surface">The handle of the Vulkan surface.</param>
        /// <param name="instance">The handle of the parent instance.</param>
        VulkanSurface(const VkSurfaceKHR& surface, const VkInstance& instance);
        VulkanSurface(const VulkanSurface&) = delete;
        VulkanSurface(VulkanSurface&&) = delete;
        virtual ~VulkanSurface() noexcept;

    public:
        /// <summary>
        /// Returns the handle of the backend, the surface has been created from.
        /// </summary>
        /// <returns>The handle of the backend, the surface has been created from.</returns>
        const VkInstance& instance() const noexcept;
    };

    /// <summary>
    /// 
    /// </summary>
    class LITEFX_VULKAN_API IVulkanImage : public virtual IImage, public virtual IResource<VkImage> {
    public:
        virtual ~IVulkanImage() noexcept = default;

    public:
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        virtual const VkImageView& getImageView() const noexcept = 0;
    };

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TParent"></typeparam>
    template <typename TParent>
    class LITEFX_VULKAN_API VulkanRuntimeObject {
    private:
        const TParent& m_parent;
        const VulkanDevice* m_device;

    public:
        explicit VulkanRuntimeObject(const TParent& parent, const VulkanDevice* device) :
            m_parent(parent), m_device(device) 
        {
            if (device == nullptr)
                throw RuntimeException("The device must be initialized.");
        }

        VulkanRuntimeObject(VulkanRuntimeObject&&) = delete;
        VulkanRuntimeObject(const VulkanRuntimeObject&) = delete;
        virtual ~VulkanRuntimeObject() noexcept = default;

    public:
        virtual const TParent& parent() const noexcept { return m_parent; }
        virtual const VulkanDevice* getDevice() const noexcept { return m_device; };
    };

    DEFINE_EXCEPTION(VulkanPlatformException, std::runtime_error);

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TException"></typeparam>
    /// <typeparam name="...TArgs"></typeparam>
    /// <param name="result"></param>
    /// <param name="message"></param>
    /// <param name="...args"></param>
    template <typename TException, typename ...TArgs>
    inline void raiseIfFailed(VkResult result, const std::string& message, TArgs&&... args) {
        if (result == VK_SUCCESS) [[likely]]
            return;

        throw TException(VulkanPlatformException("Result: {0}", result), fmt::format(message, std::forward<TArgs>(args)...));
    }
}