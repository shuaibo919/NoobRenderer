#include "Engine/Function/SceneManager.h"
using namespace NoobRenderer;
void SceneManager::Init()
{
}
void SceneManager::AddScene(Scene::Ptr &scene)
{
    m_scenes[m_current_count++] = scene;
}
Scene::Index SceneManager::CreateFromFileAndAdd(std::string path)
{
    auto _ptr = Scene::FromSceneFile(path);
    m_scenes[m_current_count] = _ptr;
    return m_current_count++;
}
Scene::Index SceneManager::CreateEmptyAndAdd(std::string &name)
{
    auto _ptr = std::make_shared<Scene>(name);
    m_scenes[m_current_count] = _ptr;
    return m_current_count++;
}
Scene::Index SceneManager::CreateEmptyAndAdd(std::string_view name)
{
    auto _ptr = std::make_shared<Scene>(name);
    m_scenes[m_current_count] = _ptr;
    return m_current_count++;
}
Scene::Ptr SceneManager::GetCurrentScene()
{
    return m_current_scene;
}
Scene::Node::Ptr &SceneManager::GetCurrentSceneNode()
{
    return m_current_scene->GetRootNode();
}
Scene::Ptr SceneManager::GetScene(Scene::Index idx)
{
    if (m_scenes.find(idx) == m_scenes.end())
        return nullptr;
    return m_scenes[idx];
}
void SceneManager::DeleteScene(Scene::Index idx)
{
    if (m_scenes.find(idx) == m_scenes.end())
        return;
    m_scenes.erase(idx);
}
void SceneManager::DeleteScene(Scene::Ptr &ptr)
{
    Scene::Index idx = -1;
    for (auto &[id, p] : m_scenes)
    {
        if (p == ptr)
        {
            idx = id;
            break;
        }
    }
    DeleteScene(idx);
    if (ptr == m_current_scene)
    {
        m_current_scene = nullptr;
        ptr.reset();
    }
}
void SceneManager::SetCurrentScene(Scene::Index idx)
{
    if (m_scenes.find(idx) != m_scenes.end())
        m_current_scene = m_scenes[idx];
}