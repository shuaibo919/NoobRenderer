#include "Graphics/Backend/Vulkan/Vk.h"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "Graphics/Backend/Vulkan/VKRenderContext.h"
namespace pluto
{
    namespace Graphics
    {
        VKObjectManageByContext::VKObjectManageByContext(VKRenderContext *ctx) : Context(ctx)
        {
            Context->AttachToRenderContext(this);
        }

        void VKObjectManageByContext::DetachFromRenderContext()
        {
            Context->DetachFromRenderContext(this);
        }

        void VKObjectManageByContext::DestroyByContext()
        {
            this->mDestroyedByContext = true;
            this->DestroyImplementation();
        }

        void VKObjectManageByContext::Destroy()
        {
            if (this->mDestroyedByContext)
                return;
            this->DetachFromRenderContext();
            this->DestroyImplementation();
        }
    }
}