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

    static JSC_PROPERTY_GET(getCanvas);
    static JSC_PROPERTY_GET(getCurrentTransform);
    static JSC_PROPERTY_GET(getDirection);
    static JSC_PROPERTY_GET(getFillStyle);
    static JSC_PROPERTY_GET(getFilter);
    static JSC_PROPERTY_GET(getFont);
    static JSC_PROPERTY_GET(getGlobalAlpha);
    static JSC_PROPERTY_GET(getGlobalCompositeOperation);
    static JSC_PROPERTY_GET(getImageSmoothingEnabled);
    static JSC_PROPERTY_GET(getImageSmoothingQuality);
    static JSC_PROPERTY_GET(getLineCap);
    static JSC_PROPERTY_GET(getLineDashOffset);
    static JSC_PROPERTY_GET(getLineJoin);
    static JSC_PROPERTY_GET(getLineWidth);
    static JSC_PROPERTY_GET(getMiterLimit);
    static JSC_PROPERTY_GET(getShadowBlur);
    static JSC_PROPERTY_GET(getShadowColor);
    static JSC_PROPERTY_GET(getShadowOffsetX);
    static JSC_PROPERTY_GET(getShadowOffsetY);
    static JSC_PROPERTY_GET(getStrokeStyle);
    static JSC_PROPERTY_GET(getTextAlign);
    static JSC_PROPERTY_GET(getTextBaseline);

    static JSC_PROPERTY_SET(setCanvas);
    static JSC_PROPERTY_SET(setCurrentTransform);
    static JSC_PROPERTY_SET(setDirection);
    static JSC_PROPERTY_SET(setFillStyle);
    static JSC_PROPERTY_SET(setFilter);
    static JSC_PROPERTY_SET(setFont);
    static JSC_PROPERTY_SET(setGlobalAlpha);
    static JSC_PROPERTY_SET(setGlobalCompositeOperation);
    static JSC_PROPERTY_SET(setImageSmoothingEnabled);
    static JSC_PROPERTY_SET(setImageSmoothingQuality);
    static JSC_PROPERTY_SET(setLineCap);
    static JSC_PROPERTY_SET(setLineDashOffset);
    static JSC_PROPERTY_SET(setLineJoin);
    static JSC_PROPERTY_SET(setLineWidth);
    static JSC_PROPERTY_SET(setMiterLimit);
    static JSC_PROPERTY_SET(setShadowBlur);
    static JSC_PROPERTY_SET(setShadowColor);
    static JSC_PROPERTY_SET(setShadowOffsetX);
    static JSC_PROPERTY_SET(setShadowOffsetY);
    static JSC_PROPERTY_SET(setStrokeStyle);
    static JSC_PROPERTY_SET(setTextAlign);
    static JSC_PROPERTY_SET(setTextBaseline);

    static JSC_FUNCTION(arc);
    static JSC_FUNCTION(arcTo);
    static JSC_FUNCTION(beginPath);
    static JSC_FUNCTION(bezierCurveTo);
    static JSC_FUNCTION(clearHitRegions);
    static JSC_FUNCTION(clearRect);
    static JSC_FUNCTION(clip);
    static JSC_FUNCTION(closePath);
    static JSC_FUNCTION(createImageData);
    static JSC_FUNCTION(createLinearGradient);
    static JSC_FUNCTION(createPattern);
    static JSC_FUNCTION(createRadialGradient);
    static JSC_FUNCTION(drawFocusIfNeeded);
    static JSC_FUNCTION(drawImage);
    static JSC_FUNCTION(drawWidgetAsOnScreen);
    static JSC_FUNCTION(drawWindow);
    static JSC_FUNCTION(ellipse);
    static JSC_FUNCTION(fill);
    static JSC_FUNCTION(fillRect);
    static JSC_FUNCTION(fillText);
    static JSC_FUNCTION(getImageData);
    static JSC_FUNCTION(getLineDash);
    static JSC_FUNCTION(isPointInPath);
    static JSC_FUNCTION(isPointInStroke);
    static JSC_FUNCTION(lineTo);
    static JSC_FUNCTION(measureText);
    static JSC_FUNCTION(moveTo);
    static JSC_FUNCTION(putImageData);
    static JSC_FUNCTION(quadraticCurveTo);
    static JSC_FUNCTION(rect);
    static JSC_FUNCTION(removeHitRegion);
    static JSC_FUNCTION(resetTransform);
    static JSC_FUNCTION(restore);
    static JSC_FUNCTION(rotate);
    static JSC_FUNCTION(save);
    static JSC_FUNCTION(scale);
    static JSC_FUNCTION(scrollPathIntoView);
    static JSC_FUNCTION(setLineDash);
    static JSC_FUNCTION(setTransform);
    static JSC_FUNCTION(stroke);
    static JSC_FUNCTION(strokeRect);
    static JSC_FUNCTION(strokeText);
    static JSC_FUNCTION(transform);
    static JSC_FUNCTION(translate);

    SDL_GLContext context = nullptr;
    uint32_t vao = 0;
    size_t canvasIndex = 0;

    JSC::Value m_currentTransform;
    JSC::Value m_direction;
    JSC::Value m_fillStyle;
    JSC::Value m_filter;
    JSC::Value m_font;
    JSC::Value m_globalAlpha;
    JSC::Value m_globalCompositeOperation;
    JSC::Value m_imageSmoothingEnabled;
    JSC::Value m_imageSmoothingQuality;
    JSC::Value m_lineCap;
    JSC::Value m_lineDashOffset;
    JSC::Value m_lineJoin;
    JSC::Value m_lineWidth;
    JSC::Value m_miterLimit;
    JSC::Value m_shadowBlur;
    JSC::Value m_shadowColor;
    JSC::Value m_shadowOffsetX;
    JSC::Value m_shadowOffsetY;
    JSC::Value m_strokeStyle;
    JSC::Value m_textAlign;
    JSC::Value m_textBaseline;

    NVGcontext *vg;
    int fontIcons = 0;
    int fontNormal = 0;
    int fontBold = 0;
    int fontEmoji = 0;

};
