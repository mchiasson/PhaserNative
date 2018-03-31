#pragma once

#include "JSC/JSCHelpers.h"


class Performance : public JSC::Binding<Performance>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(now);
};

