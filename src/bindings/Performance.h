#pragma once

#include "JSC/JSCHelpers.h"


class Performance : public JSC::Binding<Performance>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(now);
};

