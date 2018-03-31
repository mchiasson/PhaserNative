#pragma once

#include "JSC/JSCHelpers.h"

class Navigator : public JSC::Binding<Navigator>
{
public:
    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);
};
