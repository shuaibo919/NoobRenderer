#include "Graphics/RHI/BufferLayout.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

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