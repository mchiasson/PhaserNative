#ifndef PHASERNATIVEEVENT_H
#define PHASERNATIVEEVENT_H

#include <cinttypes>
#include <set>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL_events.h>

#include "JSC/JSCObject.h"

struct PhaserNativeEvent
{
    static std::vector<SDL_Event*> Timers;
    static uint32_t Timeout;
    static uint32_t ImageDecoded;
    static uint32_t RequestAnimationFrame;
    static uint32_t XHR;

    static std::vector<JSC::Object> keyDownListeners;
    static std::vector<JSC::Object> keyUpListeners;
    static std::vector<JSC::Object> mouseMoveListeners;
    static std::vector<JSC::Object> mouseDownListeners;
    static std::vector<JSC::Object> mouseUpListeners;
    static std::vector<JSC::Object> touchStartListeners;
    static std::vector<JSC::Object> touchMoveListeners;
    static std::vector<JSC::Object> touchEndListeners;

};

#endif // PHASERNATIVEEVENT_H
