#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class BufferLayout
        {
        public:
            struct Element
            {
                std::string name;
                RHIFormat format{RHIFormat::R32G32B32A32Float};
                uint32_t offset{0};
                bool normalised{false};
            };
            BufferLayout() : mSize(0) {}

            template <typename T>
            void Push(const std::string &name, bool Normalised = false)
            {
                log<pluto::Error>("BufferLayout::Push<T> is not implemented for type T = {%s}", typeid(T).name());
            }

            inline const std::vector<Element> &GetLayout() const
            {
                return mLayout;
            }
            inline uint32_t GetStride() const
            {
                return mSize;
            }

        private:
            uint32_t mSize;
            std::vector<Element> mLayout;

        private:
            void Push(const std::string &name, RHIFormat format, uint32_t size, bool Normalised)
            {
                mLayout.push_back({name, format, mSize, Normalised});
                mSize += size;
            }
        };

        template <>
        void BufferLayout::Push<float>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R32Float, sizeof(float), Normalised);
        }
        template <>
        void BufferLayout::Push<uint32_t>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R32UInt, sizeof(uint32_t), Normalised);
        }
        template <>
        void BufferLayout::Push<uint8_t>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R8UInt, sizeof(uint8_t), Normalised);
        }
        template <>
        void BufferLayout::Push<glm::vec2>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R32G32Float, sizeof(glm::vec2), Normalised);
        }
        template <>
        void BufferLayout::Push<glm::vec3>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R32G32B32Float, sizeof(glm::vec3), Normalised);
        }
        template <>
        void BufferLayout::Push<glm::vec4>(const std::string &name, bool Normalised)
        {
            Push(name, RHIFormat::R32G32B32A32Float, sizeof(glm::vec4), Normalised);
        }
    }
}
