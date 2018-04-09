#include "PhaserNativeGraphics.h"

#include "nanovg/nanovg_gl.h"

#include "JSC/JSCException.h"

#include <thread>

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

    for (int i = 0; window == nullptr && i < 3; ++i)
    {
        SDL_Log("SDL_CreateWindow attempt #%d\n", i+1);
        window = SDL_CreateWindow(nullptr,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  0,
                                  0,
                                  flags);

        if (window == nullptr)
        {
            SDL_LogError(0, "SDL_CreateWindow attempt #%d failed : %s\n", i+1, SDL_GetError());
        }
    }


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

NVGcontext* PhaserNativeCreateNanoVGContext()
{
    NVGcontext *ctx = nullptr;

    int nvg_flags = 0;

    nvg_flags |= NVG_STENCIL_STROKES;

#ifndef USE_MSAA
    nvg_flags |= NVG_ANTIALIAS;
#endif

#ifndef NDEBUG
    nvg_flags |= NVG_DEBUG;
#endif

#ifdef NANOVG_GL2_IMPLEMENTATION
    ctx = nvgCreateGL2(nvg_flags);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    ctx = nvgCreateGL3(nvg_flags);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    ctx = nvgCreateGLES2(nvg_flags);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    ctx = nvgCreateGLES3(nvg_flags);
#endif


    return ctx;
}

void PhaserNativeDestroyNanoVGContext(NVGcontext *ctx)
{
#ifdef NANOVG_GL2_IMPLEMENTATION
    nvgDeleteGL2(ctx);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    nvgDeleteGL3(ctx);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    nvgDeleteGLES2(ctx);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    nvgDeleteGLES3(ctx);
#endif
}
