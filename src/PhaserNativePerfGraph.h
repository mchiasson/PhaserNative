#pragma once

#include "nanovg/nanovg.h"
#include "PhaserNativePerfGraph.h"

#include <string>

#define GRAPH_HISTORY_COUNT 100
#define GPU_QUERY_COUNT 5


class PhaserNativePerfGraph {
public:
    enum Style {
        STYLE_FPS,
        STYLE_MS,
        STYLE_MEM,
    };

    PhaserNativePerfGraph(const char *title, Style style, float maximumValue);
    void updateGraph(float frameTime);
    void renderGraph(NVGcontext* vg, float x, float y);
    float getGraphAverage() const;
    float getMaximumValue() const { return maximumValue; }
    void setMaximumValue(float maximumValue) { this->maximumValue = maximumValue; }

private:
    const char *title;
    Style style;
    float maximumValue;
    int currentIndex;
    float values[GRAPH_HISTORY_COUNT];
};

class PhaserNativeFPSPerfGraph : public PhaserNativePerfGraph {
public: PhaserNativeFPSPerfGraph(const char *title) : PhaserNativePerfGraph(title, STYLE_FPS, 80.0f) {}
};

class PhaserNativeMSPerfGraph : public PhaserNativePerfGraph {
public: PhaserNativeMSPerfGraph(const char *title) : PhaserNativePerfGraph(title, STYLE_MS, 5.0f) {}
};

class PhaserNativeMemPerfGraph : public PhaserNativePerfGraph {
public: PhaserNativeMemPerfGraph(const char *title) : PhaserNativePerfGraph(title, STYLE_MEM, 5.0f) {}
};

struct GPUtimer {
    bool supported;
    int cur, ret;
    unsigned int queries[GPU_QUERY_COUNT];
};
typedef struct GPUtimer GPUtimer;

void initGPUTimer(GPUtimer* timer);
void startGPUTimer(GPUtimer* timer);
int stopGPUTimer(GPUtimer* timer, float* times, int maxTimes);


