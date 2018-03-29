#include "PhaserNativeEvent.h"

std::vector<SDL_Event*> PhaserNativeEvent::Timers;

uint32_t PhaserNativeEvent::Timeout = 0;
