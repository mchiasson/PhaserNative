#include "PhaserNativeDebugRenderer.h"
#include "PhaserNativeGraphics.h"
#include "JSC/JSCException.h"


#if defined(__linux__)
#include <sys/sysinfo.h>
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#endif

#define DRAW_GPU_TIME 0
#define DRAW_GPU_MEMORY 0

PhaserNativeDebugRenderer::PhaserNativeDebugRenderer() :
    fps("Frame Time"),
    cpuGraph("CPU Time"),
    gpuGraph("GPU Time"),
    cpuMemGraph("RSS"),
    gpuMemGraph("Sys GPU Mem")
{

#if defined(__linux__)
    {
        struct sysinfo info;
        sysinfo(&info);
        cpuMemGraph.setMaximumValue(info.totalram/1048576.0f);

        pageSize = sysconf(_SC_PAGESIZE);
    }
#endif

#if DRAW_GPU_MEMORY
    if (phaserGLSupport(GL_NVX_gpu_memory_info))
    {
        int total;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total);
        gpuMemGraph.setMaximumValue(total/1024.0f);
    }
#endif

    m_currentFrameTime = SDL_GetPerformanceCounter();
}

PhaserNativeDebugRenderer::~PhaserNativeDebugRenderer()
{
    PhaserNativeDestroyNanoVGContext(vg);
}

void PhaserNativeDebugRenderer::startProfiling() {
#if DRAW_GPU_TIME
    startGPUTimer(&gpuTimer);
#endif
    m_prevFrameTime = m_currentFrameTime;
    m_currentFrameTime = SDL_GetPerformanceCounter();
    m_deltaFrameTime = m_currentFrameTime - m_prevFrameTime;
}


void PhaserNativeDebugRenderer::stopProfiling() {

    // Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
    uint64_t cpuTime  = (SDL_GetPerformanceCounter() - m_currentFrameTime);

    fps.updateGraph(m_deltaFrameTime / (float)SDL_GetPerformanceFrequency());
    cpuGraph.updateGraph(cpuTime / (float)SDL_GetPerformanceFrequency());

#if DRAW_GPU_TIME
    // We may get multiple results.
    float gpuTimes[3];
    int n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
    for (int i = 0; i < n; i++) {
        gpuGraph.updateGraph(gpuTimes[i]);
    }
#endif

#if defined(__linux__)
    {
        struct statm_t {
            unsigned long size,resident,share,text,lib,data,dt;
        } result;

        const char* statm_path = "/proc/self/statm";

        FILE *f = fopen(statm_path,"r");
        if(!f){
            perror(statm_path);
            abort();
        }
        if(7 != fscanf(f,"%ld %ld %ld %ld %ld %ld %ld", &result.size,&result.resident,&result.share,&result.text,&result.lib,&result.data,&result.dt))
        {
            perror(statm_path);
            abort();
        }
        fclose(f);

        cpuMemGraph.updateGraph(result.resident*pageSize/1048576.0f);
    }
#endif

#if DRAW_GPU_MEMORY
    if (phaserGLSupport(GL_NVX_gpu_memory_info))
    {
        int available;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
        gpuMemGraph.updateGraph(gpuMemGraph.getMaximumValue()-(available/1024.0f));
    }
#endif
}

void PhaserNativeDebugRenderer::renderStats()
{
    if (!vg)
    {
        vg = PhaserNativeCreateNanoVGContext();

        fontIcons = nvgCreateFont(vg, "icons", "entypo.ttf");
        if (fontIcons == -1) {
            throw JSC::Exception("Could not add font icons.");
        }

        fontNormal = nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
        if (fontNormal == -1) {
            throw JSC::Exception("Could not add font italic.");
        }

        fontBold = nvgCreateFont(vg, "sans-bold", "Roboto-Bold.ttf");
        if (fontBold == -1) {
            throw JSC::Exception("Could not add font bold.");
        }

        fontEmoji = nvgCreateFont(vg, "emoji", "NotoEmoji-Regular.ttf");
        if (fontEmoji == -1) {
            throw JSC::Exception("Could not add font emoji.");
        }

        nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
        nvgAddFallbackFontId(vg, fontBold, fontEmoji);

#if DRAW_GPU_TIME
        initGPUTimer(&gpuTimer);
#endif
    }

    int perfGraphCursor = 5;
    fps.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

    cpuGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

#if DRAW_GPU_TIME
    if (gpuTimer.supported) {
        gpuGraph.renderGraph(vg, perfGraphCursor, 5);
        perfGraphCursor += 205;
    }
#endif

#if defined(__linux__)
    cpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;
#endif

#if DRAW_GPU_MEMORY
    if (phaserGLSupport(GL_NVX_gpu_memory_info)) {
        gpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
        perfGraphCursor += 205;
    }
#endif

}
