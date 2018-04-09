#include "PhaserNativeWindow.h"
#include "PhaserNativeGraphics.h"


#if defined(__linux__)
#include <sys/sysinfo.h>
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#endif

SDL_Window *PhaserNativeWindow::window = nullptr;
SDL_GLContext PhaserNativeWindow::context = nullptr;
SDL_GLContext PhaserNativeWindow::debugContext = nullptr;
NVGcontext* PhaserNativeWindow::vg = nullptr;

PhaserNativeWindow::PhaserNativeWindow() :
    fps("Frame Time"),
    cpuGraph("CPU Time"),
    cpuMemGraph("RSS"),
    gpuMemGraph("Sys GPU Mem")
{
    window = PhaserNativeCreateWindow();
    if (window == nullptr)
    {
        SDL_LogCritical(0, "Window could not be created: %s\n", SDL_GetError() );
        abort();
    }

    debugContext = PhaserNativeCreateGLContext(window);
    if(debugContext == nullptr)
    {
        SDL_LogCritical(0, "GL Context could not be created: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        abort();
    }

    vg = PhaserNativeCreateNanoVGContext();

    fontIcons = nvgCreateFont(vg, "icons", "entypo.ttf");
    if (fontIcons == -1) {
        SDL_LogCritical(0, "Could not add font icons.\n");
        throw -1;
    }
    fontNormal = nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
    if (fontNormal == -1) {
        SDL_LogCritical(0, "Could not add font italic.\n");
        throw -1;
    }
    fontBold = nvgCreateFont(vg, "sans-bold", "Roboto-Bold.ttf");
    if (fontBold == -1) {
        SDL_LogCritical(0, "Could not add font bold.\n");
        throw -1;
    }
    fontEmoji = nvgCreateFont(vg, "emoji", "NotoEmoji-Regular.ttf");
    if (fontEmoji == -1) {
        SDL_LogCritical(0, "Could not add font emoji.\n");
        throw -1;
    }
    nvgAddFallbackFontId(vg, fontNormal, fontEmoji);
    nvgAddFallbackFontId(vg, fontBold, fontEmoji);

#if defined(__linux__)
    {
        struct sysinfo info;
        sysinfo(&info);
        cpuMemGraph.setMaximumValue(info.totalram/1048576.0f);

        pageSize = sysconf(_SC_PAGESIZE);
    }
#endif

    if (phaserGLSupport(GL_NVX_gpu_memory_info))
    {
        int total;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total);
        gpuMemGraph.setMaximumValue(total/1024.0f);
    }

    m_currentTime = SDL_GetPerformanceCounter();

    context = PhaserNativeCreateGLContext(window);

    SDL_GL_SetSwapInterval(1);
}

PhaserNativeWindow::~PhaserNativeWindow()
{
    PhaserNativeDestroyNanoVGContext(vg);
    vg = nullptr;

    phaserGLShutdown();
    SDL_GL_MakeCurrent(NULL, NULL);
    SDL_GL_DeleteContext(debugContext);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void PhaserNativeWindow::renderStats()
{
    m_prevTime = m_currentTime;
    m_currentTime = SDL_GetPerformanceCounter();
    m_deltaTime = m_currentTime - m_prevTime;


    int fbWidth, fbHeight;
    SDL_GL_GetDrawableSize(window, &fbWidth, &fbHeight);

    SDL_GL_MakeCurrent(window, debugContext);

//    glViewport(0, 0, fbWidth, fbHeight);
//    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, width, height,  fbWidth / (float) width);

    int perfGraphCursor = 5;
    fps.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

    cpuGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

#if defined(__linux__)
    cpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;
#endif

    if (phaserGLSupport(GL_NVX_gpu_memory_info)) {
        gpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
        perfGraphCursor += 205;
    }

    nvgEndFrame(vg);

    SDL_GL_MakeCurrent(window, context);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
    uint64_t cpuTime  = (SDL_GetPerformanceCounter() - m_currentTime);

    fps.updateGraph(m_deltaTime / (float)SDL_GetPerformanceFrequency());
    cpuGraph.updateGraph(cpuTime / (float)SDL_GetPerformanceFrequency());

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

    if (phaserGLSupport(GL_NVX_gpu_memory_info))
    {
        int available;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
        gpuMemGraph.updateGraph(gpuMemGraph.getMaximumValue()-(available/1024.0f));
    }

}

void PhaserNativeWindow::swap()
{
    SDL_GL_SwapWindow(window);
}
