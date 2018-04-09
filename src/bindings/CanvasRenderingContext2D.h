#pragma once

#include <JSC/JSCHelpers.h>
#include <SDL2/SDL.h>
#include <nanovg/nanovg.h>

class CanvasRenderingContext2D : public JSC::Binding<CanvasRenderingContext2D>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(fillRect);

    static JSC_FUNCTION(clearRect);
    static JSC_FUNCTION(createImageData);
    static JSC_FUNCTION(getImageData);
    static JSC_FUNCTION(putImageData);
    static JSC_FUNCTION(drawImage);

    static JSC_FUNCTION(measureText);
    static JSC_FUNCTION(fillText);

    static JSC_FUNCTION(save);
    static JSC_FUNCTION(restore);

    static JSC_FUNCTION(translate);

    static JSC_PROPERTY_GET(getFillStyle);
    static JSC_PROPERTY_SET(setFillStyle);

    static JSC_PROPERTY_GET(getCanvas);

    JSC::Value m_fillStyle;

    SDL_GLContext context = nullptr;
    uint32_t vao = 0;
    size_t canvasIndex = 0;

    NVGcontext *vg;
    int fontIcons = 0;
    int fontNormal = 0;
    int fontBold = 0;
    int fontEmoji = 0;

};
