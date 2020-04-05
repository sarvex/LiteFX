#include <litefx/backends/vulkan.hpp>

using namespace LiteFX::Rendering::Backends;

class VulkanBackend::VulkanBackendImpl {
private:
    Array<String> m_extensions;
    Array<String> m_layers;

public:
    VulkanBackendImpl(const Array<String>& extensions, const Array<String>& validationLayers) noexcept :
        m_extensions(extensions), m_layers(validationLayers) { }

public:
    void initialize(VulkanBackend& parent)
    {
        // Check, if already initialized.
        if (parent.handle() != nullptr)
            throw std::runtime_error("The backend is already initialized.");

        // Parse the extensions.
        std::vector<const char*> requiredExtensions(m_extensions.size()), enabledLayers(m_layers.size());
        std::generate(requiredExtensions.begin(), requiredExtensions.end(), [this, i = 0]() mutable { return m_extensions[i++].data(); });
        std::generate(enabledLayers.begin(), enabledLayers.end(), [this, i = 0]() mutable { return m_layers[i++].data(); });

        // Check if all extensions are available.
        if (!VulkanBackend::validateExtensions(m_extensions))
            throw std::runtime_error("Some required Vulkan extensions are not supported by the system.");

        // Check if all extensions are available.
        if (!VulkanBackend::validateLayers(m_layers))
            throw std::runtime_error("Some required Vulkan layers are not supported by the system.");

        // Get the app instance.
        auto& app = parent.getApp();
        auto appName = app.getName();

        // Define Vulkan app.
        VkApplicationInfo appInfo = {};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(app.getVersion().getMajor(), app.getVersion().getMinor(), app.getVersion().getPatch());
        appInfo.pEngineName = LITEFX_ENGINE_ID;
        appInfo.engineVersion = VK_MAKE_VERSION(LITEFX_MAJOR, LITEFX_MINOR, LITEFX_REV);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        // Create Vulkan instance.
        VkInstanceCreateInfo createInfo = {};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_layers.size());
        createInfo.ppEnabledLayerNames = m_layers.size() == 0 ? nullptr : enabledLayers.data();

        if (::vkCreateInstance(&createInfo, nullptr, &parent.handle()) != VK_SUCCESS)
            throw std::runtime_error("Unable to create Vulkan instance.");
    }

public:
    const Array<String>& getExtensions() const
    {
        return m_extensions;
    }

    const Array<String>& getLayers() const
    {
        return m_layers;
    }

    Array<UniquePtr<IGraphicsAdapter>> getAdapters(const VulkanBackend& parent) const
    {
        if (parent.handle() == nullptr)
            throw std::runtime_error("The backend is not initialized.");

        uint32_t adapters = 0;
        ::vkEnumeratePhysicalDevices(parent.handle(), &adapters, nullptr);

        Array<VkPhysicalDevice> handles(adapters);
        Array<UniquePtr<IGraphicsAdapter>> results(adapters);

        ::vkEnumeratePhysicalDevices(parent.handle(), &adapters, handles.data());
        std::generate(results.begin(), results.end(), [this, &handles, i = 0]() mutable {
            return makeUnique<VulkanGraphicsAdapter>(handles[i++]);
        });

        return results;
    }

    UniquePtr<IGraphicsAdapter> getAdapter(const VulkanBackend& parent, Optional<uint32_t> adapterId) const
    {
        if (parent.handle() == nullptr)
            throw std::runtime_error("The backend is not initialized.");

        uint32_t adapters = 0;
        ::vkEnumeratePhysicalDevices(parent.handle(), &adapters, nullptr);

        Array<VkPhysicalDevice> handles(adapters);
        ::vkEnumeratePhysicalDevices(parent.handle(), &adapters, handles.data());

        auto match = std::find_if(handles.begin(), handles.end(), [&adapterId](const VkPhysicalDevice& dvc) {
            return !adapterId.has_value() || makeUnique<VulkanGraphicsAdapter>(dvc)->getDeviceId() == adapterId;
        });

        if (match == handles.end())
            throw std::runtime_error("The requested graphics adapter could not be found.");

        return makeUnique<VulkanGraphicsAdapter>(*match);
    }
};

VulkanBackend::VulkanBackend(const App& app, const Array<String>& extensions, const Array<String>& validationLayers) :
    RenderBackend(app), IResource(nullptr), m_impl(makePimpl<VulkanBackendImpl>(extensions, validationLayers))
{
    m_impl->initialize(*this);
}

VulkanBackend::~VulkanBackend() noexcept 
{
    ::vkDestroyInstance(this->handle(), nullptr);
}

Array<UniquePtr<IGraphicsAdapter>> VulkanBackend::getAdapters() const
{
    return m_impl->getAdapters(*this);
}

UniquePtr<IGraphicsAdapter> VulkanBackend::getAdapter(Optional<uint32_t> adapterId) const
{
    return m_impl->getAdapter(*this, adapterId);
}

// ------------------------------------------------------------------------------------------------
// Platform specific code.
// ------------------------------------------------------------------------------------------------

UniquePtr<ISurface> VulkanBackend::createSurfaceWin32(HWND hwnd) const
{
    if (this->handle() == nullptr)
        throw std::runtime_error("The backend is not initialized.");

    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = hwnd;
    createInfo.hinstance = ::GetModuleHandle(nullptr);

    VkSurfaceKHR surface;

    if (::vkCreateWin32SurfaceKHR(this->handle(), &createInfo, nullptr, &surface) != VK_SUCCESS)
        throw std::runtime_error("Unable to create vulkan surface for provided window.");

    return makeUnique<VulkanSurface>(surface, this->handle());
}

// ------------------------------------------------------------------------------------------------
// Static interface.
// ------------------------------------------------------------------------------------------------

bool VulkanBackend::validateExtensions(const Array<String>& extensions) noexcept
{
    auto availableExtensions = VulkanBackend::getAvailableExtensions();

    return std::all_of(extensions.begin(), extensions.end(), [&availableExtensions](const String& extension) {
        return std::find_if(availableExtensions.begin(), availableExtensions.end(), [&extension](String& str) {
            return std::equal(str.begin(), str.end(), extension.begin(), extension.end(), [](char a, char b) {
                return std::tolower(a) == std::tolower(b);
            });
        }) != availableExtensions.end();
    });
}

Array<String> VulkanBackend::getAvailableExtensions() noexcept
{
    uint32_t extensions = 0;
    ::vkEnumerateInstanceExtensionProperties(nullptr, &extensions, nullptr);

    Array<VkExtensionProperties> availableExtensions(extensions);
    Array<String> extensionNames(extensions);

    ::vkEnumerateInstanceExtensionProperties(nullptr, &extensions, availableExtensions.data());
    std::generate(extensionNames.begin(), extensionNames.end(), [&availableExtensions, i = 0]() mutable { return availableExtensions[i++].extensionName; });

    return extensionNames;
}

bool VulkanBackend::validateLayers(const Array<String>& validationLayers) noexcept
{
    auto layers = VulkanBackend::getValidationLayers();

    return std::all_of(validationLayers.begin(), validationLayers.end(), [&layers](const String& layer) {
        return std::find_if(layers.begin(), layers.end(), [&layer](String& str) {
            return std::equal(str.begin(), str.end(), layer.begin(), layer.end(), [](char a, char b) {
                return std::tolower(a) == std::tolower(b);
            });
        }) != layers.end();
    });
}

Array<String> VulkanBackend::getValidationLayers() noexcept
{
    uint32_t layers = 0;
    ::vkEnumerateInstanceLayerProperties(&layers, nullptr);

    Array<VkLayerProperties> availableLayers(layers);
    Array<String> layerNames(layers);

    ::vkEnumerateInstanceLayerProperties(&layers, availableLayers.data());
    std::generate(layerNames.begin(), layerNames.end(), [&availableLayers, i = 0]() mutable { return availableLayers[i++].layerName; });

    return layerNames;
}