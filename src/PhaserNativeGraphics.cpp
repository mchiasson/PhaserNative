#include "PhaserNativeGraphics.h"

#include "nanovg/nanovg_gl.h"

#include "JSC/JSCException.h"

#include <thread>

static NVGcontext *_current_vg = nullptr;

SDL_Window* PhaserNativeCreateWindow()
{
    SDL_Window *window = nullptr;

    uint32_t flags =
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_HIDDEN |
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_ALLOW_HIGHDPI;

#ifdef NANOVG_GL2_IMPLEMENTATION
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
#endif

#ifdef USE_MSAA
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("PhaserNative",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              0,
                              0,
                              flags);


    if (window == nullptr)
    {
        throw JSC::Exception(SDL_GetError());
    }

    return window;
}

void PhaserNativeDestroyWindow(SDL_Window *window)
{
    if (SDL_GL_MakeCurrent(nullptr, nullptr) != 0) {
        throw JSC::Exception(SDL_GetError());
    }
    SDL_DestroyWindow(window);
}

SDL_GLContext PhaserNativeCreateGLContext(SDL_Window *window)
{
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        throw JSC::Exception(SDL_GetError());
    }

    static bool needGlInit = true;
    if (needGlInit) {
        phaserGLInit();
        needGlInit = false;
    }

    return context;
}

void PhaserNativeDestroyGLContext(SDL_GLContext context)
{
    if (SDL_GL_MakeCurrent(nullptr, nullptr) != 0) {
        throw JSC::Exception(SDL_GetError());
    }
    SDL_GL_DeleteContext(context);
}

void PhaserNativeMakeCurrent(SDL_Window *window, SDL_GLContext context, NVGcontext *vg)
{
    if (SDL_GL_MakeCurrent(window, context) != 0) { throw JSC::Exception(SDL_GetError()); }
    _current_vg = vg;
}

NVGcontext* PhaserNativeCreateNanoVGContext()
{
    NVGcontext *vg = nullptr;

    int nvg_flags = 0;

    nvg_flags |= NVG_STENCIL_STROKES;

#ifndef USE_MSAA
    nvg_flags |= NVG_ANTIALIAS;
#endif

#ifndef NDEBUG
    nvg_flags |= NVG_DEBUG;
#endif

#ifdef NANOVG_GL2_IMPLEMENTATION
    vg = nvgCreateGL2(nvg_flags);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    vg = nvgCreateGL3(nvg_flags);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    vg = nvgCreateGLES2(nvg_flags);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    vg = nvgCreateGLES3(nvg_flags);
#endif

    _current_vg = vg;

    return vg;
}


void PhaserNativeDestroyNanoVGContext(NVGcontext *vg)
{
    if (_current_vg == vg)
    {
        _current_vg = nullptr;
    }

#ifdef NANOVG_GL2_IMPLEMENTATION
    nvgDeleteGL2(vg);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    nvgDeleteGL3(vg);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    nvgDeleteGLES2(vg);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    nvgDeleteGLES3(vg);
#endif
}

NVGcontext *PhaserNativeGetCurrentNanoVG()
{
    return _current_vg;
}

void PhaserNativeBeginFrame()
{
    if (_current_vg)
    {
        int width, height;
        SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);

        int fbWidth, fbHeight;
        SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &fbWidth, &fbHeight);

        nvgBeginFrame(_current_vg, width, height,  fbWidth / (float) width);
    }
}

void PhaserNativeEndFrame()
{
    if (_current_vg)
    {
        nvgEndFrame(_current_vg);
    }
}
