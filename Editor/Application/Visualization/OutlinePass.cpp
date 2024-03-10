#include "Application/Visualization/OutlinePass.h"
using namespace NoobRenderer::Application;
void DrawPickedMeshOutlinePass::Init(unsigned int width, unsigned int height)
{
    m_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/Editor/Debug.vert",
        "Resource/Shader/Editor/Debug.frag");
    // m_blur_shader = ShaderManager::Instance().LoadShaderAndGet(
    //     "Resource/Shader/Editor/Debug.vert",
    //     "Resource/Shader/Editor/Debug.frag");
    m_sobel_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/Editor/Sobel.vert",
        "Resource/Shader/Editor/Sobel.frag");
    m_rt = std::make_shared<WriteToTexture>(width, height);
    m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                  Texture::Format::RGBA, Texture::Format::RGBA16F, Texture::DataType::FLOAT);
    m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8);
    m_rt->Unbind();
    std::cout << "Editor DebugPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    m_edge = std::make_shared<WriteToTexture>(width, height);
    m_edge->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                    Texture::Format::RGBA, Texture::Format::RGBA16F, Texture::DataType::FLOAT);
    m_edge->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_edge->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_edge->SetRenderBuffer<RenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8);
    m_edge->Unbind();
    std::cout << "Editor DebugPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
}
void DrawPickedMeshOutlinePass::Rescale(unsigned int width, unsigned int height)
{
    m_rt->Rescale(width, height);
    m_edge->Rescale(width, height);
}
void DrawPickedMeshOutlinePass::Render(Scene::Node::Ptr &node)
{
    auto &final_rt = Engine::Instance().GetCurrentRenderer()->RTS.Get("Result");
    if (node == nullptr || !node->object->HasComponent<component::MeshData>())
    {
        Engine::Instance().GetCurrentRenderer()->RTS.Insert("Editor Result", final_rt);
        return;
    }
    auto &geometry_rt = Engine::Instance().GetCurrentRenderer()->GetRTHasGeometryInfo();
    auto &camera = SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent();

    geometry_rt->BlitBufferFrom(m_rt, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    m_rt->Bind();
    glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto translate = EngineUtility::DecomposePositonFromMatrix(node->model);
    auto dir = glm::normalize(translate - camera.GetPositionVector());
    auto rectify_pos = camera.GetPositionVector() + dir * 0.1f;
    // 1. Draw the object background
    m_shader->SetUniform("view", glm::lookAt(rectify_pos, rectify_pos + camera.GetFrontVector(), camera.GetUpVector()));
    m_shader->SetUniform("projection", camera.GetProjectionMatrix());
    m_shader->SetUniform("model", node->model);
    m_shader->SetUniform("drawColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    node->object->GetComponent<component::MeshData>().mesh->Draw();
    m_rt->Unbind();
    // 2. Get the outline
    m_edge->Bind();
    Texture::ResetSlot();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    m_rt->GetTexture()->Activate();
    final_rt->Activate();
    m_sobel_shader->SetUniform("texture", m_rt->GetTexture()->GetTempSlot());
    m_sobel_shader->SetUniform("edgeColor", m_edge_color);
    m_sobel_shader->SetUniform("finalRT", final_rt->GetTempSlot());
    m_sobel_shader->SetUniform("width", static_cast<float>(m_rt->GetWidth()));
    m_sobel_shader->SetUniform("height", static_cast<float>(m_rt->GetHeight()));
    EngineUtility::DrawQuad();
    m_edge->Unbind();
    Engine::Instance().GetCurrentRenderer()->RTS.Insert("Editor Result", m_edge->GetTexture());
}