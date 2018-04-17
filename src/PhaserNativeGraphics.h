#pragma once

#ifndef USE_MSAA
    #define USE_MSAA 1
#endif

#define NANOVG_GL3_IMPLEMENTATION
#include "PhaserGL.h"
#include <nanovg/nanovg.h>

#include <SDL2/SDL.h>

void PhaserNativeInit();
void PhaserNativeShutdown();

NVGcontext* PhaserNativeCreateNanoVGContext();
void PhaserNativeDestroyNanoVGContext(NVGcontext *vg);

void PhaserNativeBeginFrame();
void PhaserNativeEndFrame();
