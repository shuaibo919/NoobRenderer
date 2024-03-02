#pragma once
#include "Engine/NoobEngine.h"
namespace NoobRenderer
{
    namespace inspection
    {
        class ModelPreview
        {
            private:
                const static int m_preview_size = 256;
                inline static std::unordered_map<std::string, WriteToTexture::Ptr> m_preview_results{};
                inline static ShaderProgram::Ptr m_shader{nullptr};
            public:
                static void Init();
                static void Delete(std::string name);
                static void Add(std::string name);
                inline static std::unordered_map<std::string, WriteToTexture::Ptr> &GetAllResults(){ return m_preview_results;};
        };
    }
}