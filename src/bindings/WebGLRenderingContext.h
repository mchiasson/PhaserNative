#pragma once

#include <JSC/JSCHelpers.h>

class WebGLRenderingContext : public JSC::Binding<WebGLRenderingContext>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

};

