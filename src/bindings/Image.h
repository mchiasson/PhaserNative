#pragma once

#include "JSC/JSCHelpers.h"

class Image : public JSC::Binding<Image>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    JSC_RW_PROPERTY(src);
    JSC_RW_PROPERTY(width);
    JSC_RW_PROPERTY(height);

};

