#pragma once

#if !defined (LITEFX_DIRECTX12_API)
#  if defined(LiteFX_Backends_DirectX12_EXPORTS) && (defined _WIN32 || defined WINCE)
#    define LITEFX_DIRECTX12_API __declspec(dllexport)
#  elif (defined(LiteFX_Backends_DirectX12_EXPORTS) || defined(__APPLE__)) && defined __GNUC__ && __GNUC__ >= 4
#    define LITEFX_DIRECTX12_API __attribute__ ((visibility ("default")))
#  elif !defined(LiteFX_Backends_DirectX12_EXPORTS) && (defined _WIN32 || defined WINCE)
#    define LITEFX_DIRECTX12_API __declspec(dllimport)
#  endif
#endif

#ifndef LITEFX_DIRECTX12_API
#  define LITEFX_DIRECTX12_API
#endif

#if (!defined _WIN32 && !defined WINCE)
#  pragma message ("DirectX12: The DirectX backend requires Windows to be built on.")
#endif

#ifdef LITEFX_DEFINE_GLOBAL_EXPORTS
#include <d3d12agility.hpp>
#endif // LITEFX_DEFINE_GLOBAL_EXPORTS

#include <directx/d3d12.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>
#include <dxguids/dxguids.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <comdef.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <litefx/config.h>
#include <litefx/rendering.hpp>

namespace LiteFX::Rendering::Backends {
    using namespace LiteFX::Math;
    using namespace LiteFX::Rendering;

    constexpr char DIRECTX12_LOG[] = "Backend::DirectX12";

    // Forward declarations.
    class DirectX12VertexBufferLayout;
    class DirectX12IndexBufferLayout;
    class DirectX12DescriptorLayout;
    class DirectX12DescriptorSetLayout;
    class DirectX12DescriptorSet;
    class DirectX12PushConstantsRange;
    class DirectX12PushConstantsLayout;
    class DirectX12PipelineLayout;
    class DirectX12ShaderModule;
    class DirectX12ShaderProgram;
    class DirectX12CommandBuffer;
    class DirectX12Barrier;
    class DirectX12InputAssembler;
    class DirectX12Rasterizer;
    class DirectX12PipelineState;
    class DirectX12RenderPipeline;
    class DirectX12ComputePipeline;
    class DirectX12FrameBuffer;
    class DirectX12RenderPass;
    class DirectX12InputAttachmentMapping;
    class DirectX12SwapChain;
    class DirectX12Queue;
    class DirectX12GraphicsFactory;
    class DirectX12Device;
    class DirectX12Backend;

    // Interface declarations.
    class IDirectX12Buffer;
    class IDirectX12VertexBuffer;
    class IDirectX12IndexBuffer;
    class IDirectX12Image;
    class IDirectX12Sampler;

#if defined(BUILD_DEFINE_BUILDERS)
    // Builder declarations.
    class DirectX12VertexBufferLayoutBuilder;
    class DirectX12DescriptorSetLayoutBuilder;
    class DirectX12PushConstantsLayoutBuilder;
    class DirectX12PipelineLayoutBuilder;
    class DirectX12ShaderProgramBuilder;
    class DirectX12InputAssemblerBuilder;
    class DirectX12RasterizerBuilder;
    class DirectX12RenderPipelineBuilder;
    class DirectX12ComputePipelineBuilder;
    class DirectX12RenderPassBuilder;
    class DirectX12BarrierBuilder;
#endif // defined(BUILD_DEFINE_BUILDERS)

    /// <summary>
    /// A resource that is hold by a <c>ComPtr</c>.
    /// </summary>
    /// <typeparam name="THandle">The type of the resource interface.</typeparam>
    template <class THandle>
    using ComResource = Resource<ComPtr<THandle>>;
    // TODO: We could overwrite the handle() methods here and return `.AsWeak` from the ComPtr and create an overload to get a reference-counted handle, if required.

    /// <summary>
    /// Contains conversion helpers for DirectX 12.
    /// </summary>
    namespace DX12
    {
        /// <summary>
        /// 
        /// </summary>
        constexpr inline Format LITEFX_DIRECTX12_API getFormat(const DXGI_FORMAT& format);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline DXGI_FORMAT LITEFX_DIRECTX12_API getFormat(Format format);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline DXGI_FORMAT LITEFX_DIRECTX12_API getFormat(BufferFormat format);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline bool LITEFX_DIRECTX12_API isSRGB(Format format);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_RESOURCE_DIMENSION LITEFX_DIRECTX12_API getImageType(ImageDimensions dimensions);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline PolygonMode LITEFX_DIRECTX12_API getPolygonMode(const D3D12_FILL_MODE& mode);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_FILL_MODE LITEFX_DIRECTX12_API getPolygonMode(PolygonMode mode);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline CullMode LITEFX_DIRECTX12_API getCullMode(const D3D12_CULL_MODE& mode);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_CULL_MODE LITEFX_DIRECTX12_API getCullMode(CullMode mode);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline PrimitiveTopology LITEFX_DIRECTX12_API getPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY& topology);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_PRIMITIVE_TOPOLOGY LITEFX_DIRECTX12_API getPrimitiveTopology(PrimitiveTopology topology);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_PRIMITIVE_TOPOLOGY_TYPE LITEFX_DIRECTX12_API getPrimitiveTopologyType(PrimitiveTopology topology);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline LPCTSTR LITEFX_DIRECTX12_API getSemanticName(AttributeSemantic semantic);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="vendorId"></param>
        /// <returns></returns>
        constexpr inline String LITEFX_DIRECTX12_API getVendorName(UInt32 vendorId);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_COMPARISON_FUNC LITEFX_DIRECTX12_API getCompareOp(CompareOperation compareOp);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_STENCIL_OP LITEFX_DIRECTX12_API getStencilOp(StencilOperation stencilOp);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_BLEND LITEFX_DIRECTX12_API getBlendFactor(BlendFactor blendFactor);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_BLEND_OP LITEFX_DIRECTX12_API getBlendOperation(BlendOperation blendOperation);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_BARRIER_SYNC LITEFX_DIRECTX12_API getPipelineStage(PipelineStage pipelineStage);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_BARRIER_ACCESS LITEFX_DIRECTX12_API getResourceAccess(ResourceAccess resourceAccess);

        /// <summary>
        /// 
        /// </summary>
        constexpr inline D3D12_BARRIER_LAYOUT LITEFX_DIRECTX12_API getImageLayout(ImageLayout imageLayout);
    }

    /// <summary>
    /// Implements a DirectX12 <see cref="IGraphicsAdapter" />.
    /// </summary>
    class LITEFX_DIRECTX12_API DirectX12GraphicsAdapter final : public IGraphicsAdapter, public ComResource<IDXGIAdapter4> {
        LITEFX_IMPLEMENTATION(DirectX12GraphicsAdapterImpl);

    public:
        /// <summary>
        /// Initializes a new DirectX12 graphics adapter.
        /// </summary>
        /// <param name="adapter">The DXGI adapter interface pointer.</param>
        explicit DirectX12GraphicsAdapter(ComPtr<IDXGIAdapter4> adapter);
        DirectX12GraphicsAdapter(const DirectX12GraphicsAdapter&) = delete;
        DirectX12GraphicsAdapter(DirectX12GraphicsAdapter&&) = delete;
        virtual ~DirectX12GraphicsAdapter() noexcept;

    public:
        /// <inheritdoc />
        String name() const noexcept override;

        /// <inheritdoc />
        UInt64 uniqueId() const noexcept override;

        /// <inheritdoc />
        UInt32 vendorId() const noexcept override;

        /// <inheritdoc />
        UInt32 deviceId() const noexcept override;

        /// <inheritdoc />
        GraphicsAdapterType type() const noexcept override;

        /// <inheritdoc />
        /// <remarks>
        /// This property is not supported by DirectX 12. The method always returns `0`.
        /// </remarks>
        UInt32 driverVersion() const noexcept override;

        /// <inheritdoc />
        /// <remarks>
        /// This property is not supported by DirectX 12. The method always returns `0`.
        /// </remarks>
        UInt32 apiVersion() const noexcept override;

        /// <inheritdoc />
        UInt64 dedicatedMemory() const noexcept override;
    };

    /// <summary>
    /// Implements a DirectX12 <see cref="ISurface" />.
    /// </summary>
    class LITEFX_DIRECTX12_API DirectX12Surface final : public ISurface, public Resource<HWND> {
    public:
        /// <summary>
        /// Initializes a new DirectX 12 surface.
        /// </summary>
        /// <param name="hwnd">The window handle.</param>
        explicit DirectX12Surface(const HWND& hwnd) noexcept;
        DirectX12Surface(const DirectX12Surface&) = delete;
        DirectX12Surface(DirectX12Surface&&) = delete;
        virtual ~DirectX12Surface() noexcept;
    };

    DEFINE_EXCEPTION(DX12PlatformException, std::runtime_error);

    template <typename TException, typename ...TArgs>
    inline void raiseIfFailed(HRESULT hr, StringView message, TArgs&&... args) {
        if (SUCCEEDED(hr)) [[likely]]
            return;

        _com_error error(hr);

        if (message.empty())
            throw TException(DX12PlatformException("{1} (HRESULT 0x{0:08X})", static_cast<unsigned>(hr), error.ErrorMessage()));
        else
            throw TException(DX12PlatformException("{1} (HRESULT 0x{0:08X})", static_cast<unsigned>(hr), error.ErrorMessage()), fmt::format(fmt::runtime(message), std::forward<TArgs>(args)...));
    }
}