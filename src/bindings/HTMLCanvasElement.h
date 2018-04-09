#pragma once

#include "JSC/JSCHelpers.h"
#include <SDL2/SDL.h>

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

    static JSC_PROPERTY_GET(getWidth);
    static JSC_PROPERTY_SET(setWidth);
    static JSC_PROPERTY_GET(getHeight);
    static JSC_PROPERTY_SET(setHeight);

    SDL_Window *window = nullptr;

    size_t canvas2dIndex = 0;
    size_t glIndex = 0;
};

