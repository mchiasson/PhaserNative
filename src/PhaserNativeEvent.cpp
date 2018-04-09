#include "PhaserNativeEvent.h"

std::vector<SDL_Event*> PhaserNativeEvent::Timers;

uint32_t PhaserNativeEvent::Timeout = 0;
uint32_t PhaserNativeEvent::ImageDecoded = 0;
uint32_t PhaserNativeEvent::RequestAnimationFrame = 0;
uint32_t PhaserNativeEvent::XHR = 0;

std::vector<JSC::Object> PhaserNativeEvent::keyDownListeners;
std::vector<JSC::Object> PhaserNativeEvent::keyUpListeners;
std::vector<JSC::Object> PhaserNativeEvent::mouseMoveListeners;
std::vector<JSC::Object> PhaserNativeEvent::mouseDownListeners;
std::vector<JSC::Object> PhaserNativeEvent::mouseUpListeners;
std::vector<JSC::Object> PhaserNativeEvent::touchStartListeners;
std::vector<JSC::Object> PhaserNativeEvent::touchMoveListeners;
std::vector<JSC::Object> PhaserNativeEvent::touchEndListeners;
