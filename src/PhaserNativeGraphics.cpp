#include "PhaserNativeGraphics.h"

#include "nanovg/nanovg_gl.h"

#include "JSC/JSCException.h"

#include <algorithm>
#include <vector>

static SDL_Window *window = nullptr;
static SDL_GLContext context = nullptr;
static  std::vector<NVGcontext*> nvgs;

void PhaserNativeInit()
{
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

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    window = SDL_CreateWindow("PhaserNative",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1280,
                              720,
                              flags);

    if (!window)
    {
        throw JSC::Exception(SDL_GetError());
    }

    context = SDL_GL_CreateContext(window);

    if (!context)
    {
        throw JSC::Exception(SDL_GetError());
    }

    phaserGLInit();

}

void PhaserNativeShutdown()
{
    while(nvgs.size() > 0)
    {
        PhaserNativeDestroyNanoVGContext(nvgs.back());
    }

    SDL_GL_MakeCurrent(nullptr, nullptr);

    SDL_GL_DeleteContext(context);
    context = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;
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

    if (vg)
    {
        nvgs.push_back(vg);
    }

    return vg;
}


void PhaserNativeDestroyNanoVGContext(NVGcontext *vg)
{
    auto it = std::find(nvgs.begin(), nvgs.end(), vg);
    if (it != nvgs.end())
    {
        nvgs.erase(it);
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
}

void PhaserNativeBeginFrame()
{
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    int fbWidth, fbHeight;
    SDL_GL_GetDrawableSize(window, &fbWidth, &fbHeight);

    float devicePixelRatio = width / (float) fbWidth;

    for (size_t i = 0; i < nvgs.size(); ++i)
    {
        nvgBeginFrame(nvgs[i], width, height, devicePixelRatio);
    }
}

void PhaserNativeEndFrame()
{
    for (size_t i = 0; i < nvgs.size(); ++i)
    {
        nvgEndFrame(nvgs[i]);
    }

    SDL_GL_SwapWindow(window);
}
