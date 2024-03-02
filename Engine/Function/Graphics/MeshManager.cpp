#include "Engine/Function/Graphics/MeshManager.h"
#include "Engine/Function/Graphics/Material/All.h"
#include "Engine/Function/Graphics/Texture/WriteToTexture.h"
#include "Engine/Core/Utility.h"
#include "stb_image.h"
#include <queue>
using namespace NoobRenderer;

namespace LoadingUtils
{
    static std::unordered_map<std::string, OrdinaryTexture::Ptr> texure_cache;

    static Texture::Type FromaiTextureType(aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            return Texture::Type::TextureAlbedo;
        case aiTextureType_SPECULAR:
            return Texture::Type::TextureSpecular;
        case aiTextureType_AMBIENT:
            return Texture::Type::TextureAmbient;
        case aiTextureType_HEIGHT:
            return Texture::Type::TextureHeight;
        case aiTextureType_NORMALS:
            return Texture::Type::TextureNormal;
        case aiTextureType_BASE_COLOR:
            return Texture::Type::TextureAlbedo;
        case aiTextureType_AMBIENT_OCCLUSION:
            return Texture::Type::TextureAmbientOcclusion;
        case aiTextureType_METALNESS:
            return Texture::Type::TextureMetallic;
        case aiTextureType_DIFFUSE_ROUGHNESS:
            return Texture::Type::TextureRoughness;
        default:
            break;
        }
        return Texture::Type::None;
    }

    static NoobRenderer::OrdinaryTexture::Ptr ReadMaterialUsingAssimp(aiMaterial *mat, aiTextureType type, const std::string &dir, int specifc_channel = -1)
    {
        auto load_type = FromaiTextureType(type);
        auto counts = mat->GetTextureCount(type);
        if (counts < 1)
            return nullptr;
        aiString str;
        mat->GetTexture(type, 0, &str);
        auto filename = dir + "/" + std::string(str.C_Str());
        return std::make_shared<OrdinaryTexture>(filename, specifc_channel, load_type);
    }

    static NoobRenderer::MaterialTexture::Ptr CreateMaterialUsingAssimp(aiMaterial *&mat, aiTextureType type,
                                                                        const char *_pKey, unsigned int _type, unsigned int _idx,
                                                                        const std::string &dir, int specifc_channel = -1, bool vec3 = false)
    {
        OrdinaryTexture::Ptr tex = ReadMaterialUsingAssimp(mat, type, dir, specifc_channel);
        if (tex != nullptr)
        {
            return std::make_shared<MaterialTexture>(tex);
        }
        MaterialTexture::Ptr mat_tex = nullptr;
        if (vec3)
        {
            aiColor3D color;
            mat->Get(_pKey, _type, _idx, color);
            mat_tex = std::make_shared<MaterialTexture>(glm::vec3(color.r, color.g, color.b));
        }
        else
        {
            float val;
            mat->Get(_pKey, _type, _idx, val);
            val = std::max(std::min(val, 1.0f), 0.05f);
            mat_tex = std::make_shared<MaterialTexture>(val);
        }

        return mat_tex;
    }
    static NoobRenderer::MaterialTexture::Ptr CreateMaterialUsingAssimp(aiMaterial *mat, aiTextureType type, const std::string &dir, int specifc_channel = -1)
    {
        OrdinaryTexture::Ptr tex = ReadMaterialUsingAssimp(mat, type, dir, specifc_channel);
        if (tex != nullptr)
        {
            return std::make_shared<MaterialTexture>(tex);
        }
        return nullptr;
    }

    static NoobRenderer::MaterialTexture::Ptr CreateMaterialUsingAssimp(aiMaterial *mat, aiTextureType type, const std::string &dir, bool vec3, int specifc_channel = -1)
    {
        auto mat_tex = CreateMaterialUsingAssimp(mat, type, dir, specifc_channel);
        if (mat_tex == nullptr)
        {
            mat_tex = vec3 ? std::make_shared<MaterialTexture>(glm::vec3(0.0f)) : std::make_shared<MaterialTexture>(0.45f);
        }
        return mat_tex;
    }

}

void MeshManager::Init()
{
    CreateBuildInCube();
    CreateBuildInSphere();
}

MeshManager::~MeshManager()
{
}

std::vector<Mesh::Ptr> MeshManager::GetMeshList(std::string_view name)
{
    if (m_meshLists.find(name.data()) != m_meshLists.end())
    {
        return m_meshLists[name.data()];
    }
    return {};
}
std::array<float, 6> MeshManager::GetMeshListBox(std::string_view name)
{
    if (m_meshLists.find(name.data()) != m_meshLists.end())
    {
        return m_meshListBoxs[name.data()];
    }
    return {};
}
bool MeshManager::HasMesh(std::string_view name, MeshIndex id)
{
    return m_meshLists.find(name.data()) != m_meshLists.end() && m_meshLists[name.data()].size() > id;
}
Mesh::Ptr &MeshManager::GetMesh(std::string_view name, MeshIndex id)
{
    return m_meshLists[name.data()][id];
}
void MeshManager::DeleteMeshList(std::string name)
{
    if (m_meshLists.find(name) != m_meshLists.end())
    {
        m_meshLists.erase(name);
        m_meshListBoxs.erase(name);
    }
}

size_t MeshManager::RetrieveMeshSharedSize(std::string_view name)
{
    if (m_meshLists.find(name.data()) != m_meshLists.end())
    {
        return m_meshLists[name.data()].size();
    }
    auto success = MeshManager::Import(name);
    if (success)
        return RetrieveMeshSharedSize(name);
    return -1;
}
Mesh::Ptr MeshManager::RetrieveMeshShared(std::string_view name, MeshIndex id)
{
    if (m_meshLists.find(name.data()) != m_meshLists.end())
    {
        return std::make_shared<Mesh>(*(m_meshLists[name.data()][id]));
    }
    auto success = MeshManager::Import(name);
    if (success)
        return RetrieveMeshShared(name, id);
    return nullptr;
}
size_t MeshManager::GetMeshsCount(std::string_view name)
{
    if (m_meshLists.find(name.data()) != m_meshLists.end())
    {
        return m_meshLists[name.data()].size();
    }
    return 0;
}

bool MeshManager::Import(std::string_view obj_path)
{
    // read file via ASSIMP
    auto path = std::string(obj_path.data());
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                       aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "MeshSetManager::Import:: " << importer.GetErrorString() << std::endl;
        return false;
    }
    // retrieve the directory path of the filepath
    auto directory = path.substr(0, path.find_last_of("/\\"));
    std::cout << "Loading model data from: " << directory << std::endl;

    // process ASSIMP's root node recursively
    m_tmp_tex_loaded.clear();
    m_meshLists[obj_path.data()] = std::vector<Mesh::Ptr>();
    m_meshListBoxs[obj_path.data()] = {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::min(),
        std::numeric_limits<float>::min()};
    ProcessAssimpNode(m_meshLists[obj_path.data()], m_meshListBoxs[obj_path.data()], scene->mRootNode, scene, directory);

    return true;
}

void MeshManager::ProcessAssimpNode(std::vector<Mesh::Ptr> &mesh_lists, std::array<float, 6> &bbox,
                                    aiNode *node, const aiScene *scene, const std::string &dir)
{
    if (node == nullptr)
        return;
    std::queue<aiNode *> q;
    q.push(node);
    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();
        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < cur->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[cur->mMeshes[i]];
            if (mesh == nullptr)
                continue;
            auto box = scene->mMeshes[cur->mMeshes[i]]->mAABB;
            bbox[0] = EngineUtility::Min(box.mMin.x, bbox[0]);
            bbox[1] = EngineUtility::Min(box.mMin.y, bbox[1]);
            bbox[2] = EngineUtility::Min(box.mMin.z, bbox[2]);
            bbox[3] = EngineUtility::Max(box.mMax.x, bbox[3]);
            bbox[4] = EngineUtility::Max(box.mMax.y, bbox[4]);
            bbox[5] = EngineUtility::Max(box.mMax.z, bbox[5]);

            auto mesh_ptr = ProcessAssimpMesh(mesh, scene);
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            PBRMaterial::Ptr pbrMat = std::make_shared<PBRMaterial>();
            pbrMat->name = std::string(material->GetName().C_Str());
            pbrMat->albedo = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_BASE_COLOR, AI_MATKEY_BASE_COLOR, dir, -1, true);
            pbrMat->emission = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_EMISSION_COLOR, dir, true);
            pbrMat->metallic = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_METALNESS, AI_MATKEY_METALLIC_FACTOR, dir, 0, false);
            pbrMat->roughness = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_DIFFUSE_ROUGHNESS, AI_MATKEY_ROUGHNESS_FACTOR, dir, 1, false);
            pbrMat->normal = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_NORMALS, dir);
            pbrMat->height = LoadingUtils::CreateMaterialUsingAssimp(material, aiTextureType_HEIGHT, dir);
            mesh_ptr->SetMaterial(pbrMat);
            mesh_lists.push_back(mesh_ptr);
        }
        for (unsigned int i = 0; i < cur->mNumChildren; i++)
        {
            if (cur->mChildren[i] != nullptr)
                q.push(cur->mChildren[i]);
        }
    }
}

Mesh::Ptr MeshManager::ProcessAssimpMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector(1.0f); // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec(1.0f);
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // return a mesh object created from the extracted mesh data
    return std::make_shared<Mesh>(vertices, indices);
}

void MeshManager::CreateBuildInSphere()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int SEGMENTS = 128;
    for (unsigned int x = 0; x <= SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)SEGMENTS;
            float ySegment = (float)y / (float)SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * EngineUtility::Math_PI) * std::sin(ySegment * EngineUtility::Math_PI);
            float yPos = std::cos(ySegment * EngineUtility::Math_PI);
            float zPos = std::sin(xSegment * 2.0f * EngineUtility::Math_PI) * std::sin(ySegment * EngineUtility::Math_PI);
            Vertex v;
            v.Position = glm::vec3(xPos, yPos, zPos);
            v.Normal = glm::vec3(xPos, yPos, zPos);
            v.TexCoords = glm::vec2(xSegment, ySegment);
            vertices.push_back(v);
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= SEGMENTS; ++x)
            {
                indices.push_back(y * (SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (SEGMENTS + 1) + x);
                indices.push_back(y * (SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    mesh->SetPrimitive(DrawPrimitive::TriangleStrip);
    m_meshLists["Sphere"] = {mesh};
}

void MeshManager::CreateBuildInCube()
{
    float cubedatas[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                            // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    std::vector<Vertex> vertices;
    for (auto i = 0; i < 36; ++i)
    {
        vertices.push_back(Vertex(glm::vec3(cubedatas[i * 8], cubedatas[i * 8 + 1], cubedatas[i * 8 + 2]),
                                  glm::vec3(cubedatas[i * 8 + 3], cubedatas[i * 8 + 4], cubedatas[i * 8 + 5]),
                                  glm::vec2(cubedatas[i * 8 + 6], cubedatas[i * 8 + 7])));
    }
    auto mesh = std::make_shared<Mesh>(vertices);
    mesh->SetPrimitive(DrawPrimitive::Triangle);
    m_meshLists["Cube"] = {mesh};
}
std::vector<std::string> MeshManager::GetAllMeshListName()
{
    std::vector<std::string> ret;
    for (auto &[key, _] : m_meshLists)
    {
        ret.push_back(key);
    }
    return ret;
}