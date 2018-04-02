#pragma once

#include <JSC/JSCHelpers.h>

class Body :  public JSC::Binding<Body>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(appendChild);
};
