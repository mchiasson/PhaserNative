#include "PhaserNativePerfGraph.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "PhaserGL.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#elif !defined(__MINGW32__)
#include <iconv.h>
#endif

void initGPUTimer(GPUtimer* timer)
{
    memset(timer, 0, sizeof(*timer));

    timer->supported = phaserGLSupport(GL_ARB_timer_query);
    if (timer->supported) {
        glGenQueries(GPU_QUERY_COUNT, timer->queries);
    }
}

void startGPUTimer(GPUtimer* timer)
{
    if (!timer->supported)
        return;
    glBeginQuery(GL_TIME_ELAPSED, timer->queries[timer->cur % GPU_QUERY_COUNT] );
    timer->cur++;
}

int stopGPUTimer(GPUtimer* timer, float* times, int maxTimes)
{
    NVG_NOTUSED(times);
    NVG_NOTUSED(maxTimes);
    GLint available = 1;
    int n = 0;
    if (!timer->supported)
        return 0;

    glEndQuery(GL_TIME_ELAPSED);
    while (available && timer->ret <= timer->cur) {
        // check for results if there are any
        glGetQueryObjectiv(timer->queries[timer->ret % GPU_QUERY_COUNT], GL_QUERY_RESULT_AVAILABLE, &available);
        if (available) {
            GLuint64 timeElapsed = 0;
            glGetQueryObjectui64v(timer->queries[timer->ret % GPU_QUERY_COUNT], GL_QUERY_RESULT, &timeElapsed);
            timer->ret++;
            if (n < maxTimes) {
                times[n] = (float)((double)timeElapsed * 1e-9);
                n++;
            }
        }
    }
    return n;
}

PhaserNativePerfGraph::PhaserNativePerfGraph(const char *title, Style style, float maximumValue) :
    title(title),
    style(style),
    maximumValue(maximumValue),
    currentIndex(0)
{
    memset(values, 0, GRAPH_HISTORY_COUNT * sizeof(float));
}

void PhaserNativePerfGraph::updateGraph(float frameTime)
{
    currentIndex = (currentIndex+1) % GRAPH_HISTORY_COUNT;
    values[currentIndex] = frameTime;
}

float PhaserNativePerfGraph::getGraphAverage() const
{
    int i;
    float avg = 0;
    for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
        avg += values[i];
    }
    return avg / (float)GRAPH_HISTORY_COUNT;
}

void PhaserNativePerfGraph::renderGraph(NVGcontext* vg, float x, float y)
{
    int i;
    char str[64];

    float avg = getGraphAverage();

    const float w = 200;
    const float h = 35;

    nvgBeginPath(vg);
    nvgRect(vg, x,y, w,h);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, x, y+h);

    switch(style)
    {
    case STYLE_FPS: {
        for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
            float v = values[(currentIndex+i) % GRAPH_HISTORY_COUNT];
            if (v > 0.01) v = 1.0f / v;
            float vx, vy;
            if (v > maximumValue) maximumValue = v; // automatically increase the maximum.
            vx = x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * w;
            vy = y + h - ((v / maximumValue) * h);
            nvgLineTo(vg, vx, vy);
        }
        break;
    }
    case STYLE_MS: {
        for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
            float v = values[(currentIndex+i) % GRAPH_HISTORY_COUNT] * 1000.0f;
            float vx, vy;
            if (v > maximumValue) maximumValue = v; // automatically increase the maximum.
            vx = x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * w;
            vy = y + h - ((v / maximumValue) * h);
            nvgLineTo(vg, vx, vy);
        }
        break;

    }
    case STYLE_MEM: {

        for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
            float v = values[(currentIndex+i) % GRAPH_HISTORY_COUNT] * 1.0f;
            float vx, vy;
            if (v > maximumValue) maximumValue = v; // automatically increase the maximum.
            vx = x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * w;
            vy = y + h - ((v / maximumValue) * h);
            nvgLineTo(vg, vx, vy);
        }
        break;
    }
    default: break;
    }
    nvgLineTo(vg, x+w, y+h);
    nvgFillColor(vg, nvgRGBA(192, 255, 0,128));
    nvgFill(vg);

    nvgFontFace(vg, "sans");

    if (title[0] != '\0') {
        nvgFontSize(vg, 14.0f);
        nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,192));
        nvgText(vg, x+3,y+1, title, NULL);
    }

    switch(style)
    {
    case STYLE_FPS: {
        nvgFontSize(vg, 18.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,255));
        sprintf(str, "%.2f FPS", 1.0f / avg);
        nvgText(vg, x+w-3,y+1, str, NULL);

        nvgFontSize(vg, 15.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_BOTTOM);
        nvgFillColor(vg, nvgRGBA(240,240,240,160));
        sprintf(str, "%.2f ms", avg * 1000.0f);
        nvgText(vg, x+w-3,y+h-1, str, NULL);
        break;
    }
    case STYLE_MS: {
        nvgFontSize(vg, 18.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,255));
        sprintf(str, "%.2f ms", avg * 1000.0f);
        nvgText(vg, x+w-3,y+1, str, NULL);
        break;
    }
    case STYLE_MEM: {
        nvgFontSize(vg, 18.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,255));
        sprintf(str, "%.2f MB", avg);
        nvgText(vg, x+w-3,y+1, str, NULL);

        nvgFontSize(vg, 15.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_BOTTOM);
        nvgFillColor(vg, nvgRGBA(240,240,240,160));
        sprintf(str, "%.1f %%", avg / maximumValue * 100.0f);
        nvgText(vg, x+w-3,y+h-1, str, NULL);
        break;
    }
    default: break;
    }
}
