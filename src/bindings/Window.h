#pragma once

#include "JSC/JSCHelpers.h"

class Window : public JSC::Binding<Window>
{
public:

    static JSC::Class &GetClassRef();

    static void OnRequestAnimationFrame(void* ptr, double timestamp);

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(createTimer);
    static JSC_FUNCTION(deleteTimer);

    static JSC_FUNCTION(addEventListener);
    static JSC_FUNCTION(removeEventListener);

    static JSC_FUNCTION(requestAnimationFrame);

};
