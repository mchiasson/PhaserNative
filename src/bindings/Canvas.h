#pragma once

#include "JSC/JSCHelpers.h"
#include <SDL2/SDL_log.h>

#include "CanvasRenderingContext2D.h"

class Canvas : public JSC::Binding<Canvas>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(getContext);
    static JSC_FUNCTION(addEventListener);
    static JSC_FUNCTION(removeEventListener);
    static JSC_FUNCTION(getBoundingClientRect);
};

