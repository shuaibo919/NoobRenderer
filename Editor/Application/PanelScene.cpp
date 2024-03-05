#include "Application/PanelScene.h"
using namespace NoobRenderer::Application;
namespace ScenePanelHelper
{
    // Some helper Functions in Here.
    static std::string_view DefaultEntityNames[] = {"Plane", "Cube", "Sphere", "Cylinder"};
    static std::string_view DefualtEntityPaths[] = {"Resource/Asset/basic/plane.obj", "Resource/Asset/basic/cube.obj", "Sphere", "Resource/Asset/basic/cylinder.obj"};
    static void AddEntityToScene(std::string name, std::string path, NoobRenderer::Scene::Ptr &scene, NoobRenderer::Scene::Node::Ptr &target, bool assign_default_material = true)
    {
        using namespace NoobRenderer;
        auto _scene_node = scene->CreateNodeAndPushTo(target);
        auto _mesh_list_size = MeshManager::Instance().RetrieveMeshSharedSize(path);
        _scene_node->object->AddComponent<component::NameComponent>(name);
        _scene_node->object->AddComponent<component::Transform>();
        for (int i = 0; i < _mesh_list_size; i++)
        {
            auto _child_entity = scene->CreateEntityWrapInNode("mesh_" + std::to_string(i), _scene_node);
            auto &_child_mesh = _child_entity->AddComponent<component::MeshData>(path, i);
            if (assign_default_material)
                _child_mesh.AssignDefaultMaterial();
        }
    }
    static void AddEntityToScene(std::string_view name, std::string_view path, NoobRenderer::Scene::Ptr &scene, NoobRenderer::Scene::Node::Ptr &target, bool assign_default_material = true)
    {
        AddEntityToScene(std::string(name), std::string(path), scene, target, assign_default_material);
    }
    template <EngineUtility::TisBasedOf<NoobRenderer::component::Light> T>
    static void AddLightToScene(std::string_view name, NoobRenderer::Scene::Ptr &scene, NoobRenderer::Scene::Node::Ptr &target)
    {
        auto _entity = scene->CreateEntityWrapInNode(name, target);
        _entity->AddComponent<T>();
    }

    static const char *GetEntityIcon(NoobRenderer::Scene::Node::Ptr &node)
    {
        using namespace NoobRenderer;
        auto &entity = node->object;
        if (entity->HasComponent<component::Camera>())
            return ICON_FA_CAMERA_RETRO;
        if (entity->HasComponent<component::DirectionalLight>())
            return ICON_FA_LIGHTBULB;
        if (entity->HasComponent<component::PointLight>())
            return ICON_FA_LIGHTBULB;
        return ICON_FA_CUBE;
    }

}
ScenePanel::ScenePanel(const std::string &name, bool show) : m_name(name), m_show(show) {}
void ScenePanel::ListAllScenes(Scene::Index idx, Scene::Ptr &scene_node, int &node_cnt)
{
    using namespace NoobRenderer;
    auto &root = scene_node->GetRootNode();
    // 检查标志位
    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    NodeFlags = (Selected::Instance().node == root) ? NodeFlags | (ImGuiTreeNodeFlags_Selected) : NodeFlags;
    // 是否可展开
    bool NodeExtend = ImGui::TreeNodeEx(IconFont::WithIcon(ICON_FA_CUBES, scene_node->GetName()).c_str(), NodeFlags);
    // 右键小菜单
    ListScenePopupItems(scene_node, root, node_cnt);
    // 是否选中当前场景
    if (ImGui::IsItemClicked())
    {
        Selected::Instance().node = root;
        SceneManager::Instance().SetCurrentScene(idx);
        std::cout << "ApplicationUI::Select Scene-" << idx << ":" << scene_node->GetName() << std::endl;
    }
    // 文件加载
    FileDiaglogues(scene_node, root);
    // 节点展开
    if (NodeExtend)
    {
        // 遍历SceneNode
        for (auto &child : root->childs)
        {
            ListAllNodes(child, root, scene_node, node_cnt, NodeFlags);
        }
        ImGui::TreePop();
        // 删除节点
        if (m_wait_deleted == Selected::Instance().node)
            Selected::Instance().node = nullptr;
        scene_node->DeleteNodeRecursive(m_wait_deleted, m_wait_deleted_parent);
    }
}
void ScenePanel::ListAllNodes(Scene::Node::Ptr &node, Scene::Node::Ptr &pre, Scene::Ptr &scene, int &node_cnt, ImGuiTreeNodeFlags flags)
{
    using namespace NoobRenderer;
    Entity::Ptr entity = node->object;
    auto &node_name = node->object->GetComponent<component::NameComponent>().name;
    ImGuiTreeNodeFlags NodeFlags = flags;

    NodeFlags = (node->childs.size() == 0) ? NodeFlags | (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen) : NodeFlags;
    NodeFlags = (Selected::Instance().node == node) ? NodeFlags | (ImGuiTreeNodeFlags_Selected) : NodeFlags;

    bool isExtend = ImGui::TreeNodeEx(IconFont::WithIcon(ScenePanelHelper::GetEntityIcon(node), node_name).c_str(), NodeFlags);

    // 右键小菜单
    if (ImGui::BeginPopupContextItem(std::to_string(node_cnt++).c_str()))
    {
        if (scene->GetCurrentCamera() != node && ImGui::MenuItem("Delete"))
        {
            m_wait_deleted = node;
            m_wait_deleted_parent = pre;
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked())
    {
        Selected::Instance().node = node;
        std::cout << "ApplicationUI::Select:" << node_name << std::endl;
    }

    if (isExtend)
    {
        if (node->childs.empty())
            return;
        for (auto &child : node->childs)
        {
            ListAllNodes(child, node, scene, node_cnt, flags);
        }
        ImGui::TreePop();
    }
}
void ScenePanel::ListScenePopupItems(Scene::Ptr &scene_node, Scene::Node::Ptr &root, int &node_cnt)
{
    if (ImGui::BeginPopupContextItem(std::to_string(node_cnt++).c_str()))
    {
        if (ImGui::MenuItem("Load CubeMap"))
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            config.sidePaneWidth = 400;
            ImGuiFileDialog::Instance()->OpenDialog(common::LoadCubeMapLabel, "Choose File", ".hdr", config);
        }
        if (ImGui::BeginMenu("Add Entity"))
        {
            if (ImGui::MenuItem("From File"))
            {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.sidePaneWidth = 400;
                const char *filters = "Model files (*.obj *.glb *.gltf){.obj,.glb,.gltf}";
                ImGuiFileDialog::Instance()->OpenDialog(common::LoadModelLabel, "Choose File", filters, config);
            }
            for (int i = 0; i < 4; i++)
            {
                if (ImGui::MenuItem(ScenePanelHelper::DefaultEntityNames[i].data()))
                {
                    ScenePanelHelper::AddEntityToScene(ScenePanelHelper::DefaultEntityNames[i],
                                                       ScenePanelHelper::DefualtEntityPaths[i], scene_node, root, true);
                }
            }
            if (ImGui::MenuItem("Point Light"))
                ScenePanelHelper::AddLightToScene<component::PointLight>("Point Light", scene_node, root);
            if (ImGui::MenuItem("Directional Light"))
                ScenePanelHelper::AddLightToScene<component::DirectionalLight>("Directional Light", scene_node, root);
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Delete"))
        {
            m_wait_deleted_scene = scene_node;
        }
        if (ImGui::MenuItem("Svae"))
        {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog(common::SavingChooseDirectory, "Choose a Directory", nullptr, config);
        }
        if (ImGui::BeginItemTooltip())
        {
            ImGui::Text("The save function is still under development. (todo)");
            ImGui::EndTooltip();
        }
        ImGui::EndPopup();
    }
}
void ScenePanel::FileDiaglogues(Scene::Ptr &scene_node, Scene::Node::Ptr &root)
{
    auto getfile = common::ShowFileDiaglogue(common::LoadModelLabel);
    if (getfile.first != "" && getfile.second != "")
    {
        ScenePanelHelper::AddEntityToScene(getfile.second, getfile.first, scene_node, root, false);
    }
    getfile = common::ShowFileDiaglogue(common::LoadCubeMapLabel);
    if (getfile.first != "" && getfile.second != "")
    {
        auto &ibl_cubemap = root->object->GetOrAddComponent<component::IBLCubeMap>();
        ibl_cubemap.name = getfile.first;
        ibl_cubemap.PreCalculation();
    }
    auto dir = common::ShowDirectoryDiaglogue(common::SavingChooseDirectory);
    if (dir != "")
    {
        auto json = scene_node->GetSceneFile();
        std::ofstream file(dir + "/" + scene_node->GetName() + ".json");
        if (file)
        {
            file << json;
            file.close();
        }
    }
}
void ScenePanel::Show()
{
    if (m_show)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGui::Begin(m_name.c_str(), &m_show, window_flags))
        {
            auto &all_scenes = NoobRenderer::SceneManager::Instance();
            int cnt = 0;
            for (auto &[_idx, _scene] : all_scenes)
            {
                ListAllScenes(_idx, _scene, cnt);
            }
            if (m_wait_deleted_scene != nullptr)
            {
                SceneManager::Instance().DeleteScene(m_wait_deleted_scene);
                m_wait_deleted_scene = nullptr;
                Selected::Instance().node = nullptr;
            }
        }
        ImGui::End();
    }
}

/* Beta */
void ScenePanel::RayCasting(int mouse_x, int mouse_y, int view_width, int view_height)
{
    auto scene = SceneManager::Instance().GetCurrentScene();
    if (scene == nullptr)
        return;
    auto &camera = scene->GetCurrentCameraComponent();
    auto registry = scene->GetRegistry();
    auto view = registry->view<component::MeshData>();
    // 计算Mouse_Ray
    glm::vec3 ray_ndc = EngineUtility::GetNormalisedDeviceCoordinates(static_cast<float>(mouse_x), static_cast<float>(mouse_y), view_width, view_height);
    // we want our ray's z to point forwards - this is usually the negative z direction in OpenGL style
    glm::vec4 ray_clip = glm::vec4(ray_ndc.x, ray_ndc.y, -1.0, 1.0);
    glm::vec4 ray_eye = EngineUtility::ToEyeCoords(ray_clip, camera.GetProjectionMatrix());
    glm::vec3 ray_world = EngineUtility::ToWorldCoords(ray_eye, camera.GetViewMatrix());

    auto _record = EngineUtility::RayCastingRecord();
    auto _viewpos = camera.GetPositionVector();
    Scene::Node::Ptr _hitted_node = nullptr;
    for (auto [entity, meshdata] : view.each())
    {
        glm::mat4 model = scene->GetSceneNodeByEntity(entity)->model;
        auto &_mesh = MeshManager::Instance().GetMesh(meshdata.name, meshdata.index);
        auto get_picking = _mesh->IsPicking(_viewpos, model, ray_world);
        if (get_picking.GetHitted() && get_picking.GetRayFactor() < _record.GetRayFactor())
        {
            _record = get_picking;
            _hitted_node = scene->GetSceneNodeByEntity(entity);
        }
    }
    if (_hitted_node != nullptr)
    {
        std::cout << "Pick:" << _hitted_node->object->GetComponent<component::NameComponent>().name << std::endl;
        // Todo: hint for mesh picked.
    }
}