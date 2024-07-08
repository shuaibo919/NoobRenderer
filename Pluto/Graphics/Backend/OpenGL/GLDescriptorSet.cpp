/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLShader.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLDescriptorSet::GLDescriptorSet(GLDescriptorSet::Properties *&&pProperties)
    : DescriptorSet(std::move(pProperties))
{
}

GLDescriptorSet::~GLDescriptorSet()
{
}
