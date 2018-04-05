#pragma once

#include <JSC/JSCHelpers.h>

class Body :  public JSC::Binding<Body>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(appendChild);
};
