#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Function/Graphics/Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace NoobRenderer
{
    class MeshManager final : public Singleton<MeshManager>
    {
    private:
        std::unordered_map<std::string, std::vector<Mesh::Ptr>> m_meshLists;
        std::unordered_map<std::string, std::array<float, 6>> m_meshListBoxs;

    public:
        MeshManager() = delete;
        MeshManager(token){};
        ~MeshManager();
        void Init();

        size_t RetrieveMeshSharedSize(std::string_view name);
        Mesh::Ptr RetrieveMeshShared(std::string_view name, MeshIndex id);
        std::vector<Mesh::Ptr> GetMeshList(std::string_view name);
        std::array<float, 6> GetMeshListBox(std::string_view name);
        size_t GetMeshsCount(std::string_view name);
        void DeleteMeshList(std::string name);
        bool HasMesh(std::string_view name, MeshIndex id = 0);
        Mesh::Ptr &GetMesh(std::string_view name, MeshIndex id = 0);
        std::vector<std::string> GetAllMeshListName();
        bool Import(std::string_view obj_path);
        void CreateBuildInSphere();
        void CreateBuildInCube();

    public:
        inline size_t GetMeshListCount() const { return m_meshLists.size(); }
        inline std::unordered_map<std::string, std::vector<Mesh::Ptr>> &GetMeshLists() { return m_meshLists; };
        inline std::array<std::string, 2> GetBuildInMeshsName() const { return {"Cube", "Sphere"}; };

    private:
        std::unordered_map<std::string, OrdinaryTexture::Ptr> m_tmp_tex_loaded;
        Mesh::Ptr ProcessAssimpMesh(aiMesh *mesh, const aiScene *scene);
        void ProcessAssimpNode(std::vector<Mesh::Ptr> &mesh_lists, std::array<float, 6> &bbox, aiNode *node, const aiScene *scene, const std::string &dir);
    };
}