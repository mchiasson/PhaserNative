#pragma once

#include "JSC/JSCHelpers.h"

#include <SDL2/SDL_events.h>

class KeyboardEvent : public JSC::Binding<KeyboardEvent>
{
public:
    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(preventDefault);


    void configure(SDL_Event &e);
};
