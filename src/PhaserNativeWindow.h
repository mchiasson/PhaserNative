#ifndef PHASERNATIVEWINDOW_H
#define PHASERNATIVEWINDOW_H

#include <string>

#include <SDL2/SDL.h>
#include <nanovg/nanovg.h>

#include "PhaserNativePerfGraph.h"

class PhaserNativeWindow
{
public:

    PhaserNativeWindow();
    ~PhaserNativeWindow();

    void renderStats();
    void swap();

    static SDL_Window *window;
    static SDL_GLContext context;
    static NVGcontext* vg;

private:

    GPUtimer gpuTimer = {};
    PhaserNativeFPSPerfGraph fps;
    PhaserNativeMSPerfGraph cpuGraph;
    PhaserNativeMSPerfGraph gpuGraph;
    PhaserNativeMemPerfGraph cpuMemGraph;
    PhaserNativeMemPerfGraph gpuMemGraph;

    bool fullScreen = false;
    bool resizable = true;
    int width = 1280;
    int height = 720;
    std::string title = "PhaserNative";
    float m_backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    uint64_t pageSize = 0;
    uint64_t m_currentTime = 0;
    uint64_t m_prevTime = 0;
    uint64_t m_deltaTime = 0;

    int fontIcons = 0;
    int fontNormal = 0;
    int fontBold = 0;
    int fontEmoji = 0;

};

#endif // PHASERNATIVEWINDOW_H
