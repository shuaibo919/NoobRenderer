#ifndef PLUTO_OPENGL_GL_SHADER_COMPILER_H
#define PLUTO_OPENGL_GL_SHADER_COMPILER_H

#include "Backend/Program.h"
#include "OpenGL/GLHeaders.h"
#include "OpenGL/GLContext.h"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace pluto::backend
{
    class ShaderCompiler
    {
        struct OpenGLProgramToken;

    public:
        using program_token_t = std::shared_ptr<OpenGLProgramToken>;

        explicit ShaderCompiler();
        // explicit ShaderCompiler(OpenGLDriver &driver);

        ShaderCompiler(ShaderCompiler const &rhs) = delete;
        ShaderCompiler(ShaderCompiler &&rhs) = delete;
        ShaderCompiler &operator=(ShaderCompiler const &rhs) = delete;
        ShaderCompiler &operator=(ShaderCompiler &&rhs) = delete;

        ~ShaderCompiler() noexcept;
        bool isParallelShaderCompileSupported() const noexcept;

        void init() noexcept;
        void terminate() noexcept;
        GLuint getProgram(program_token_t &token);
        void tick();
        static void terminate(program_token_t &token);
        static void setUserData(const program_token_t &token, void *user) noexcept;
        static void *getUserData(const program_token_t &token) noexcept;

    private:
        uint32_t mShaderCompilerThreadCount = 0u;
        GLuint initialize(ShaderCompiler::program_token_t &token) noexcept;
        static void getProgramFromCompilerPool(program_token_t &token) noexcept;
        static void compileShaders();
        static void linkProgram();
        static bool checkProgramStatus(program_token_t const &token) noexcept;
        void executeTickOps() noexcept;
        bool cancelTickOp(program_token_t token) noexcept;
    };

} // namespace pluto::backend

#endif // PLUTO_OPENGL_GL_SHADER_COMPILER_H
