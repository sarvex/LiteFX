#include <litefx/rendering.hpp>

using namespace LiteFX::Rendering;

// ------------------------------------------------------------------------------------------------
// Implementation.
// ------------------------------------------------------------------------------------------------

class RenderPipeline::RenderPipelineImpl {
private:
    UniquePtr<IRenderPipelineLayout> m_layout;
    const IGraphicsDevice* m_device;

public:
    RenderPipelineImpl(UniquePtr<IRenderPipelineLayout>&& layout, const IGraphicsDevice* device) noexcept :
        m_layout(std::move(layout)), m_device(device) { }

    RenderPipelineImpl(const IGraphicsDevice* device) noexcept :
        m_layout(makeUnique<RenderPipelineLayout>()), m_device(device) { }

public:
    const IGraphicsDevice* getDevice() const noexcept
    {
        return m_device;
    }

    const IRenderPipelineLayout* getLayout() const noexcept 
    {
        return m_layout.get();
    }

    void setLayout(UniquePtr<IRenderPipelineLayout>&& layout)
    {
        m_layout = std::move(layout);
    }
};

// ------------------------------------------------------------------------------------------------
// Interface.
// ------------------------------------------------------------------------------------------------

RenderPipeline::RenderPipeline(const IGraphicsDevice* device) :
    m_impl(makePimpl<RenderPipelineImpl>(device))
{
    if (device == nullptr)
        throw std::invalid_argument("The graphics device must be initialized.");
}

RenderPipeline::RenderPipeline(const IGraphicsDevice* device, UniquePtr<IRenderPipelineLayout>&& layout) :
    m_impl(makePimpl<RenderPipelineImpl>(std::move(layout), device))
{
    if (device == nullptr)
        throw std::invalid_argument("The graphics device must be initialized.");
}

RenderPipeline::~RenderPipeline() noexcept = default;

const IGraphicsDevice* RenderPipeline::getDevice() const noexcept
{
    return m_impl->getDevice();
}

const IRenderPipelineLayout* RenderPipeline::getLayout() const noexcept
{
    return m_impl->getLayout();
}

void RenderPipeline::use(UniquePtr<IRenderPipelineLayout>&& layout)
{
    m_impl->setLayout(std::move(layout));
}