#include "PhaserNativeEvent.h"

std::vector<SDL_Event*> PhaserNativeEvent::Timers;

uint32_t PhaserNativeEvent::Timeout = 0;

std::unordered_map<std::string, std::set<JSC::Object> > PhaserNativeEvent::eventListeners;
