#include "PhaserNativeWindow.h"

#ifndef USE_MSAA
    #define USE_MSAA 1
#endif

#include "PhaserGL.h"

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>

#if defined(__linux__)
#include <sys/sysinfo.h>
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#endif

SDL_Window *PhaserNativeWindow::window = nullptr;
SDL_GLContext PhaserNativeWindow::context = nullptr;
NVGcontext* PhaserNativeWindow::vg = nullptr;

PhaserNativeWindow::PhaserNativeWindow() :
    fps("Frame Time"),
    cpuGraph("CPU Time"),
    gpuGraph("GPU Time"),
    cpuMemGraph("RSS"),
    gpuMemGraph("Sys GPU Mem")
{
#ifndef NDEBUG
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
#endif

    int rc = 0;
    if((rc = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER)) < 0 )
    {
        SDL_LogError(0, "SDL could not initialize: %s\n", SDL_GetError() );
        throw rc;
    }

//    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI;
    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

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

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              flags);
    if (window == nullptr)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL,
                       "Window could not be created: %s\n", SDL_GetError() );
        abort();
    }

    context = SDL_GL_CreateContext(window);
    if(context == nullptr)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL,
                       "GL Context could not be created: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        abort();
    }

    SDL_GL_MakeCurrent(window, context);

    phaserGLInit();

    int nvg_flags = 0;

    nvg_flags |= NVG_STENCIL_STROKES;

#ifndef USE_MSAA
    nvg_flags |= NVG_ANTIALIAS;
#endif

#ifndef NDEBUG
    nvg_flags |= NVG_DEBUG;
#endif

#ifdef USE_MSAA
    #ifdef NANOVG_GL2_IMPLEMENTATION
        vg = nvgCreateGL2(nvg_flags);
    #elif defined(NANOVG_GL3_IMPLEMENTATION)
        vg = nvgCreateGL3(nvg_flags);
    #elif defined(NANOVG_GLES2_IMPLEMENTATION)
        vg = nvgCreateGLES2(nvg_flags);
    #elif defined(NANOVG_GLES3_IMPLEMENTATION)
        vg = nvgCreateGLES3(nvg_flags);
    #endif
#else
    #ifdef NANOVG_GL2_IMPLEMENTATION
        vg = nvgCreateGL2(nvg_flags);
    #elif defined(NANOVG_GL3_IMPLEMENTATION)
        vg = nvgCreateGL3(nvg_flags);
    #elif defined(NANOVG_GLES2_IMPLEMENTATION)
        vg = nvgCreateGLES2(nvg_flags);
    #elif defined(NANOVG_GLES3_IMPLEMENTATION)
        vg = nvgCreateGLES3(nvg_flags);
    #endif
#endif

    SDL_GL_SetSwapInterval(1);
    initGPUTimer(&gpuTimer);

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
}

PhaserNativeWindow::~PhaserNativeWindow()
{
#ifdef NANOVG_GL2_IMPLEMENTATION
    nvgDeleteGL2(vg);
#elif defined(NANOVG_GL3_IMPLEMENTATION)
    nvgDeleteGL3(vg);
#elif defined(NANOVG_GLES2_IMPLEMENTATION)
    nvgDeleteGLES2(vg);
#elif defined(NANOVG_GLES3_IMPLEMENTATION)
    nvgDeleteGLES3(vg);
#endif

    phaserGLShutdown();
    SDL_GL_MakeCurrent(NULL, NULL);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void PhaserNativeWindow::render()
{
    m_prevTime = m_currentTime;
    m_currentTime = SDL_GetPerformanceCounter();
    m_deltaTime = m_currentTime - m_prevTime;

    float gpuTimes[3];

    int fbWidth, fbHeight;
    SDL_GL_GetDrawableSize(window, &fbWidth, &fbHeight);

    startGPUTimer(&gpuTimer);

    SDL_GL_MakeCurrent(window, context);

    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, width, height,  fbWidth / (float) width);

    int perfGraphCursor = 5;
    fps.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

    cpuGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;

    if (gpuTimer.supported) {
        gpuGraph.renderGraph(vg, perfGraphCursor, 5);
        perfGraphCursor += 205;
    }

#if defined(__linux__)
    cpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
    perfGraphCursor += 205;
#endif

    if (phaserGLSupport(GL_NVX_gpu_memory_info)) {
        gpuMemGraph.renderGraph(vg, perfGraphCursor, 5);
        perfGraphCursor += 205;
    }

    nvgEndFrame(vg);

    // Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
    uint64_t cpuTime  = (SDL_GetPerformanceCounter() - m_currentTime);

    fps.updateGraph(m_deltaTime / (float)SDL_GetPerformanceFrequency());
    cpuGraph.updateGraph(cpuTime / (float)SDL_GetPerformanceFrequency());

    // We may get multiple results.
    int n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
    for (int i = 0; i < n; i++) {
        gpuGraph.updateGraph(gpuTimes[i]);
    }

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

    SDL_GL_SwapWindow(window);
}
