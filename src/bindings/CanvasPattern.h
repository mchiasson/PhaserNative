#pragma once

#include "JSC/JSCHelpers.h"
#include <nanovg/nanovg.h>

#include <map>

class CanvasPattern : public JSC::Binding<CanvasPattern>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    int imageID = 0;

    float ox = 0;
    float oy = 0;
    float w = 0;
    float h = 0;
    float angle = 0;
    float alpha = 1;

};

