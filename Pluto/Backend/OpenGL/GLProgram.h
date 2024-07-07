#ifndef PLUTO_OPENGL_GL_PROGRAM_H
#define PLUTO_OPENGL_GL_PROGRAM_H

#include "Backend/RHIBase.h"
#include "backend/BackendEnums.h"
#include "backend/Handle.h"
#include "backend/Program.h"
#include "OpenGL/GLHeaders.h"
#include "OpenGL/GLContext.h"
#include "OpenGL/GLShaderCompiler.h"

#include <array>
#include <limits>
#include <cstddef>
#include <cstdint>

namespace pluto::backend
{
    class OpenGLProgram : public RHIProgram
    {
    public:
        OpenGLProgram() noexcept;
        ~OpenGLProgram() noexcept;

        bool isValid();
        void use();

        struct
        {
            GLuint program = 0;
        } gl;

    private:
        // keep these away from of other class attributes
        struct LazyInitializationData;
        void initialize();
        void initializeProgramState();
        void updateSamplers();
        struct UniformsRecord
        {
        };
        UniformsRecord const *mUniformsRecords = nullptr; // 8 bytes
    };

    static_assert(sizeof(OpenGLProgram) <= 64); // currently 64 bytes

} // namespace pluto::backend

#endif // PLUTO_OPENGL_GL_PROGRAM_H
