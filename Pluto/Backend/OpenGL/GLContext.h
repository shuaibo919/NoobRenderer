/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PLUTO_OPENGL_GL_CONTEXT_H
#define PLUTO_OPENGL_GL_CONTEXT_H

#include "Backend/BackendEnums.h"
#include "Backend/Handle.h"
#include "OpenGL/GLHeaders.h"

#include <array>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include <stddef.h>
#include <stdint.h>

namespace pluto::backend
{

    class OpenGLContext
    {
    public:
        static constexpr const size_t MAX_TEXTURE_UNIT_COUNT = 32;
        static constexpr const size_t MAX_BUFFER_BINDINGS = 32;

        struct RenderPrimitive
        {
            GLuint vao[2] = {};      // 4
            GLuint elementArray = 0; // 4
            // mutable utils::bitset<uint16_t> vertexAttribArray; // 2

            // if this differs from vertexBufferWithObjects->bufferObjectsVersion, this VAO needs to
            // be updated (see OpenGLDriver::updateVertexArrayObject())
            uint8_t vertexBufferVersion = 0; // 1

            // if this differs from OpenGLContext::state.age, this VAO needs to
            // be updated (see OpenGLDriver::updateVertexArrayObject())
            uint8_t stateVersion = 0; // 1

            // If this differs from OpenGLContext::state.age, this VAO's name needs to be updated.
            // See OpenGLContext::bindVertexArray()
            uint8_t nameVersion = 0; // 1

            // Size in bytes of indices in the index buffer
            uint8_t indicesSize = 0; // 1

            // The optional 32-bit handle to a GLVertexBuffer is necessary only if the referenced
            // VertexBuffer supports buffer objects. If this is zero, then the VBO handles array is
            // immutable.
            Handle<RHIVertexBuffer> vertexBufferWithObjects; // 4

            GLenum getIndicesType() const noexcept
            {
                return indicesSize == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
            }
        } gl;

        static bool queryOpenGLVersion(GLint *major, GLint *minor) noexcept;

        explicit OpenGLContext() noexcept;

        ~OpenGLContext() noexcept final;

        void terminate() noexcept;

        // TimerQueryInterface ------------------------------------------------------------------------

        // --------------------------------------------------------------------------------------------
        constexpr inline size_t getIndexForCap(GLenum cap) noexcept;
        constexpr static inline size_t getIndexForBufferTarget(GLenum target) noexcept;

        void resetState() noexcept;

        inline void useProgram(GLuint program) noexcept;

        void pixelStore(GLenum, GLint) noexcept;
        inline void activeTexture(GLuint unit) noexcept;
        inline void bindTexture(GLuint unit, GLuint target, GLuint texId) noexcept;

        void unbindTexture(GLenum target, GLuint id) noexcept;
        void unbindTextureUnit(GLuint unit) noexcept;
        inline void bindVertexArray(RenderPrimitive const *p) noexcept;
        inline void bindSampler(GLuint unit, GLuint sampler) noexcept;
        void unbindSampler(GLuint sampler) noexcept;

        void bindBuffer(GLenum target, GLuint buffer) noexcept;
        inline void bindBufferRange(GLenum target, GLuint index, GLuint buffer,
                                    GLintptr offset, GLsizeiptr size) noexcept;

        GLuint bindFramebuffer(GLenum target, GLuint buffer) noexcept;
        void unbindFramebuffer(GLenum target) noexcept;

        inline void enableVertexAttribArray(RenderPrimitive const *rp, GLuint index) noexcept;
        inline void disableVertexAttribArray(RenderPrimitive const *rp, GLuint index) noexcept;
        inline void enable(GLenum cap) noexcept;
        inline void disable(GLenum cap) noexcept;
        inline void frontFace(GLenum mode) noexcept;
        inline void cullFace(GLenum mode) noexcept;
        inline void blendEquation(GLenum modeRGB, GLenum modeA) noexcept;
        inline void blendFunction(GLenum srcRGB, GLenum srcA, GLenum dstRGB, GLenum dstA) noexcept;
        inline void colorMask(GLboolean flag) noexcept;
        inline void depthMask(GLboolean flag) noexcept;
        inline void depthFunc(GLenum func) noexcept;
        inline void stencilFuncSeparate(GLenum funcFront, GLint refFront, GLuint maskFront,
                                        GLenum funcBack, GLint refBack, GLuint maskBack) noexcept;
        inline void stencilOpSeparate(GLenum sfailFront, GLenum dpfailFront, GLenum dppassFront,
                                      GLenum sfailBack, GLenum dpfailBack, GLenum dppassBack) noexcept;
        inline void stencilMaskSeparate(GLuint maskFront, GLuint maskBack) noexcept;
        inline void polygonOffset(GLfloat factor, GLfloat units) noexcept;

        inline void setScissor(GLint left, GLint bottom, GLsizei width, GLsizei height) noexcept;
        inline void viewport(GLint left, GLint bottom, GLsizei width, GLsizei height) noexcept;
        inline void depthRange(GLclampf near, GLclampf far) noexcept;

        void deleteBuffers(GLsizei n, const GLuint *buffers, GLenum target) noexcept;
        void deleteVertexArray(GLuint vao) noexcept;

        void destroyWithContext(size_t index, std::function<void(OpenGLContext &)> const &closure) noexcept;

        // function to handle state changes we don't control
        void updateTexImage(GLenum target, GLuint id) noexcept;

        // This is the index of the context in use. Must be 0 or 1. This is used to manange the
        // OpenGL name of ContainerObjects within each context.
        uint32_t contextIndex = 0;

        void unbindEverything() noexcept;
        void synchronizeStateAndCache(size_t index) noexcept;

    private:
        std::vector<std::function<void(OpenGLContext &)>> mDestroyWithNormalContext;
        RenderPrimitive mDefaultVAO;
        std::optional<GLuint> mDefaultFbo[2];
        std::array<
            std::tuple<GLuint, void const *, uint16_t>,
            CONFIG_UNIFORM_BINDING_COUNT>
            mUniformBindings = {};
        mutable tsl::robin_map<SamplerParams, GLuint,
                               SamplerParams::Hasher, SamplerParams::EqualTo>
            mSamplerMap;

        void bindFramebufferResolved(GLenum target, GLuint buffer) noexcept;

        template <typename T, typename F>
        static inline void update_state(T &state, T const &expected, F functor, bool force = false) noexcept
        {
            if (force || state != expected)
                [unlikely]
                {
                    state = expected;
                    functor();
                }
        }

        void setDefaultState() noexcept;
    };

    // ------------------------------------------------------------------------------------------------

    constexpr size_t OpenGLContext::getIndexForCap(GLenum cap) noexcept
    { // NOLINT
        size_t index = 0;
        switch (cap)
        {
        case GL_BLEND:
            index = 0;
            break;
        case GL_CULL_FACE:
            index = 1;
            break;
        case GL_SCISSOR_TEST:
            index = 2;
            break;
        case GL_DEPTH_TEST:
            index = 3;
            break;
        case GL_STENCIL_TEST:
            index = 4;
            break;
        case GL_DITHER:
            index = 5;
            break;
        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            index = 6;
            break;
        case GL_SAMPLE_COVERAGE:
            index = 7;
            break;
        case GL_POLYGON_OFFSET_FILL:
            index = 8;
            break;
#ifdef GL_ARB_seamless_cube_map
        case GL_TEXTURE_CUBE_MAP_SEAMLESS:
            index = 9;
            break;
#endif
        case GL_PROGRAM_POINT_SIZE:
            index = 10;
            break;
        default:
            break;
        }
        assert_invariant(index < state.enables.caps.size());
        return index;
    }

    constexpr size_t OpenGLContext::getIndexForBufferTarget(GLenum target) noexcept
    {
        size_t index = 0;
        switch (target)
        {
            // The indexed buffers MUST be first in this list (those usable with bindBufferRange)

        case GL_UNIFORM_BUFFER:
            index = 0;
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            index = 1;
            break;

        case GL_SHADER_STORAGE_BUFFER:
            index = 2;
            break;

        case GL_ARRAY_BUFFER:
            index = 3;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            index = 4;
            break;

        case GL_PIXEL_PACK_BUFFER:
            index = 5;
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            index = 6;
            break;

        default:
            break;
        }
        return index;
    }

    // ------------------------------------------------------------------------------------------------

    void OpenGLContext::activeTexture(GLuint unit) noexcept
    {
        assert_invariant(unit < MAX_TEXTURE_UNIT_COUNT);
        update_state(state.textures.active, unit, [&]()
                     { glActiveTexture(GL_TEXTURE0 + unit); });
    }

    void OpenGLContext::bindSampler(GLuint unit, GLuint sampler) noexcept
    {
        assert_invariant(unit < MAX_TEXTURE_UNIT_COUNT);
        assert_invariant(mFeatureLevel >= FeatureLevel::FEATURE_LEVEL_1);
#ifndef FILAMENT_SILENCE_NOT_SUPPORTED_BY_ES2
        update_state(state.textures.units[unit].sampler, sampler, [&]()
                     { glBindSampler(unit, sampler); });
#endif
    }

    void OpenGLContext::setScissor(GLint left, GLint bottom, GLsizei width, GLsizei height) noexcept
    {
        vec4gli const scissor(left, bottom, width, height);
        update_state(state.window.scissor, scissor, [&]()
                     { glScissor(left, bottom, width, height); });
    }

    void OpenGLContext::viewport(GLint left, GLint bottom, GLsizei width, GLsizei height) noexcept
    {
        vec4gli const viewport(left, bottom, width, height);
        update_state(state.window.viewport, viewport, [&]()
                     { glViewport(left, bottom, width, height); });
    }

    void OpenGLContext::depthRange(GLclampf near, GLclampf far) noexcept
    {
        vec2glf const depthRange(near, far);
        update_state(state.window.depthRange, depthRange, [&]()
                     { glDepthRangef(near, far); });
    }

    void OpenGLContext::bindVertexArray(RenderPrimitive const *p) noexcept
    {
        RenderPrimitive *vao = p ? const_cast<RenderPrimitive *>(p) : &mDefaultVAO;
        update_state(state.vao.p, vao, [&]()
                     {

        // See if we need to create a name for this VAO on the fly, this would happen if:
        // - we're not the default VAO, because its name is always 0
        // - our name is 0, this could happen if this VAO was created in the "other" context
        // - the nameVersion is out of date *and* we're on the protected context, in this case:
        //      - the name must be stale from a previous use of this context because we always
        //        destroy the protected context when we're done with it.
        bool const recreateVaoName = p != &mDefaultVAO &&
                ((vao->vao[contextIndex] == 0) ||
                        (vao->nameVersion != state.age && contextIndex == 1));
        if (UTILS_UNLIKELY(recreateVaoName)) {
            vao->nameVersion = state.age;
            procs.genVertexArrays(1, &vao->vao[contextIndex]);
        }

        procs.bindVertexArray(vao->vao[contextIndex]);
        // update GL_ELEMENT_ARRAY_BUFFER, which is updated by glBindVertexArray
        size_t const targetIndex = getIndexForBufferTarget(GL_ELEMENT_ARRAY_BUFFER);
        state.buffers.genericBinding[targetIndex] = vao->elementArray;
        if (UTILS_UNLIKELY(bugs.vao_doesnt_store_element_array_buffer_binding || recreateVaoName)) {
            // This shouldn't be needed, but it looks like some drivers don't do the implicit
            // glBindBuffer().
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->elementArray);
        } });
    }

    void OpenGLContext::bindBufferRange(GLenum target, GLuint index, GLuint buffer,
                                        GLintptr offset, GLsizeiptr size) noexcept
    {
        assert_invariant(mFeatureLevel >= FeatureLevel::FEATURE_LEVEL_1);

#ifndef FILAMENT_SILENCE_NOT_SUPPORTED_BY_ES2
#ifdef BACKEND_OPENGL_LEVEL_GLES31
        assert_invariant(false || target == GL_UNIFORM_BUFFER || target == GL_TRANSFORM_FEEDBACK_BUFFER || target == GL_SHADER_STORAGE_BUFFER);
#else
        assert_invariant(false || target == GL_UNIFORM_BUFFER || target == GL_TRANSFORM_FEEDBACK_BUFFER);
#endif
        size_t const targetIndex = getIndexForBufferTarget(target);
        // this ALSO sets the generic binding
        assert_invariant(targetIndex < sizeof(state.buffers.targets) / sizeof(*state.buffers.targets));
        if (state.buffers.targets[targetIndex].buffers[index].name != buffer || state.buffers.targets[targetIndex].buffers[index].offset != offset || state.buffers.targets[targetIndex].buffers[index].size != size)
        {
            state.buffers.targets[targetIndex].buffers[index].name = buffer;
            state.buffers.targets[targetIndex].buffers[index].offset = offset;
            state.buffers.targets[targetIndex].buffers[index].size = size;
            state.buffers.genericBinding[targetIndex] = buffer;
            glBindBufferRange(target, index, buffer, offset, size);
        }
#endif
    }

    void OpenGLContext::bindTexture(GLuint unit, GLuint target, GLuint texId) noexcept
    {
        update_state(state.textures.units[unit].target, target, [&]()
                     {
        activeTexture(unit);
        glBindTexture(state.textures.units[unit].target, 0); });
        update_state(state.textures.units[unit].id, texId, [&]()
                     {
        activeTexture(unit);
        glBindTexture(target, texId); }, target == GL_TEXTURE_EXTERNAL_OES);
    }

    void OpenGLContext::useProgram(GLuint program) noexcept
    {
        update_state(state.program.use, program, [&]()
                     { glUseProgram(program); });
    }

    void OpenGLContext::enableVertexAttribArray(RenderPrimitive const *rp, GLuint index) noexcept
    {
        assert_invariant(rp);
        assert_invariant(index < rp->vertexAttribArray.size());
        bool const force = rp->stateVersion != state.age;
        if (UTILS_UNLIKELY(force || !rp->vertexAttribArray[index]))
        {
            rp->vertexAttribArray.set(index);
            glEnableVertexAttribArray(index);
        }
    }

    void OpenGLContext::disableVertexAttribArray(RenderPrimitive const *rp, GLuint index) noexcept
    {
        assert_invariant(rp);
        assert_invariant(index < rp->vertexAttribArray.size());
        bool const force = rp->stateVersion != state.age;
        if (UTILS_UNLIKELY(force || rp->vertexAttribArray[index]))
        {
            rp->vertexAttribArray.unset(index);
            glDisableVertexAttribArray(index);
        }
    }

    void OpenGLContext::enable(GLenum cap) noexcept
    {
        size_t const index = getIndexForCap(cap);
        if (UTILS_UNLIKELY(!state.enables.caps[index]))
        {
            state.enables.caps.set(index);
            glEnable(cap);
        }
    }

    void OpenGLContext::disable(GLenum cap) noexcept
    {
        size_t const index = getIndexForCap(cap);
        if (UTILS_UNLIKELY(state.enables.caps[index]))
        {
            state.enables.caps.unset(index);
            glDisable(cap);
        }
    }

    void OpenGLContext::frontFace(GLenum mode) noexcept
    {
        update_state(state.raster.frontFace, mode, [&]()
                     { glFrontFace(mode); });
    }

    void OpenGLContext::cullFace(GLenum mode) noexcept
    {
        update_state(state.raster.cullFace, mode, [&]()
                     { glCullFace(mode); });
    }

    void OpenGLContext::blendEquation(GLenum modeRGB, GLenum modeA) noexcept
    {
        if (UTILS_UNLIKELY(
                state.raster.blendEquationRGB != modeRGB || state.raster.blendEquationA != modeA))
        {
            state.raster.blendEquationRGB = modeRGB;
            state.raster.blendEquationA = modeA;
            glBlendEquationSeparate(modeRGB, modeA);
        }
    }

    void OpenGLContext::blendFunction(GLenum srcRGB, GLenum srcA, GLenum dstRGB, GLenum dstA) noexcept
    {
        if (UTILS_UNLIKELY(
                state.raster.blendFunctionSrcRGB != srcRGB ||
                state.raster.blendFunctionSrcA != srcA ||
                state.raster.blendFunctionDstRGB != dstRGB ||
                state.raster.blendFunctionDstA != dstA))
        {
            state.raster.blendFunctionSrcRGB = srcRGB;
            state.raster.blendFunctionSrcA = srcA;
            state.raster.blendFunctionDstRGB = dstRGB;
            state.raster.blendFunctionDstA = dstA;
            glBlendFuncSeparate(srcRGB, dstRGB, srcA, dstA);
        }
    }

    void OpenGLContext::colorMask(GLboolean flag) noexcept
    {
        update_state(state.raster.colorMask, flag, [&]()
                     { glColorMask(flag, flag, flag, flag); });
    }
    void OpenGLContext::depthMask(GLboolean flag) noexcept
    {
        update_state(state.raster.depthMask, flag, [&]()
                     { glDepthMask(flag); });
    }

    void OpenGLContext::depthFunc(GLenum func) noexcept
    {
        update_state(state.raster.depthFunc, func, [&]()
                     { glDepthFunc(func); });
    }

    void OpenGLContext::stencilFuncSeparate(GLenum funcFront, GLint refFront, GLuint maskFront,
                                            GLenum funcBack, GLint refBack, GLuint maskBack) noexcept
    {
        update_state(state.stencil.front.func, {funcFront, refFront, maskFront}, [&]()
                     { glStencilFuncSeparate(GL_FRONT, funcFront, refFront, maskFront); });
        update_state(state.stencil.back.func, {funcBack, refBack, maskBack}, [&]()
                     { glStencilFuncSeparate(GL_BACK, funcBack, refBack, maskBack); });
    }

    void OpenGLContext::stencilOpSeparate(GLenum sfailFront, GLenum dpfailFront, GLenum dppassFront,
                                          GLenum sfailBack, GLenum dpfailBack, GLenum dppassBack) noexcept
    {
        update_state(state.stencil.front.op, {sfailFront, dpfailFront, dppassFront}, [&]()
                     { glStencilOpSeparate(GL_FRONT, sfailFront, dpfailFront, dppassFront); });
        update_state(state.stencil.back.op, {sfailBack, dpfailBack, dppassBack}, [&]()
                     { glStencilOpSeparate(GL_BACK, sfailBack, dpfailBack, dppassBack); });
    }

    void OpenGLContext::stencilMaskSeparate(GLuint maskFront, GLuint maskBack) noexcept
    {
        update_state(state.stencil.front.stencilMask, maskFront, [&]()
                     { glStencilMaskSeparate(GL_FRONT, maskFront); });
        update_state(state.stencil.back.stencilMask, maskBack, [&]()
                     { glStencilMaskSeparate(GL_BACK, maskBack); });
    }

    void OpenGLContext::polygonOffset(GLfloat factor, GLfloat units) noexcept
    {
        update_state(state.polygonOffset, {factor, units}, [&]()
                     {
        if (factor != 0 || units != 0) {
            glPolygonOffset(factor, units);
            enable(GL_POLYGON_OFFSET_FILL);
        } else {
            disable(GL_POLYGON_OFFSET_FILL);
        } });
    }

} // namespace filament

#endif // PLUTO_OPENGL_CONTEXT_H
