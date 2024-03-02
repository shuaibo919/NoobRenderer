#pragma once

#include "Engine/Function/Graphics/Shader.h"

namespace NoobRenderer
{
    /// @brief shader管理:负责创建,管理和删除shader
    class ShaderManager final : public Singleton<ShaderManager>
    {
    private:
        std::vector<ShaderProgram::Ptr> m_shaders;
        std::unordered_map<std::string, ShaderIndex> m_indices;
        ShaderIndex newShaderID{0};

    public:
        ShaderManager() = delete;
        ShaderManager(token);
        ~ShaderManager();
        void Init();

        /**
         * @brief 加载shader(并编译)
         * @param vShaderName 顶点着色器
         * @param fShaderName 片段着色器
         * @return ShaderIndex: the global shader id(unique,用于从管理器取出对应shader)
         */
        ShaderIndex LoadShader(std::string_view vShaderName, std::string_view fShaderName);

        ShaderProgram::Ptr LoadShaderAndGet(std::string_view vShaderName, std::string_view fShaderName);

        /**
         * @brief 加载shader(并编译)
         * @param vShaderName 顶点着色器
         * @param fShaderName 片段着色器
         * @param gShaderName 几何着色器
         * @return ShaderIndex: the global shader id(unique,用于从管理器取出对应shader)
         */
        ShaderIndex LoadShader(std::string_view vShaderName, std::string_view fShaderName, std::string_view gShaderName, bool duplicate = false);

        ShaderProgram::Ptr LoadShaderAndGet(std::string_view vShaderName, std::string_view fShaderName, std::string_view gShaderName, bool duplicate = false);

        /**
         * @brief 取得加载shader
         * @param index: the global shader id
         * @return ShaderProgram(GLuint)
         */
        GLuint GetShaderGLProgram(unsigned int index);

        /**
         * @brief 取得加载shader
         * @param index: the global shader id
         * @return ShaderProgram(class)
         */
        ShaderProgram::Ptr GetShaderProgram(unsigned int index);

        /**
         * @brief 遍历并执行
         * @tparam unaryFunction 独参函数
         * @param func
         */
        using CaptureShaderFunction = std::function<void(ShaderProgram::Ptr)>;
        void Foreach(CaptureShaderFunction func)
        {
            for (auto &_shader : m_shaders)
            {
                func(_shader);
            }
        }

        /**
         * @brief 重编译所有shader
         */
        void RebuildShaders();
    };
}