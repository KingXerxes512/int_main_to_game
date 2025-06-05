#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "opengl/glext.h"
#include "opengl/wglext.h"

#define FOR_OPENGL_FUNCTIONS(DO)                                                                                       \
    DO(::PFNGLCREATESHADERPROC, glCreateShader)                                                                        \
    DO(::PFNGLDELETESHADERPROC, glDeleteShader)                                                                        \
    DO(::PFNGLSHADERSOURCEPROC, glShaderSource)                                                                        \
    DO(::PFNGLCOMPILESHADERPROC, glCompileShader)                                                                      \
    DO(::PFNGLGETSHADERIVPROC, glGetShaderiv)                                                                          \
    DO(::PFNGLCREATEPROGRAMPROC, glCreateProgram)                                                                      \
    DO(::PFNGLDELETEPROGRAMPROC, glDeleteProgram)                                                                      \
    DO(::PFNGLATTACHSHADERPROC, glAttachShader)                                                                        \
    DO(::PFNGLLINKPROGRAMPROC, glLinkProgram)                                                                          \
    DO(::PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                                                                  \
    DO(::PFNGLGENBUFFERSPROC, glGenBuffers)                                                                            \
    DO(::PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                                                                  \
    DO(::PFNGLBINDBUFFERPROC, glBindBuffer)                                                                            \
    DO(::PFNGLBUFFERDATAPROC, glBufferData)                                                                            \
    DO(::PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)                                                          \
    DO(::PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)                                                  \
    DO(::PFNGLUSEPROGRAMPROC, glUseProgram)                                                                            \
    DO(::PFNGLDRAWARRAYSEXTPROC, glDrawArraysEXT)                                                                      \
    DO(::PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)                                                            \
    DO(::PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                                                                      \
    DO(::PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback)                                                        \
    DO(::PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)                                                            \
    DO(::PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                                                                \
    DO(::PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl)                                                          \
    DO(::PFNGLCREATEBUFFERSPROC, glCreateBuffers)                                                                      \
    DO(::PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage)                                                            \
    DO(::PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays)                                                            \
    DO(::PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC, glVertexArrayVertexBuffer)                                           \
    DO(::PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib)                                                  \
    DO(::PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat)                                                  \
    DO(::PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding)                                                \
    DO(::PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData)                                                            \
    DO(::PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer)                                                \
    DO(::PFNGLBINDBUFFERBASEPROC, glBindBufferBase)                                                                    \
    DO(::PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                                                                \
    DO(::PFNGLCREATETEXTURESPROC, glCreateTextures)                                                                    \
    DO(::PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D)                                                                \
    DO(::PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D)                                                              \
    DO(::PFNGLCREATESAMPLERSPROC, glCreateSamplers)                                                                    \
    DO(::PFNGLDELETESAMPLERSPROC, glDeleteSamplers)                                                                    \
    DO(::PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit)                                                                  \
    DO(::PFNGLBINDSAMPLERPROC, glBindSampler)                                                                          \
    DO(::PFNGLUNIFORM1IPROC, glUniform1i)                                                                              \
    DO(::PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                                                        \
    DO(::PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                                                              \
    DO(::PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT)

#define DO_DEFINE(TYPE, NAME) inline TYPE NAME;
FOR_OPENGL_FUNCTIONS(DO_DEFINE)
