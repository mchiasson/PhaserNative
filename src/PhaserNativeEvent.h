#ifndef PHASERNATIVEEVENT_H
#define PHASERNATIVEEVENT_H

#include <cinttypes>
#include <vector>

#include <SDL2/SDL_events.h>


struct PhaserNativeEvent
{
    static std::vector<SDL_Event*> Timers;

    static uint32_t Timeout;
};

#endif // PHASERNATIVEEVENT_H
