#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <nanovg/nanovg.h>

#include "PhaserNativePerfGraph.h"

class PhaserNativeDebugRenderer
{
public:

    PhaserNativeDebugRenderer();
    ~PhaserNativeDebugRenderer();

    void startProfiling();
    void stopProfiling();
    void renderStats();

private:

    SDL_GLContext context = nullptr;
    NVGcontext *vg = nullptr;

    GPUtimer gpuTimer = {};
    PhaserNativeFPSPerfGraph fps;
    PhaserNativeMSPerfGraph cpuGraph;
    PhaserNativeMSPerfGraph gpuGraph;
    PhaserNativeMemPerfGraph cpuMemGraph;
    PhaserNativeMemPerfGraph gpuMemGraph;

    uint64_t pageSize = 0;
    uint64_t m_currentFrameTime = 0;
    uint64_t m_prevFrameTime = 0;
    uint64_t m_deltaFrameTime = 0;

    int fontIcons = 0;
    int fontNormal = 0;
    int fontBold = 0;
    int fontEmoji = 0;
};
