#pragma once

#include "JSC/JSCHelpers.h"

class Navigator : public JSC::Binding<Navigator>
{
public:
    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);
};
