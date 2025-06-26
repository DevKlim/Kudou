#pragma once

#ifndef GL_API_ENABLED
#define GL_API_ENABLED // Allow using desktop GL.
#endif

#ifndef GLES_API_ENABLED
#define GLES_API_ENABLED // Allow using GLES (ANGLE).
#endif

#ifdef EGL_STATIC
#define KHRONOS_STATIC 1
#include "thirdparty/angle/include/EGL/egl.h"
#include "thirdparty/angle/include/EGL/eglext.h"
#undef KHRONOS_STATIC
#else
#include "thirdparty/glad/glad/egl.h"
#endif
#include "thirdparty/glad/glad/gl.h"
