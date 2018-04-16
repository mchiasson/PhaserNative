#pragma once

#include "JSC/JSCHelpers.h"
#include <nanovg/nanovg.h>

#include <map>

class CanvasGradient : public JSC::Binding<CanvasGradient>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(addColorStop);

    float x0 = 0;
    float y0 = 0;
    float r0 = 0;
    float x1 = 0;
    float y1 = 0;
    float r1 = 0;

    bool isRadial = false;

    std::map<float, NVGcolor> colorStop;
};

