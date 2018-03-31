#pragma once

#include <SDL2/SDL_timer.h>

#include "PhaserNativeEvent.h"

#include "JSC/JSCHelpers.h"

class Timer : public JSC::Binding<Timer>
{
public:

    static void Register(JSContextRef ctx);

private:

    static JSC_FUNCTION(createTimer);
    static JSC_FUNCTION(deleteTimer);

};
