#pragma once

#include <JSC/JSCHelpers.h>

class CanvasRenderingContext2D : public JSC::Binding<CanvasRenderingContext2D>
{
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_INITIALIZER(Initializer);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(fillRect);
    static JSC_FUNCTION(getImageData);
    static JSC_FUNCTION(putImageData);

    JSC_RW_PROPERTY(fillStyle);

};
