#pragma once

#include <litefx/app.hpp>
#include <litefx/math.hpp>
#include <litefx/rendering_api.hpp>
#include <litefx/rendering_formatters.hpp>
#include <litefx/rendering_pipelines.hpp>

namespace LiteFX::Rendering {
	using namespace LiteFX;
	using namespace LiteFX::Math;

	// Define interfaces.
	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API ITexture {
	public:
		virtual ~ITexture() noexcept = default;

	public:
		virtual Size2d getSize() const noexcept = 0;
		virtual Format getFormat() const noexcept = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API ISwapChain {
	public:
		virtual ~ISwapChain() noexcept = default;

	public:
		virtual const IGraphicsDevice* getDevice() const noexcept = 0;
		virtual const Size2d& getBufferSize() const noexcept = 0;
		virtual size_t getWidth() const noexcept = 0;
		virtual size_t getHeight() const noexcept = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API ISurface {
	public:
		virtual ~ISurface() noexcept = default;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API ICommandPool {
	public:
		virtual ~ICommandPool() noexcept = default;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API ICommandQueue {
	public:
		virtual ~ICommandQueue() noexcept = default;

	public:
		virtual QueueType getType() const noexcept = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API IGraphicsDevice {
	public:
		virtual ~IGraphicsDevice() noexcept = default;

	public:
		virtual const IGraphicsAdapter* getAdapter() const noexcept= 0;
		virtual const ISurface* getSurface() const noexcept = 0;
		virtual const ISwapChain* getSwapChain() const noexcept = 0;
		virtual Array<Format> getSurfaceFormats() const = 0;

	public:
		virtual size_t getBufferWidth() const noexcept = 0;
		virtual size_t getBufferHeight() const noexcept = 0;

	public:
		//virtual UniquePtr<ITexture> createTexture2d(const Format& format = Format::B8G8R8A8_UNORM_SRGB, const Size2d& size = Size2d(0)) const = 0;
		virtual UniquePtr<IShaderModule> loadShaderModule(const ShaderType& type, const String& fileName, const String& entryPoint = "main") const = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API IGraphicsAdapter {
	public:
		virtual ~IGraphicsAdapter() noexcept = default;

	public:
		virtual String getName() const noexcept = 0;
		virtual uint32_t getVendorId() const noexcept = 0;
		virtual uint32_t getDeviceId() const noexcept = 0;
		virtual GraphicsAdapterType getType() const noexcept = 0;
		virtual uint32_t getDriverVersion() const noexcept = 0;
		virtual uint32_t getApiVersion() const noexcept = 0;

	public:
		virtual UniquePtr<IGraphicsDevice> createDevice(const ISurface* surface, const Format& format = Format::B8G8R8A8_UNORM_SRGB, const Array<String>& extensions = { }) const = 0;
		virtual ICommandQueue* findQueue(const QueueType& queueType) const = 0;
		virtual ICommandQueue* findQueue(const QueueType& queueType, const ISurface* forSurface) const = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API GraphicsDevice : public IGraphicsDevice {
		LITEFX_IMPLEMENTATION(GraphicsDeviceImpl)

	public:
		GraphicsDevice(const IGraphicsAdapter* adapter, const ISurface* surface);
		GraphicsDevice(const GraphicsDevice&) noexcept = delete;
		GraphicsDevice(GraphicsDevice&&) noexcept = delete;
		virtual ~GraphicsDevice() noexcept;

	public:
		virtual const IGraphicsAdapter* getAdapter() const noexcept override;
		virtual const ISurface* getSurface() const noexcept override;

	public:
		/// <summary>
		/// Initializes a new render pipeline of type <typeparamref name="T"/> and returns a builder instance for it.
		/// </summary>
		/// <param name="_args">The arguments which are passed to the constructor of the render pipeline.</param>
		/// <typeparam name="T">The type of the render pipeline. The type must implement <see cref="IRenderPipeline" /> interface.</typeparam>
		template <typename T, typename ...TArgs, std::enable_if_t<std::is_convertible_v<T*, IRenderPipeline*>, int> = 0, typename TBuilder = T::builder>
		TBuilder build(TArgs&&... _args) const {
			return TBuilder(makeUnique<T>(this, std::forward<TArgs>(_args)...));
		}
	};

	/// <summary>
	/// 
	/// </summary>
	template <typename TDerived, typename TDevice>
	class GraphicsDeviceBuilder : public Builder<TDerived, TDevice> {
	public:
		using builder_type::Builder;

	public:
		virtual TDerived& withFormat(const Format& format) = 0;
		virtual TDerived& withQueue(const QueueType& queueType) = 0;
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API IRenderBackend : public IBackend {
	public:
		virtual ~IRenderBackend() noexcept = default;

	public:
		virtual Array<const IGraphicsAdapter*> listAdapters() const = 0;
		virtual const IGraphicsAdapter* findAdapter(const Optional<uint32_t>& adapterId = std::nullopt) const = 0;
		virtual const ISurface* getSurface() const noexcept = 0;
		virtual const IGraphicsAdapter* getAdapter() const noexcept = 0;

	public:
		virtual void use(const IGraphicsAdapter* adapter) = 0;
		virtual void use(UniquePtr<ISurface>&& surface) = 0;

	public:
		template <typename T, typename ...TArgs, std::enable_if_t<std::is_convertible_v<T*, IGraphicsDevice*>, int> = 0, typename TBuilder = T::builder>
		TBuilder build(TArgs&&... _args) const {
			return TBuilder(makeUnique<T>(this->getAdapter(), this->getSurface(), std::forward<TArgs>(_args)...));
		}
	};

	/// <summary>
	/// 
	/// </summary>
	class LITEFX_RENDERING_API RenderBackend : public IRenderBackend {
		LITEFX_IMPLEMENTATION(RenderBackendImpl)

	public:
		explicit RenderBackend(const App& app) noexcept;
		RenderBackend(const RenderBackend&) noexcept = delete;
		RenderBackend(RenderBackend&&) noexcept = delete;
		virtual ~RenderBackend() noexcept;

	public:
		virtual BackendType getType() const noexcept override;
		const App& getApp() const noexcept;
	};
}