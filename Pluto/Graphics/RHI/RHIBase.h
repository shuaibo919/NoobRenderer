#pragma once
#include "Core/Base.hpp"

namespace pluto
{
    namespace Graphics
    {
        class RenderContext;
        class RHIBase
        {
            friend class GLRenderContext;
            friend class VKRenderContext;

        public:
            RHIBase(RenderContext *ctx) : mRenderContext(ctx) {}
            virtual ~RHIBase() = default;

        protected:
            RenderContext *mRenderContext{nullptr};
        };

        template <typename T>
        class RHICache
        {
        public:
            struct CacheAssociation
            {
                SharedPtr<T> object{nullptr};
                // float lastAccessedTime{0}; Todo: Add TimeStamp
            };

        public:
            static SharedPtr<T> TryGet(RenderContext *ctx, void *Properties)
            {
                auto pProperties = static_cast<T::Properties *>(Properties);
                auto hash = T::GetHash(*pProperties);
                auto found = mCacheMap.find(hash);
                if (found != mCacheMap.end() && found->second.object != nullptr)
                {
                    return found->second.object;
                }
                return std::make_shared<T>(ctx, new T::Properties(*pProperties));
            }

            static void ClearCache()
            {
                // Todo
                // PLog<PError>("ClearCache Not Implemented");
                mCacheMap.clear();
            }

        private:
            static std::unordered_map<uint64_t, CacheAssociation> mCacheMap;
        };
    }
}
