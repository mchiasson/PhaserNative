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

    static std::unordered_map<std::string, std::set<JSC::Object> > eventListeners;
};

#endif // PHASERNATIVEEVENT_H
