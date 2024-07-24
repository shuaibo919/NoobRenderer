#pragma once
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"

#define GlCmd(x) GLCommandCall([&]() { GlCall(x); })

#define GlCmdWrap(x) GLCommandCall([&]() { x; })

namespace pluto
{
    namespace Graphics
    {
        namespace OpenGL
        {
            inline static void EmulateCmdRecording(const SharedPtr<CommandBuffer> &cmd, GLCommandCall &&_call)
            {
                std::static_pointer_cast<GLCommandBuffer>(cmd)->EmulateRecording(std::forward<GLCommandCall>(_call));
            }
        }
    }
}