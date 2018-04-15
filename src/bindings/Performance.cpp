#include "Performance.h"
#include "SDL2/SDL_timer.h"

double Performance::now()
{
    return ((double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency() * 1000.0);
}

