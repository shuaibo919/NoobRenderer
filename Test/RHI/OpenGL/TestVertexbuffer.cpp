#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderContext.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Window.h"
#include "Graphics/Backend/OpenGL/GL.h"
#include "Core/Log.hpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

unsigned int BuildShader();

int main()
{
    using namespace pluto;
    using namespace pluto::Graphics;
    RenderDevice::Init();
    RenderDevice::Create();
    auto ctx = GraphicsContext::Create(RenderAPI::OPENGL, RenderDevice::Get());
    auto window = Window::Create(ctx, 300, 600, "Test");
    ctx->SetMainSwapChain(window->GetSwapChain());
    auto vertexBuffer = VertexBuffer::Builder()
                            .SetVertexData(vertices, 3, sizeof(vertices))
                            .SetUsage(BufferUsage::Static)
                            .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 0, 3 * sizeof(float))
                            .Create(ctx);
    auto shader = BuildShader();

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    while (!window->ShouldClose())
    {
        window->PollEvents();
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shader);

            glBindBuffer(GL_ARRAY_BUFFER, std::static_pointer_cast<GLVertexBuffer>(vertexBuffer)->GetHandle());
            glBindVertexArray(VAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        window->SwapBuffers();
    }
    window->Terminate();
    RenderDevice::Release();

    return 0;
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

unsigned int BuildShader()
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}