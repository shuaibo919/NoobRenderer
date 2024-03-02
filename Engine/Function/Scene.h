#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Entity.h"
#include "Engine/Function/Graphics/Shader.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Graphics/Texture/All.h"

namespace NoobRenderer
{

    /**
     * @brief 场景
     */
    class Scene
    {
    public:
        struct Setting
        {
            float ambient_strength{0.2f};
            bool enable_hdr{true};
            bool enable_gamma_correction{true};
            bool enable_antialias{true};
            bool enable_bloom{true};
        };
        struct Node
        {
            using Ptr = std::shared_ptr<Scene::Node>;
            glm::mat4 model{1.0f};
            Entity::Ptr object{nullptr};
            std::vector<Node::Ptr> childs;
            friend void to_json(nlohmann::json &nlohmann_json_j, const Scene::Node::Ptr &nlohmann_json_t)
            {
                nlohmann_json_j["model"] = nlohmann_json_t->model;
                if (nlohmann_json_t->object != nullptr)
                {
                    nlohmann_json_j["object"] = (*nlohmann_json_t->object);
                    nlohmann_json_j["childs"] = nlohmann::json::array();
                    for (auto &child : nlohmann_json_t->childs)
                    {
                        nlohmann::json tmp;
                        to_json(tmp, child);
                        nlohmann_json_j["childs"].push_back(tmp);
                    }
                }
            }
            friend void from_json(const nlohmann::json &nlohmann_json_j, Scene::Node::Ptr &nlohmann_json_t)
            {
                nlohmann_json_t = std::make_shared<Scene::Node>();
                nlohmann_json_t->model = nlohmann_json_j.at("model").get<glm::mat4>();
                if (nlohmann_json_j.find("object") != nlohmann_json_j.end())
                {
                    nlohmann_json_t->object = nlohmann_json_j.at("object").get<Entity::Ptr>();
                }
                if (nlohmann_json_j.find("childs") != nlohmann_json_j.end())
                {
                    // recursive way to recover node from json
                    for (auto &child_json : nlohmann_json_j["childs"])
                    {
                        Scene::Node::Ptr _ptr = child_json;
                        nlohmann_json_t->childs.emplace_back(_ptr);
                    }
                }
            }
        };

    public:
        using Ptr = std::shared_ptr<Scene>;
        using CaptureCameraFunction = std::function<void(std::string, component::Camera &)>;
        using Index = unsigned int;
        Scene();
        Scene(std::string &name);
        Scene(std::string_view name);
        Scene(std::string &name, Node::Ptr &root);
        static Scene::Ptr FromSceneFile(std::string path);
        ~Scene();

        /**
         * @brief 添加节点至Root
         */
        Node::Ptr CreateNodeAndPushTo();

        /**
         * @brief 添加节点至Root
         * @param target 被添加节点(目标)
         */
        Node::Ptr CreateNodeAndPushTo(Node::Ptr &target);

        /**
         * @brief 添加节点至Root
         * @param node 待添加节点
         */
        void AddNode(Node::Ptr &node);

        /**
         * @brief 添加节点
         * @param node 待添加节点
         * @param target 被添加节点(目标)
         */
        void AddNode(Node::Ptr &node, Node::Ptr &target);

        /**
         * @brief 以递归方式删除某个节点, 默认搜索起点为根节点, 节点为空则无操作执行
         * @param node 待删除节点
         */
        void DeleteNodeRecursive(Node::Ptr &node);

        /**
         * @brief 以递归方式删除某个节点, 默认搜索起点为根节点, 节点为空则无操作执行
         * @param node 待删除节点
         */
        void DeleteNodeRecursive(Node::Ptr &node, Node::Ptr &start);

        /**
         * @brief 添加一个机位(只有主机位会被渲染)
         * @param camera_name 添加摄像机
         */
        Scene::Node::Ptr AddCamera(std::string_view camera_name);

        /**
         * @brief 根据摄像机的名称字段,设置当前的主机位(RenderEngine默认的主机位名称为"Main")
         * @param camera_name 摄像机节点
         */
        void SetCurrentMainCamera(Scene::Node::Ptr &camera_node);

        /**
         * @brief 遍历所有机位
         * @param func CaptureCameraFunction - void(std::string,std::shared_ptr<Camera>)
         */
        void ForeachCamera(CaptureCameraFunction &func);

        /**
         * @brief 序列化
         * @return 序列化场景json
         */
        nlohmann::json GetSceneFile();

        /**
         * @brief No checking, use carefully!
         */
        Node::Ptr &GetSceneNodeByEntity(entt::entity entity);
        void GetSceneNodeByEntity(entt::entity entity, Node::Ptr &node);

        /**
         * @brief 区别于Entity::Create, Scene::CreateEntityWrapInNode 在创建实体时自动指定Registry为Scene,并为创建的Entity添加TreeNode(Scene::node)视图
         * @return Node中的object实体指针
         */
        Entity::Ptr CreateEntityWrapInNode();

        /**
         * @brief 区别于Entity::Create, Scene::CreateEntityWrapInNode 在创建实体时自动指定Registry为Scene,并为创建的Entity添加TreeNode(Scene::node)视图
         * @param target 被添加节点(目标)
         * @return Node中的object实体指针
         */
        Entity::Ptr CreateEntityWrapInNode(Node::Ptr &target);

        /**
         * @brief 区别于Entity::Create, Scene::CreateEntityWrapInNode 在创建实体时自动指定Registry为Scene,并为创建的Entity添加TreeNode(Scene::node)视图
         * @param name 指定Node的名称
         * @return Node中的object实体指针
         */
        Entity::Ptr CreateEntityWrapInNode(std::string_view name);

        /**
         * @brief 区别于Entity::Create, Scene::CreateEntityWrapInNode 在创建实体时自动指定Registry为Scene,并为创建的Entity添加TreeNode(Scene::node)视图
         * @param name 指定Node的名称
         * @param target target 被添加节点(目标)
         * @return Node中的object实体指针
         */
        Entity::Ptr CreateEntityWrapInNode(std::string_view name, Node::Ptr &target);

        /**
         * @brief 以给定shader执行最基础的render,通常用于如shadow map等预计算的过程中
         * @param shader
         */
        void BasicRender(ShaderProgram::Ptr &shader);

        void UpdateAllNodesModelMatrix();

    private:
        std::unordered_map<entt::entity, Node::Ptr> m_entityToNodeCache; // 实体到节点的缓存,用于快速查找
        void DeleteNode(Node::Ptr &node);
        void UpdateEntityToNodeCache();

    public:
        inline Node::Ptr &GetRootNode() { return m_root; }
        inline const std::string &GetName() { return m_root->object->GetComponent<component::NameComponent>().name; }
        inline Scene::Node::Ptr GetCurrentCamera() { return m_current_camera; }
        inline component::Camera &GetCurrentCameraComponent() { return m_current_camera->object->GetComponent<component::Camera>(); }
        inline std::shared_ptr<entt::registry> GetRegistry() { return m_registry; }
        inline Scene::Setting &GetConfig() { return m_config; }

    private:
        std::shared_ptr<entt::registry> m_registry{nullptr};
        Scene::Node::Ptr m_current_camera{nullptr};
        Node::Ptr m_root;
        /* scene properties */
        Scene::Setting m_config;
    };
} // namespace NoobRenderer
