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
    }
}
