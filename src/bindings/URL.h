#pragma once

#include "JSC/JSCHelpers.h"


class URL : public JSC::Binding<URL> {
public:
    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);
};
