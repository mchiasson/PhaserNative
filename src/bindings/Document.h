#pragma once

#include "JSC/JSCHelpers.h"

class Document :  public JSC::Binding<Document>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(createElement);
};
