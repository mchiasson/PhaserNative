#pragma once

#ifndef USE_MSAA
    #define USE_MSAA 1
#endif

#define NANOVG_GL3_IMPLEMENTATION
#include "PhaserGL.h"
#include <nanovg/nanovg.h>

#include <SDL2/SDL.h>

SDL_Window* PhaserNativeCreateWindow();
void PhaserNativeDestroyWindow(SDL_Window *window);

SDL_GLContext PhaserNativeCreateGLContext(SDL_Window* window);
void PhaserNativeDestroyGLContext(SDL_GLContext context);

void PhaserNativeMakeCurrent(SDL_Window *window, SDL_GLContext context, NVGcontext *vg);

NVGcontext* PhaserNativeCreateNanoVGContext();
void PhaserNativeDestroyNanoVGContext(NVGcontext *vg);

NVGcontext *PhaserNativeGetCurrentNanoVG();

void PhaserNativeBeginFrame();
void PhaserNativeEndFrame();
