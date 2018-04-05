#pragma once

#include "JSC/JSCHelpers.h"
#include <SDL2/SDL_log.h>

class HTMLCanvasElement : public JSC::Binding<HTMLCanvasElement>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(getContext);
    static JSC_FUNCTION(addEventListener);
    static JSC_FUNCTION(removeEventListener);
    static JSC_FUNCTION(getBoundingClientRect);
};

