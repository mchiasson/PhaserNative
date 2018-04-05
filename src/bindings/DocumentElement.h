#pragma once

#include "JSC/JSCHelpers.h"

class DocumentElement :  public JSC::Binding<DocumentElement>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(createElement);
};
