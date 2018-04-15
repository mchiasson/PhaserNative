#include "PhaserNativeEvent.h"

std::vector<SDL_Event*> PhaserNativeEvent::Timers;

uint32_t PhaserNativeEvent::Timeout = 0;
uint32_t PhaserNativeEvent::ImageDecoded = 0;
uint32_t PhaserNativeEvent::RequestAnimationFrame = 0;
uint32_t PhaserNativeEvent::XHR = 0;

