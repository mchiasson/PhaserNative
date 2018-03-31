#pragma once

#include "JSC/JSCHelpers.h"

class Window : public JSC::Binding<Window>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

};
