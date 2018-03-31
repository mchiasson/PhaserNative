#pragma once

#include "JSC/JSCHelpers.h"

class DocumentElement :  public JSC::Binding<DocumentElement>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(createElement);
};
