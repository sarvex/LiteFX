#include "sample.h"
#include <glm/gtc/matrix_transform.hpp>

#define LIGHT_SOURCES 8

enum DescriptorSets : UInt32
{
    Constant = 0,                                       // All buffers that are immutable.
    PerFrame = 1,                                       // All buffers that are updated each frame.
};

enum Pipelines : UInt32
{
    Basic = 0                                           // Default render pipeline.
};

const Array<Vertex> vertices =
{
    { { -0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 0.33f, 0.33f, 0.33f, 1.0f }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f } }
};

const Array<UInt16> indices = { 
    0, 1, 2, 1, 3, 2,       // Front
    4, 6, 5, 5, 6, 7,       // Back
    8, 9, 10, 9, 11, 10,    // Right
    12, 14, 13, 13, 14, 15, // Left
    16, 17, 18, 17, 19, 18, // Bottom
    20, 22, 21, 21, 22, 23  // Top
};

struct CameraBuffer {
    glm::mat4 ViewProjection;
    glm::vec4 Position;
} camera;

struct TransformBuffer {
    glm::mat4 World;
} transform;

struct LightBuffer {
    glm::vec4 Position;
    glm::vec4 Color;
    glm::vec4 Properties;   // x: radius, y: intensity, w: enabled (if > 0.f)
} lights[LIGHT_SOURCES];

static void onResize(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<SampleApp*>(::glfwGetWindowUserPointer(window));
    app->resize(width, height);
}

void SampleApp::initRenderGraph()
{
    m_renderPass = m_device->buildRenderPass()
        .renderTarget(RenderTargetType::Present, Format::B8G8R8A8_UNORM, { 0.f, 0.f, 0.f, 1.f }, true, false, false)
        .renderTarget(RenderTargetType::DepthStencil, Format::D32_SFLOAT, { 1.f, 0.f, 0.f, 0.f }, true, false, false)
        .go();
}

void SampleApp::initPipelines()
{
    m_pipeline = m_renderPass->makePipeline(Pipelines::Basic, "Basic")
        .withViewport(m_viewport)
        .withScissor(m_scissor)
        .layout()
            .shaderProgram()
                .addVertexShaderModule("shaders/ubo_array_vs.dxi")
                .addFragmentShaderModule("shaders/ubo_array_ps.dxi")
                .go()
            .addDescriptorSet(DescriptorSets::Constant, ShaderStage::Vertex | ShaderStage::Fragment)
                .addUniform(0, sizeof(CameraBuffer))
                .addUniform(1, sizeof(LightBuffer), LIGHT_SOURCES)
                .go()
            .addDescriptorSet(DescriptorSets::PerFrame, ShaderStage::Vertex)
                .addUniform(0, sizeof(TransformBuffer))
                .go()
            .go()
        .rasterizer()
            .withPolygonMode(PolygonMode::Solid)
            .withCullMode(CullMode::BackFaces)
            .withCullOrder(CullOrder::ClockWise)
            .withLineWidth(1.f)
            .go()
        .inputAssembler()
            .withTopology(PrimitiveTopology::TriangleList)
            .withIndexType(IndexType::UInt16)
            .addVertexBuffer(sizeof(Vertex), 0)
                .addAttribute(BufferFormat::XYZ32F, offsetof(Vertex, Position), AttributeSemantic::Position)
                .addAttribute(BufferFormat::XYZW32F, offsetof(Vertex, Color), AttributeSemantic::Color)
                .addAttribute(BufferFormat::XYZ32F, offsetof(Vertex, Normal), AttributeSemantic::Normal)
                .go()
            .go()
        .go();

    m_inputAssembler = m_pipeline->inputAssembler();
}

void SampleApp::initBuffers()
{
    // Get the pipeline instance.
    auto commandBuffer = m_device->bufferQueue().createCommandBuffer(true);

    // Create the staging buffer.
    // NOTE: The mapping works, because vertex and index buffers have an alignment of 0, so we can treat the whole buffer as a single element the size of the 
    //       whole buffer.
    auto stagedVertices = m_device->factory().createVertexBuffer(m_inputAssembler->vertexBufferLayout(0), BufferUsage::Staging, vertices.size());
    stagedVertices->map(vertices.data(), vertices.size() * sizeof(::Vertex), 0);

    // Create the actual vertex buffer and transfer the staging buffer into it.
    m_vertexBuffer = m_device->factory().createVertexBuffer(m_inputAssembler->vertexBufferLayout(0), BufferUsage::Resource, vertices.size());
    commandBuffer->transfer(*stagedVertices, *m_vertexBuffer, 0, 0, vertices.size());

    // Create the staging buffer for the indices. For infos about the mapping see the note about the vertex buffer mapping above.
    auto stagedIndices = m_device->factory().createIndexBuffer(m_inputAssembler->indexBufferLayout(), BufferUsage::Staging, indices.size());
    stagedIndices->map(indices.data(), indices.size() * m_inputAssembler->indexBufferLayout().elementSize(), 0);

    // Create the actual index buffer and transfer the staging buffer into it.
    m_indexBuffer = m_device->factory().createIndexBuffer(m_inputAssembler->indexBufferLayout(), BufferUsage::Resource, indices.size());
    commandBuffer->transfer(*stagedIndices, *m_indexBuffer, 0, 0, indices.size());

    // Initialize the static buffers. The camera and lights buffers are constant, so we only need to create one buffer (for each), that can be read 
    // from all frames. Since this is a write-once/read-multiple scenario, we also transfer the buffer to the more efficient memory heap on the GPU.
    auto& staticBindingLayout = m_pipeline->layout().descriptorSet(DescriptorSets::Constant);
    auto& cameraBufferLayout = staticBindingLayout.descriptor(0);
    m_cameraStagingBuffer = m_device->factory().createBuffer(cameraBufferLayout.type(), BufferUsage::Staging, cameraBufferLayout.elementSize(), 1);
    m_cameraBuffer = m_device->factory().createBuffer(cameraBufferLayout.type(), BufferUsage::Resource, cameraBufferLayout.elementSize(), 1);

    // Allocate the descriptor set and bind the camera buffer to it.
    m_staticBindings = staticBindingLayout.allocate();
    m_staticBindings->update(cameraBufferLayout.binding(), *m_cameraBuffer, 0);

    // Update the camera. Since the descriptor set already points to the proper buffer, all changes are implicitly visible.
    this->updateCamera(*commandBuffer);

    // Allocate the lights buffer and the lights staging buffer.
    this->initLights();
    auto& lightsBufferLayout = staticBindingLayout.descriptor(1);
    auto lightsStagingBuffer = m_device->factory().createBuffer(lightsBufferLayout.type(), BufferUsage::Staging, lightsBufferLayout.elementSize(), LIGHT_SOURCES);
    m_lightsBuffer = m_device->factory().createBuffer(lightsBufferLayout.type(), BufferUsage::Resource, lightsBufferLayout.elementSize(), LIGHT_SOURCES);
    m_staticBindings->update(lightsBufferLayout.binding(), *m_lightsBuffer, 0, LIGHT_SOURCES);

    auto lightsData = lights | std::views::transform([](const LightBuffer& light) { return reinterpret_cast<const void*>(&light); }) | ranges::to<Array<const void*>>();
    lightsStagingBuffer->map(lightsData, sizeof(LightBuffer));
    commandBuffer->transfer(*lightsStagingBuffer, *m_lightsBuffer, 0, 0, LIGHT_SOURCES);

    // Next, we create the descriptor sets for the transform buffer. The transform changes with every frame. Since we have three frames in flight, we
    // create a buffer with three elements and bind the appropriate element to the descriptor set for every frame.
    auto& transformBindingLayout = m_pipeline->layout().descriptorSet(DescriptorSets::PerFrame);
    auto& transformBufferLayout = transformBindingLayout.descriptor(0);
    m_perFrameBindings = transformBindingLayout.allocate(3);
    m_transformBuffer = m_device->factory().createBuffer(transformBufferLayout.type(), BufferUsage::Dynamic, transformBufferLayout.elementSize(), 3);
    std::ranges::for_each(m_perFrameBindings, [this, &transformBufferLayout, i = 0](const UniquePtr<DirectX12DescriptorSet>& descriptorSet) mutable { descriptorSet->update(transformBufferLayout.binding(), *m_transformBuffer, i++); });
    
    // End and submit the command buffer.
    commandBuffer->end(true, true);
}

void SampleApp::initLights()
{
    lights[0] = LightBuffer{ .Position = { -1.f, -1.f, -1.f, 1.f }, .Color = { 0.f, 0.f, 1.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[1] = LightBuffer{ .Position = {  1.f, -1.f, -1.f, 1.f }, .Color = { 1.f, 1.f, 0.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[2] = LightBuffer{ .Position = { -1.f,  1.f, -1.f, 1.f }, .Color = { 0.f, 1.f, 1.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[3] = LightBuffer{ .Position = {  1.f,  1.f, -1.f, 1.f }, .Color = { 1.f, 1.f, 1.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[4] = LightBuffer{ .Position = { -1.f, -1.f,  1.f, 1.f }, .Color = { 0.f, 1.f, 0.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[5] = LightBuffer{ .Position = {  1.f, -1.f,  1.f, 1.f }, .Color = { 1.f, 0.f, 1.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[6] = LightBuffer{ .Position = { -1.f,  1.f,  1.f, 1.f }, .Color = { 1.f, 0.f, 0.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
    lights[7] = LightBuffer{ .Position = {  1.f,  1.f,  1.f, 1.f }, .Color = { 0.f, 0.f, 1.f, 1.f }, .Properties = { 5.f, 2.5f, 0.f, 1.f } };
}

void SampleApp::updateCamera(const DirectX12CommandBuffer& commandBuffer)
{
    // Calculate the camera view/projection matrix.
    auto aspectRatio = m_viewport->getRectangle().width() / m_viewport->getRectangle().height();
    camera.Position = glm::vec4(3.0f, 0.0f, 1.5f, 1.f);
    glm::mat4 view = glm::lookAt(glm::vec3(camera.Position), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.0001f, 1000.0f);
    camera.ViewProjection = projection * view;
    m_cameraStagingBuffer->map(reinterpret_cast<const void*>(&camera), sizeof(camera));
    commandBuffer.transfer(*m_cameraStagingBuffer, *m_cameraBuffer);
}

void SampleApp::run() 
{
    // Start by creating the surface and selecting the adapter.
    auto backend = this->findBackend<DirectX12Backend>(BackendType::Rendering);
    auto adapter = backend->findAdapter(m_adapterId);

    if (adapter == nullptr)
        adapter = backend->findAdapter(std::nullopt);

    auto surface = makeUnique<DirectX12Surface>(::glfwGetWin32Window(m_window.get()));

    // Get the proper frame buffer size.
    int width, height;
    ::glfwGetFramebufferSize(m_window.get(), &width, &height);

    // Create viewport and scissors.
    m_viewport = makeShared<Viewport>(RectF(0.f, 0.f, static_cast<Float>(width), static_cast<Float>(height)));
    m_scissor = makeShared<Scissor>(RectF(0.f, 0.f, static_cast<Float>(width), static_cast<Float>(height)));

    // Create the device with the initial frame buffer size and triple buffering.
    m_device = backend->createDevice(*adapter, *surface, *backend, Format::B8G8R8A8_UNORM, Size2d(width, height), 3);

    // Initialize resources.
    this->initRenderGraph();
    this->initPipelines();
    this->initBuffers();

    // Run application loop until the window is closed.
    while (!::glfwWindowShouldClose(m_window.get()))
    {
        this->handleEvents();
        this->drawFrame();
    }

    // Shut down the device.
    m_device->wait();

    // Destroy all resources.
    m_staticBindings = nullptr;
    m_perFrameBindings.clear();
    m_cameraBuffer = nullptr;
    m_cameraStagingBuffer = nullptr;
    m_lightsBuffer = nullptr;
    m_transformBuffer = nullptr;
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;

    // Destroy the pipeline, render pass and the device.
    m_pipeline = nullptr;
    m_renderPass = nullptr;
    m_device = nullptr;

    // Destroy the window.
    ::glfwDestroyWindow(m_window.get());
    ::glfwTerminate();
}

void SampleApp::initialize()
{
    ::glfwSetWindowUserPointer(m_window.get(), this);
    ::glfwSetFramebufferSizeCallback(m_window.get(), ::onResize); 
}

void SampleApp::resize(int width, int height)
{
    App::resize(width, height);

    if (m_device == nullptr)
        return;

    // In order to re-create the swap chain, we need to wait for all frames in flight to finish.
    m_device->wait();

    // Resize the frame buffer and recreate the swap chain.
    auto surfaceFormat = m_device->swapChain().surfaceFormat();
    auto renderArea = Size2d(width, height);
    m_device->swapChain().reset(surfaceFormat, renderArea, 3);
    // NOTE: Important to do this in order, since dependencies (i.e. input attachments) are re-created and might be mapped to images that do no longer exist when a dependency
    //       gets re-created. This is hard to detect, since some frame buffers can have a constant size, that does not change with the render area and do not need to be 
    //       re-created. We should either think of a clever implicit dependency management for this, or at least document this behavior!
    m_renderPass->resizeFrameBuffers(renderArea);

    // Also resize viewport and scissor.
    m_viewport->setRectangle(RectF(0.f, 0.f, static_cast<Float>(width), static_cast<Float>(height)));
    m_scissor->setRectangle(RectF(0.f, 0.f, static_cast<Float>(width), static_cast<Float>(height)));

    // Also update the camera.
    auto commandBuffer = m_device->bufferQueue().createCommandBuffer(true);
    this->updateCamera(*commandBuffer);
    commandBuffer->end(true, true);
}

void SampleApp::handleEvents()
{
    ::glfwPollEvents();
}

void SampleApp::drawFrame()
{
    // Store the initial time this method has been called first.
    static auto start = std::chrono::high_resolution_clock::now();

    // Swap the back buffers for the next frame.
    auto backBuffer = m_device->swapChain().swapBackBuffer();

    // Begin rendering on the render pass and use the only pipeline we've created for it.
    m_renderPass->begin(backBuffer);
    auto& commandBuffer = m_renderPass->activeFrameBuffer().commandBuffer();
    commandBuffer.use(*m_pipeline);

    // Get the amount of time that has passed since the first frame.
    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration<float, std::chrono::seconds::period>(now - start).count();

    // Compute world transform and update the transform buffer.
    transform.World = glm::rotate(glm::mat4(1.0f), time * glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_transformBuffer->map(reinterpret_cast<const void*>(&transform), sizeof(transform), backBuffer);

    // Bind both descriptor sets to the pipeline.
    commandBuffer.bind(*m_staticBindings);
    commandBuffer.bind(*m_perFrameBindings[backBuffer]);

    // Bind the vertex and index buffers.
    commandBuffer.bind(*m_vertexBuffer);
    commandBuffer.bind(*m_indexBuffer);

    // Draw the object and present the frame by ending the render pass.
    commandBuffer.drawIndexed(m_indexBuffer->elements());
    m_renderPass->end();
}