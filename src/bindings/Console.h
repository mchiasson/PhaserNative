#pragma once

#include <SDL2/SDL_log.h>

#include "JSC/JSCHelpers.h"

class Console : public JSC::Binding<Console>
{
public:
    static JSC::Class &GetClassRef();
private:
    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(log);
    static JSC_FUNCTION(info);
    static JSC_FUNCTION(warn);
    static JSC_FUNCTION(error);
};
