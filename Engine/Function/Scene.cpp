#include "Engine/Function/Scene.h"
#include <fstream>
#include <regex>
using namespace NoobRenderer;

Scene::Scene(std::string &name)
{
    m_registry = std::make_shared<entt::registry>();
    m_root = std::make_shared<Scene::Node>();
    m_root->object = Entity::Create(name);
    m_current_camera = AddCamera("Main Camera");
}
Scene::Scene(std::string_view name)
{
    m_registry = std::make_shared<entt::registry>();
    m_root = std::make_shared<Scene::Node>();
    m_root->object = Entity::Create(name);
    m_current_camera = AddCamera("Main Camera");
}
Scene::Scene(std::string &name, Node::Ptr &root) : m_root(root)
{
    m_registry = m_root->object->GetRegistry();
    m_current_camera = AddCamera("Main Camera");
}
Scene::Scene()
{
    m_registry = std::make_shared<entt::registry>();
}
Scene::Ptr Scene::FromSceneFile(std::string path)
{
    nlohmann::json scenefile;
    std::ifstream(path) >> scenefile;
    std::string dir = path.substr(0, path.find_last_of("/\\"));

    auto replace_pattern = std::regex("\\\\");
    dir = std::regex_replace(dir, replace_pattern, "/");

    auto dir_regex = std::regex("@CURRENTDIR");
    std::string result = std::regex_replace(scenefile.dump(), dir_regex, dir);
    scenefile = nlohmann::json::parse(result);
    // 切换至默认Registry
    auto _ptr = std::make_shared<Scene>();
    EntityDefaultRegistry::Instance().SwitchDefaultRegistry(_ptr->GetRegistry());
    // 反序列化
    _ptr->m_root = scenefile.at("Graph").get<Scene::Node::Ptr>();
    for (auto &node : _ptr->m_root->childs)
    {
        auto namecmp = node->object->TryGetComponent<component::NameComponent>();
        if (namecmp != nullptr && namecmp->name == "Main Camera")
        {
            _ptr->m_current_camera = node;
            break;
        }
    }
    _ptr->UpdateEntityToNodeCache();
    return _ptr;
}

Scene::~Scene()
{
    m_registry.reset();
    m_current_camera.reset();
    DeleteNode(m_root);
}
Scene::Node::Ptr Scene::CreateNodeAndPushTo()
{
    auto node = std::make_shared<Scene::Node>();
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    AddNode(node);
    return node;
}
Scene::Node::Ptr Scene::CreateNodeAndPushTo(Node::Ptr &target)
{
    auto node = std::make_shared<Scene::Node>();
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    AddNode(node, target);
    return node;
}
void Scene::AddNode(Node::Ptr &node)
{
    AddNode(node, m_root);
}
void Scene::AddNode(Node::Ptr &node, Node::Ptr &target)
{
    target->childs.emplace_back(node);
}
void Scene::DeleteNodeRecursive(Node::Ptr &node)
{
    DeleteNodeRecursive(node, m_root);
}
void Scene::DeleteNodeRecursive(Node::Ptr &node, Node::Ptr &start)
{
    if (start == nullptr)
        return;
    auto count = 0;
    for (auto &child : start->childs)
    {
        if (child == node)
        {
            DeleteNode(node);
            start->childs.erase(start->childs.begin() + count);
            return;
        }
        DeleteNodeRecursive(node, child);
        ++count;
    }
}
Entity::Ptr Scene::CreateEntityWrapInNode()
{
    auto node = CreateNodeAndPushTo();
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    return node->object;
}
Entity::Ptr Scene::CreateEntityWrapInNode(Node::Ptr &target)
{
    auto node = CreateNodeAndPushTo(target);
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    return node->object;
}
Entity::Ptr Scene::CreateEntityWrapInNode(std::string_view name)
{
    auto node = CreateNodeAndPushTo();
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    node->object->AddComponent<component::NameComponent>(name);
    return node->object;
}
Entity::Ptr Scene::CreateEntityWrapInNode(std::string_view name, Node::Ptr &target)
{
    auto node = CreateNodeAndPushTo(target);
    node->object = Entity::Create(this->m_registry);
    m_entityToNodeCache[node->object->GetEntity()] = node;
    node->object->AddComponent<component::NameComponent>(name);
    return node->object;
}
void Scene::DeleteNode(Node::Ptr &node)
{
    if (node == nullptr)
        return;
    for (auto &child : node->childs)
    {
        DeleteNode(child);
    }
    if (node->object != nullptr)
    {
        node->object->Destroy();
        m_entityToNodeCache.erase(node->object->GetEntity());
    }
    node->object.reset();
    node->childs.clear();
}
Scene::Node::Ptr Scene::AddCamera(std::string_view camera_name)
{
    auto node = CreateNodeAndPushTo();
    node->object = Entity::Create(this->m_registry);
    node->object->AddComponent<component::NameComponent>(camera_name);
    node->object->AddComponent<component::Camera>();
    return node;
}
void Scene::SetCurrentMainCamera(Scene::Node::Ptr &camera_node)
{
    m_current_camera = camera_node;
}
void Scene::UpdateEntityToNodeCache()
{
    m_entityToNodeCache.clear();
    std::function<void(Node::Ptr & node)> dfs = [&](Node::Ptr &node)
    {
        if (node == nullptr)
            return;
        if (node->object != nullptr)
        {
            m_entityToNodeCache[node->object->GetEntity()] = node;
        }
        for (auto &child : node->childs)
            dfs(child);
    };
    dfs(m_root);
}
void Scene::UpdateAllNodesModelMatrix()
{
    std::function<void(Node::Ptr &, glm::mat4)> dfs = [&](Node::Ptr &node, glm::mat4 parent_matrix)
    {
        if (node == nullptr)
            return;
        auto transform = node->object->TryGetComponent<component::Transform>();
        if (transform != nullptr)
        {
            transform->CalculateModelMatrix();
            parent_matrix *= transform->model;
        }
        node->model = parent_matrix;
        for (auto &child : node->childs)
            dfs(child, parent_matrix);
    };
    dfs(m_root, glm::mat4(1.0f));
}
void Scene::BasicRender(ShaderProgram::Ptr &shader)
{
    auto scene_view = m_registry->view<component::MeshData>();
    for (auto [entity, mesh_data] : scene_view.each())
    {
        if (mesh_data.mesh == nullptr)
            continue;
        shader->SetUniform("model", m_entityToNodeCache[entity]->model);
        mesh_data.mesh->Draw();
    }
}
void Scene::ForeachCamera(CaptureCameraFunction &func)
{
    auto view = m_registry->view<component::NameComponent, component::Camera>();
    for (auto [_, name, camera] : view.each())
    {
        func(name.name, camera);
    }
}
nlohmann::json Scene::GetSceneFile()
{
    nlohmann::json _jsonfile;
    _jsonfile["Graph"] = m_root;
    return _jsonfile;
}
Scene::Node::Ptr &Scene::GetSceneNodeByEntity(entt::entity entity)
{
    return m_entityToNodeCache[entity];
}
void Scene::GetSceneNodeByEntity(entt::entity entity, Node::Ptr &node)
{
    node = m_entityToNodeCache[entity];
}