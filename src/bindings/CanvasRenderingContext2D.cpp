#include "CanvasRenderingContext2D.h"

#include "ColorUtil.h"
#include "Image.h"
#include "HTMLCanvasElement.h"
#include "CanvasGradient.h"
#include "CanvasPattern.h"

#include "PhaserNativeGraphics.h"

#include <stb/stb_image.h>

#include <sstream>

#define THROW_UNIMPLEMENTED_ERROR() \
    do {\
        std::stringstream ss;\
        ss << "canvas2d." << __FUNCTION__ << " not implemented! Contact a Developer!";\
        *exception = JSC::Object::MakeError(ss.str().c_str());\
    } while((void)0, 0)

#define WARN_UNIMPLEMENTED() \
    do {\
        static bool warned = false;\
        if (!warned) {\
            SDL_LogWarn(0, "canvas2d.%s not implemented! Contact a Developer!\n", __FUNCTION__);\
            warned=true;\
        }\
    } while((void)0, 0)


JSC_CONSTRUCTOR(CanvasRenderingContext2D::Constructor) {

    CanvasRenderingContext2D &canvas2d = CreateNativeInstance();

    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(JSC::Value(argv[0]).toUnsignedInteger());

    canvas2d.canvasIndex = JSC::Value(argv[0]);
    canvas2d.vg = PhaserNativeCreateNanoVGContext();

    canvas2d.fontIcons = nvgCreateFont(canvas2d.vg, "icons", "entypo.ttf");
    if (canvas2d.fontIcons == -1) {
        *exception = JSC::Object::MakeError("Could not add font icons.");
        return JSC::Object();
    }

    canvas2d.fontNormal = nvgCreateFont(canvas2d.vg, "sans", "Roboto-Regular.ttf");
    if (canvas2d.fontNormal == -1) {
        *exception = JSC::Object::MakeError("Could not add font italic.");
        return JSC::Object();
    }

    canvas2d.fontBold = nvgCreateFont(canvas2d.vg, "sans-bold", "Roboto-Bold.ttf");
    if (canvas2d.fontBold == -1) {
        *exception = JSC::Object::MakeError("Could not add font bold.");
        return JSC::Object();
    }

    canvas2d.fontEmoji = nvgCreateFont(canvas2d.vg, "emoji", "NotoEmoji-Regular.ttf");
    if (canvas2d.fontEmoji == -1) {
        *exception = JSC::Object::MakeError("Could not add font emoji.");
        return JSC::Object();
    }

    nvgAddFallbackFontId(canvas2d.vg, canvas2d.fontNormal, canvas2d.fontEmoji);
    nvgAddFallbackFontId(canvas2d.vg, canvas2d.fontBold, canvas2d.fontEmoji);

    canvas2d.object.setProperty("globalAlpha", 1.0f); // defautl value.

    return canvas2d.object;

}

JSC_FINALIZER(CanvasRenderingContext2D::Finalizer) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);

    PhaserNativeDestroyNanoVGContext(canvas2d.vg);
    canvas2d.vg = nullptr;
    canvas2d.fontIcons = 0;
    canvas2d.fontNormal = 0;
    canvas2d.fontBold = 0;
    canvas2d.fontEmoji = 0;

    canvas2d.canvasIndex = 0;

    canvas2d.m_currentTransform = JSC::Value();
    canvas2d.m_direction = JSC::Value();
    canvas2d.m_fillStyle = JSC::Value();
    canvas2d.m_filter = JSC::Value();
    canvas2d.m_font = JSC::Value();
    canvas2d.m_globalAlpha = JSC::Value();
    canvas2d.m_globalCompositeOperation = JSC::Value();
    canvas2d.m_imageSmoothingEnabled = JSC::Value();
    canvas2d.m_imageSmoothingQuality = JSC::Value();
    canvas2d.m_lineCap = JSC::Value();
    canvas2d.m_lineDashOffset = JSC::Value();
    canvas2d.m_lineJoin = JSC::Value();
    canvas2d.m_lineWidth = JSC::Value();
    canvas2d.m_miterLimit = JSC::Value();
    canvas2d.m_shadowBlur = JSC::Value();
    canvas2d.m_shadowColor = JSC::Value();
    canvas2d.m_shadowOffsetX = JSC::Value();
    canvas2d.m_shadowOffsetY = JSC::Value();
    canvas2d.m_strokeStyle = JSC::Value();
    canvas2d.m_textAlign = JSC::Value();
    canvas2d.m_textBaseline = JSC::Value();

    FreeNativeInstance(object);
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getCanvas) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    return canvas.object;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getCurrentTransform) {
    return GetNativeInstance(object).m_currentTransform;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getDirection) {
   return GetNativeInstance(object).m_direction;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getFillStyle) {
   return GetNativeInstance(object).m_fillStyle;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getFilter) {
  return GetNativeInstance(object).m_filter;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getFont) {
    return GetNativeInstance(object).m_font;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getGlobalAlpha) {
    return GetNativeInstance(object).m_globalAlpha;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getGlobalCompositeOperation) {
    return GetNativeInstance(object).m_globalCompositeOperation;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getImageSmoothingEnabled) {
    return GetNativeInstance(object).m_imageSmoothingEnabled;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getImageSmoothingQuality) {
    return GetNativeInstance(object).m_imageSmoothingQuality;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getLineCap) {
    return GetNativeInstance(object).m_lineCap;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getLineDashOffset) {
    return GetNativeInstance(object).m_lineDashOffset;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getLineJoin) {
   return GetNativeInstance(object).m_lineJoin;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getLineWidth) {
    return GetNativeInstance(object).m_lineWidth;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getMiterLimit) {
    return GetNativeInstance(object).m_miterLimit;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getShadowBlur) {
    return GetNativeInstance(object).m_shadowBlur;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getShadowColor) {
    return GetNativeInstance(object).m_shadowColor;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getShadowOffsetX) {
   return GetNativeInstance(object).m_shadowOffsetX;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getShadowOffsetY) {
    return GetNativeInstance(object).m_shadowOffsetY;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getStrokeStyle) {
    return GetNativeInstance(object).m_strokeStyle;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getTextAlign) {
    return GetNativeInstance(object).m_textAlign;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getTextBaseline) {
    return GetNativeInstance(object).m_textBaseline;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setCanvas) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(JSC::Value(value).toObject());
    canvas2d.canvasIndex = canvas.index;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setCurrentTransform) {
    GetNativeInstance(object).m_currentTransform = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setDirection) {
   GetNativeInstance(object).m_direction = value;
   WARN_UNIMPLEMENTED();
   return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setFillStyle) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_fillStyle = value;

    if (canvas2d.m_fillStyle.isString())
    {
        nvgFillColor(canvas2d.vg, ColorUtil::stringToColor(canvas2d.m_fillStyle.toString().getUTF8String()));
    }
    else if (canvas2d.m_fillStyle.isObjectOfClass(CanvasGradient::GetClassRef()))
    {
        CanvasGradient &gradient = CanvasGradient::GetNativeInstance(canvas2d.m_fillStyle);
        if (gradient.isRadial)
        {
            NVGcolor start;
            NVGcolor end;

            auto first = gradient.colorStop.begin();
            auto last = gradient.colorStop.rbegin();

            if (first == gradient.colorStop.end())
            {
                start = nvgRGBf(0, 0, 0);
            }
            else
            {
                start = first->second;
            }

            if (last == gradient.colorStop.rend())
            {
                end = nvgRGBf(1, 1, 1);
            }
            else
            {
                end = last->second;
            }

            NVGpaint paint = nvgRadialGradient(canvas2d.vg, gradient.x0, gradient.y0, gradient.r0, gradient.r1, start, end);
            nvgFillPaint(canvas2d.vg, paint);
        }
        else
        {
            NVGcolor start;
            NVGcolor end;

            auto first = gradient.colorStop.begin();
            auto last = gradient.colorStop.rbegin();

            if (first == gradient.colorStop.end())
            {
                start = nvgRGBf(0, 0, 0);
            }
            else
            {
                start = first->second;
            }

            if (last == gradient.colorStop.rend())
            {
                end = nvgRGBf(1, 1, 1);
            }
            else
            {
                end = last->second;
            }

            NVGpaint paint = nvgLinearGradient(canvas2d.vg, gradient.x0, gradient.y0, gradient.x1, gradient.y1, start, end);
            nvgFillPaint(canvas2d.vg, paint);
        }
    }

   return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setFilter) {
  GetNativeInstance(object).m_filter = value;
  WARN_UNIMPLEMENTED();
  return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setFont) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    GetNativeInstance(object).m_font = value;

    WARN_UNIMPLEMENTED();

    // TODO parse the actual m_font
    nvgFontSize(canvas2d.vg, 48);
    nvgFontFaceId(canvas2d.vg, canvas2d.fontNormal);

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setGlobalAlpha) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_globalAlpha = value;
    nvgGlobalAlpha(canvas2d.vg, canvas2d.m_globalAlpha.toFloat());
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setGlobalCompositeOperation) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_globalCompositeOperation = value;

    std::string opStr = canvas2d.m_globalCompositeOperation.toString().getUTF8String();

    if (opStr == "source-over") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_SOURCE_OVER);
    } else if (opStr == "source-in") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_SOURCE_IN);
    } else if (opStr == "source-out") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_SOURCE_OUT);
    } else if (opStr == "source-atop") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_ATOP);
    } else if (opStr == "destination-over") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_DESTINATION_OVER);
    } else if (opStr == "destination-in") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_DESTINATION_IN);
    } else if (opStr == "destination-out") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_DESTINATION_OUT);
    } else if (opStr == "destination-atop") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_DESTINATION_ATOP);
    } else if (opStr == "lighter") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_LIGHTER);
    } else if (opStr == "copy") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_COPY);
    } else if (opStr == "xor") {
        nvgGlobalCompositeOperation(canvas2d.vg, NVG_XOR);
    } else {
        SDL_LogWarn(0, "globalCompositeOperation '%s' is not implemented. Contact a Developer!\n", opStr.c_str());
    }

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setImageSmoothingEnabled) {
    GetNativeInstance(object).m_imageSmoothingEnabled = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setImageSmoothingQuality) {
    GetNativeInstance(object).m_imageSmoothingQuality = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setLineCap) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_lineCap = value;

    std::string lineCap = canvas2d.m_lineCap.toString().getUTF8String();

    if (lineCap == "butt")
    {
        nvgLineCap(canvas2d.vg, NVG_BUTT);
    }
    else if (lineCap == "round")
    {
        nvgLineCap(canvas2d.vg, NVG_ROUND);
    }
    else if (lineCap == "square")
    {
        nvgLineCap(canvas2d.vg, NVG_SQUARE);
    }

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setLineDashOffset) {
    GetNativeInstance(object).m_lineDashOffset = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setLineJoin) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_lineJoin = value;

    std::string lineJoin = canvas2d.m_lineJoin.toString().getUTF8String();

    if (lineJoin == "miter")
    {
        nvgLineJoin(canvas2d.vg, NVG_MITER);
    }
    else if (lineJoin == "round")
    {
        nvgLineJoin(canvas2d.vg, NVG_ROUND);
    }
    else if (lineJoin == "bevel")
    {
        nvgLineJoin(canvas2d.vg, NVG_BEVEL);
    }

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setLineWidth) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_lineWidth = value;
    nvgStrokeWidth(canvas2d.vg, canvas2d.m_lineWidth.toFloat());

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setMiterLimit) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_miterLimit = value;
    nvgMiterLimit(canvas2d.vg, canvas2d.m_miterLimit.toFloat());

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setShadowBlur) {
    GetNativeInstance(object).m_shadowBlur = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setShadowColor) {
    GetNativeInstance(object).m_shadowColor = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setShadowOffsetX) {
   GetNativeInstance(object).m_shadowOffsetX = value;
   WARN_UNIMPLEMENTED();
   return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setShadowOffsetY) {
    GetNativeInstance(object).m_shadowOffsetY = value;
    WARN_UNIMPLEMENTED();
    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setStrokeStyle) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_strokeStyle = value;

    if (canvas2d.m_strokeStyle.isString())
    {
        nvgStrokeColor(canvas2d.vg, ColorUtil::stringToColor(canvas2d.m_strokeStyle.toString().getUTF8String()));
    }
    else if (canvas2d.m_strokeStyle.isObjectOfClass(CanvasGradient::GetClassRef()))
    {
        CanvasGradient &gradient = CanvasGradient::GetNativeInstance(canvas2d.m_strokeStyle);
        if (gradient.isRadial)
        {
            NVGcolor start;
            NVGcolor end;

            auto first = gradient.colorStop.begin();
            auto last = gradient.colorStop.rbegin();

            if (first == gradient.colorStop.end())
            {
                start = nvgRGBf(0, 0, 0);
            }
            else
            {
                start = first->second;
            }

            if (last == gradient.colorStop.rend())
            {
                end = nvgRGBf(1, 1, 1);
            }
            else
            {
                end = last->second;
            }

            NVGpaint paint = nvgRadialGradient(canvas2d.vg, gradient.x0, gradient.y0, gradient.r0, gradient.r1, start, end);
            nvgStrokePaint(canvas2d.vg, paint);
        }
        else
        {
            NVGcolor start;
            NVGcolor end;

            auto first = gradient.colorStop.begin();
            auto last = gradient.colorStop.rbegin();

            if (first == gradient.colorStop.end())
            {
                start = nvgRGBf(0, 0, 0);
            }
            else
            {
                start = first->second;
            }

            if (last == gradient.colorStop.rend())
            {
                end = nvgRGBf(1, 1, 1);
            }
            else
            {
                end = last->second;
            }

            NVGpaint paint = nvgLinearGradient(canvas2d.vg, gradient.x0, gradient.y0, gradient.x1, gradient.y1, start, end);
            nvgStrokePaint(canvas2d.vg, paint);
        }
    }

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setTextAlign) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_textAlign = value;

    std::string textAlignStr = canvas2d.m_textAlign.toString().getUTF8String();
    std::string textBaselineStr = canvas2d.m_textBaseline.toString().getUTF8String();

    int textAlign = 0;
    int textBaseline = 0;

    if (textAlignStr == "left") {
        textAlign = NVG_ALIGN_LEFT;
    } else if (textAlignStr == "right") {
        textAlign = NVG_ALIGN_RIGHT;
    } else if (textAlignStr == "center") {
        textAlign = NVG_ALIGN_CENTER;
    } else if (textAlignStr == "start") {
        textAlign = NVG_ALIGN_LEFT;
    } else if (textAlignStr == "end") {
        textAlign = NVG_ALIGN_RIGHT;
    }

    if (textBaselineStr == "top") {
        textBaseline = NVG_ALIGN_TOP;
    } else if (textBaselineStr == "hanging") {
        textBaseline = NVG_ALIGN_MIDDLE;
    } else if (textBaselineStr == "middle") {
        textBaseline = NVG_ALIGN_MIDDLE;
    } else if (textBaselineStr == "alphabetic") {
        textBaseline = NVG_ALIGN_BASELINE;
    } else if (textBaselineStr == "ideographic") {
        textBaseline = NVG_ALIGN_BOTTOM;
    } else if (textBaselineStr == "bottom") {
        textBaseline = NVG_ALIGN_BOTTOM;
    }

    nvgTextAlign(canvas2d.vg, textAlign | textBaseline);

    return true;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setTextBaseline) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    canvas2d.m_textBaseline = value;

    std::string textAlignStr = canvas2d.m_textAlign.toString().getUTF8String();
    std::string textBaselineStr = canvas2d.m_textBaseline.toString().getUTF8String();

    int textAlign = 0;
    int textBaseline = 0;

    if (textAlignStr == "left") {
        textAlign = NVG_ALIGN_LEFT;
    } else if (textAlignStr == "right") {
        textAlign = NVG_ALIGN_RIGHT;
    } else if (textAlignStr == "center") {
        textAlign = NVG_ALIGN_CENTER;
    } else if (textAlignStr == "start") {
        textAlign = NVG_ALIGN_LEFT;
    } else if (textAlignStr == "end") {
        textAlign = NVG_ALIGN_RIGHT;
    }

    if (textBaselineStr == "top") {
        textBaseline = NVG_ALIGN_TOP;
    } else if (textBaselineStr == "hanging") {
        textBaseline = NVG_ALIGN_MIDDLE;
    } else if (textBaselineStr == "middle") {
        textBaseline = NVG_ALIGN_MIDDLE;
    } else if (textBaselineStr == "alphabetic") {
        textBaseline = NVG_ALIGN_BASELINE;
    } else if (textBaselineStr == "ideographic") {
        textBaseline = NVG_ALIGN_BOTTOM;
    } else if (textBaselineStr == "bottom") {
        textBaseline = NVG_ALIGN_BOTTOM;
    }

    nvgTextAlign(canvas2d.vg, textAlign | textBaseline);

    return true;
}

JSC_FUNCTION(CanvasRenderingContext2D::arc) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x              = JSC::Value(argv[0]).toFloat();
    float y              = JSC::Value(argv[1]).toFloat();
    float radius         = JSC::Value(argv[2]).toFloat();
    float startAngle     = JSC::Value(argv[3]).toFloat();
    float endAngle       = JSC::Value(argv[4]).toFloat();
    bool  anticlockwise  = argc == 6 ? JSC::Value(argv[5]).toBoolean() : true;

    int dir = anticlockwise ? NVG_CCW : NVG_CW;

    nvgArc(canvas2d.vg,
           x,
           y,
           radius,
           startAngle,
           endAngle,
           dir);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::arcTo) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x1 = JSC::Value(argv[0]).toFloat();
    float y1 = JSC::Value(argv[1]).toFloat();
    float x2 = JSC::Value(argv[2]).toFloat();
    float y2 = JSC::Value(argv[3]).toFloat();
    float r  = JSC::Value(argv[4]).toFloat();

    nvgArcTo(canvas2d.vg,x1,y1,x2,y2,r);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::beginPath) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgBeginPath(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::bezierCurveTo) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float cp1x = JSC::Value(argv[0]).toFloat();
    float cp1y = JSC::Value(argv[1]).toFloat();
    float cp2x = JSC::Value(argv[2]).toFloat();
    float cp2y = JSC::Value(argv[3]).toFloat();
    float x    = JSC::Value(argv[4]).toFloat();
    float y    = JSC::Value(argv[5]).toFloat();

    nvgBezierTo(canvas2d.vg,cp1x,cp1y,cp2x,cp2y,x,y);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::clearHitRegions) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    THROW_UNIMPLEMENTED_ERROR();

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::clearRect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();
    float w = JSC::Value(argv[2]).toFloat();
    float h = JSC::Value(argv[3]).toFloat();

    nvgSave(canvas2d.vg);
    nvgFillColor(canvas2d.vg,nvgRGBA(0, 0, 0, 0));
    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, x, y, w, h);
    nvgFill(canvas2d.vg);
    nvgRestore(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::clip) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    WARN_UNIMPLEMENTED();

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::closePath) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgClosePath(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::createImageData) {

    JSC::Value sw = argv[0];
    JSC::Value sh = argv[1];

    unsigned length = sw.toInteger() * sh.toInteger() * 4;
    JSC::Object data = JSC::Object:: MakeTypedArray(kJSTypedArrayTypeUint8Array, length);

    JSC::Object imageData = JSC::Object::MakeDefault();
    imageData.setProperty("data", data, kJSPropertyAttributeReadOnly);
    imageData.setProperty("width", sw, kJSPropertyAttributeReadOnly);
    imageData.setProperty("height", sh, kJSPropertyAttributeReadOnly);

    return imageData;
}

JSC_FUNCTION(CanvasRenderingContext2D::createLinearGradient) {

    JSC::Object gradientObj = CanvasGradient::CreateJSObject({});
    CanvasGradient &gradient = CanvasGradient::GetNativeInstance(gradientObj);

    gradient.x0 = JSC::Value(argv[0]).toFloat();
    gradient.y0 = JSC::Value(argv[1]).toFloat();
    gradient.x1 = JSC::Value(argv[2]).toFloat();
    gradient.y1 = JSC::Value(argv[3]).toFloat();
    gradient.isRadial = false;

    return gradientObj;
}

JSC_FUNCTION(CanvasRenderingContext2D::createPattern) {

    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    JSC::Object patternObj = CanvasPattern::CreateJSObject({});
    CanvasPattern &pattern = CanvasPattern::GetNativeInstance(patternObj);

    JSC::Value imageVal = argv[0];

    if (imageVal.isObjectOfClass(Image::GetClassRef()))
    {
        Image &image = Image::GetNativeInstance(imageVal.toObject());

        if (image.imageID == 0)
        {
            image.imageID = nvgCreateImageRGBA(canvas2d.vg,
                                               image.object.getProperty("width").toInteger(),
                                               image.object.getProperty("height").toInteger(),
                                               0,
                                               image.pixels);

            stbi_image_free(image.pixels);
            image.pixels = 0;
        }

        pattern.imageID = image.imageID;
    }
    else
    {
        THROW_UNIMPLEMENTED_ERROR();
    }

    return patternObj;
}

JSC_FUNCTION(CanvasRenderingContext2D::createRadialGradient) {

    JSC::Object gradientObj = CanvasGradient::CreateJSObject({});
    CanvasGradient &gradient = CanvasGradient::GetNativeInstance(gradientObj);

    gradient.x0 = JSC::Value(argv[0]).toFloat();
    gradient.y0 = JSC::Value(argv[1]).toFloat();
    gradient.r0 = JSC::Value(argv[2]).toFloat();
    gradient.x1 = JSC::Value(argv[3]).toFloat();
    gradient.y1 = JSC::Value(argv[4]).toFloat();
    gradient.r1 = JSC::Value(argv[5]).toFloat();
    gradient.isRadial = true;

    return gradientObj;
}

JSC_FUNCTION(CanvasRenderingContext2D::drawFocusIfNeeded) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::drawImage) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);


    Image &image = Image::GetNativeInstance(JSC::Value(argv[0]).toObject());

    if (image.imageID == 0)
    {
        image.imageID = nvgCreateImageRGBA(canvas2d.vg,
                                           image.object.getProperty("width").toInteger(),
                                           image.object.getProperty("height").toInteger(),
                                           0,
                                           image.pixels);

        stbi_image_free(image.pixels);
        image.pixels = 0;
    }

    int w, h;
    nvgImageSize(canvas2d.vg, image.imageID, &w, &h);

    float sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight;

    switch (argc)
    {
    case 3:
        sx = 0;
        sy = 0;
        sWidth = w;
        sHeight = h;
        dx = JSC::Value(argv[1]).toInteger();
        dy = JSC::Value(argv[2]).toInteger();
        dWidth = sWidth;
        dHeight = sHeight;
        break;
    case 5:
        sx = 0;
        sy = 0;
        sWidth = w;
        sHeight = h;
        dx = JSC::Value(argv[1]).toInteger();
        dy = JSC::Value(argv[2]).toInteger();
        dWidth = JSC::Value(argv[3]).toInteger();
        dHeight = JSC::Value(argv[4]).toInteger();
        break;
    case 9:
        sx = JSC::Value(argv[1]).toInteger();
        sy = JSC::Value(argv[2]).toInteger();
        sWidth = JSC::Value(argv[3]).toInteger();
        sHeight = JSC::Value(argv[4]).toInteger();
        dx = JSC::Value(argv[5]).toInteger();
        dy = JSC::Value(argv[6]).toInteger();
        dWidth = JSC::Value(argv[7]).toInteger();
        dHeight = JSC::Value(argv[8]).toInteger();
        break;
    }


    nvgSave(canvas2d.vg);
    nvgResetScissor(canvas2d.vg);
    nvgIntersectScissor(canvas2d.vg, dx, dy, dWidth, dHeight);

    float sw =  dWidth / sWidth;
    float sh =  dHeight / sHeight;
    float rx, ry, rw, rh;
    rw = w * sw;
    rh = h * sh;
    rx = dx - sx*sw;
    ry = dy - sy*sh;

    NVGpaint imgPaint = nvgImagePattern(canvas2d.vg, rx, ry, rw, rh, 0.0f, image.imageID, 1.0f);
    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, rx, ry, rw, rh);
    nvgFillPaint(canvas2d.vg, imgPaint);
    nvgFill(canvas2d.vg);
    nvgRestore(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::drawWidgetAsOnScreen) {
    THROW_UNIMPLEMENTED_ERROR();

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::drawWindow) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::ellipse) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float cx = JSC::Value(argv[0]).toFloat();
    float cy = JSC::Value(argv[1]).toFloat();
    float rx = JSC::Value(argv[2]).toFloat();
    float ry = JSC::Value(argv[3]).toFloat();
//    float r  = JSC::Value(argv[4]).toFloat();
//    float sa = JSC::Value(argv[5]).toFloat();
//    float ea = JSC::Value(argv[6]).toFloat();
//    bool anticlockwise = argc == 8 ? JSC::Value(argv[7]).toBoolean() : true;

//    int dir = anticlockwise ? NVG_CCW : NVG_CW;

    nvgEllipse(canvas2d.vg,cx,cy,rx,ry);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::fill) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgFill(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::fillRect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();
    float w = JSC::Value(argv[2]).toFloat();
    float h = JSC::Value(argv[3]).toFloat();

    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, x, y, w, h);
    nvgFill(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::fillText) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    std::string text = JSC::Value(argv[0]).toString().getUTF8String();
    float x = JSC::Value(argv[1]).toFloat();
    float y = JSC::Value(argv[2]).toFloat();

    if (argc >= 4)
    {
        float maxWidth = JSC::Value(argv[3]).toFloat();
        nvgTextBox(canvas2d.vg, x, y, maxWidth, text.c_str(), nullptr);
    }
    else
    {
        nvgText(canvas2d.vg, x, y, text.c_str(), nullptr);
    }

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::getImageData) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    JSC::Value sx = argv[0];
    JSC::Value sy = argv[1];
    JSC::Value sw = argv[2];
    JSC::Value sh = argv[3];

    const unsigned length = sw.toInteger() * sh.toInteger() * 4;
    void* pixels = malloc(length);

    glReadBuffer(GL_BACK);
    glReadPixels(sx.toInteger(), sy.toInteger(), sw.toInteger(), sh.toInteger(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    JSC::Object typedArray = JSC::Object::MakeTypedArrayWithBytesNoCopy(kJSTypedArrayTypeUint8Array, pixels, length, [](void* ptr, void*){free(ptr);}, nullptr);

    JSC::Object imageData = JSC::Object::MakeDefault();
    imageData.setProperty("data", typedArray, kJSPropertyAttributeReadOnly);
    imageData.setProperty("width", sw, kJSPropertyAttributeReadOnly);
    imageData.setProperty("height", sh, kJSPropertyAttributeReadOnly);

    return imageData;
}

JSC_FUNCTION(CanvasRenderingContext2D::getLineDash) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::isPointInPath) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::isPointInStroke) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::lineTo) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();

    nvgLineTo(canvas2d.vg, x, y);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::measureText) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    std::string text = JSC::Value(argv[0]).toString().getUTF8String();

    float width =  nvgTextBounds(canvas2d.vg, 0, 0, text.c_str(), nullptr, nullptr);

    JSC::Object textMetrics = JSC::Object::MakeDefault();
    textMetrics.setProperty("width", width);

    return textMetrics;
}

JSC_FUNCTION(CanvasRenderingContext2D::moveTo) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();

    nvgMoveTo(canvas2d.vg, x, y);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::putImageData) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    JSC::Object imageData = JSC::Value(argv[0]).toObject();

    JSC::Value dx = argv[1];
    JSC::Value dy = argv[2];

    JSC::Object data = imageData.getProperty("data").toObject();
    float width = imageData.getProperty("width").toFloat();
    float height = imageData.getProperty("height").toFloat();
    void* pixels = data.getTypedArrayBytesPtr();

    int image = nvgCreateImageRGBA(canvas2d.vg, width, height, 0, (unsigned char*)pixels);

    NVGpaint imgPaint = nvgImagePattern(canvas2d.vg, 0, 0, width, height, 0.0f, image, 1.0f);
    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, dx.toFloat(), dy.toFloat(), width, height);
    nvgFillPaint(canvas2d.vg, imgPaint);
    nvgFill(canvas2d.vg);

    nvgDeleteImage(canvas2d.vg, image);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::quadraticCurveTo) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float cpx = JSC::Value(argv[0]).toFloat();
    float cpy = JSC::Value(argv[1]).toFloat();
    float x = JSC::Value(argv[2]).toFloat();
    float y = JSC::Value(argv[3]).toFloat();

    nvgQuadTo(canvas2d.vg, cpx, cpy, x, y);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::rect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();
    float w = JSC::Value(argv[2]).toFloat();
    float h = JSC::Value(argv[3]).toFloat();

    nvgRect(canvas2d.vg, x, y, w, h);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::removeHitRegion) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::resetTransform) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgResetTransform(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::restore) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgRestore(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::rotate) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float angle = JSC::Value(argv[0]).toFloat();

    nvgRotate(canvas2d.vg, angle);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::save) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgSave(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::scale) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();

    nvgScale(canvas2d.vg, x, y);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::scrollPathIntoView) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::setLineDash) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::setTransform) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float a = JSC::Value(argv[0]).toFloat();
    float b = JSC::Value(argv[1]).toFloat();
    float c = JSC::Value(argv[2]).toFloat();
    float d = JSC::Value(argv[3]).toFloat();
    float e = JSC::Value(argv[4]).toFloat();
    float f = JSC::Value(argv[5]).toFloat();

    nvgResetTransform(canvas2d.vg);
    nvgTransform(canvas2d.vg, a, b, c, d, e, f);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::stroke) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgStroke(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::strokeRect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();
    float w = JSC::Value(argv[2]).toFloat();
    float h = JSC::Value(argv[3]).toFloat();

    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, x, y, w, h);
    nvgStroke(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::strokeText) {
    THROW_UNIMPLEMENTED_ERROR();
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::transform) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float a = JSC::Value(argv[0]).toFloat();
    float b = JSC::Value(argv[1]).toFloat();
    float c = JSC::Value(argv[2]).toFloat();
    float d = JSC::Value(argv[3]).toFloat();
    float e = JSC::Value(argv[4]).toFloat();
    float f = JSC::Value(argv[5]).toFloat();

    nvgTransform(canvas2d.vg, a, b, c, d, e, f);


    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::translate) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    float x = JSC::Value(argv[0]).toFloat();
    float y = JSC::Value(argv[1]).toFloat();

    nvgTranslate(canvas2d.vg, x, y);

    return JSC::Value::MakeUndefined();
}


JSC::Class &CanvasRenderingContext2D::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {
            { "canvas",                   CanvasRenderingContext2D::getCanvas,                   CanvasRenderingContext2D::setCanvas,                   kJSPropertyAttributeDontDelete },
            { "currentTransform",         CanvasRenderingContext2D::getCurrentTransform,         CanvasRenderingContext2D::setCurrentTransform,         kJSPropertyAttributeDontDelete },
            { "direction",                CanvasRenderingContext2D::getDirection,                CanvasRenderingContext2D::setDirection,                kJSPropertyAttributeDontDelete },
            { "fillStyle",                CanvasRenderingContext2D::getFillStyle,                CanvasRenderingContext2D::setFillStyle,                kJSPropertyAttributeDontDelete },
            { "filter",                   CanvasRenderingContext2D::getFilter,                   CanvasRenderingContext2D::setFilter,                   kJSPropertyAttributeDontDelete },
            { "font",                     CanvasRenderingContext2D::getFont,                     CanvasRenderingContext2D::setFont,                     kJSPropertyAttributeDontDelete },
            { "globalAlpha",              CanvasRenderingContext2D::getGlobalAlpha,              CanvasRenderingContext2D::setGlobalAlpha,              kJSPropertyAttributeDontDelete },
            { "globalCompositeOperation", CanvasRenderingContext2D::getGlobalCompositeOperation, CanvasRenderingContext2D::setGlobalCompositeOperation, kJSPropertyAttributeDontDelete },
            { "imageSmoothingEnabled",    CanvasRenderingContext2D::getImageSmoothingEnabled,    CanvasRenderingContext2D::setImageSmoothingEnabled,    kJSPropertyAttributeDontDelete },
            { "imageSmoothingQuality",    CanvasRenderingContext2D::getImageSmoothingQuality,    CanvasRenderingContext2D::setImageSmoothingQuality,    kJSPropertyAttributeDontDelete },
            { "lineCap",                  CanvasRenderingContext2D::getLineCap,                  CanvasRenderingContext2D::setLineCap,                  kJSPropertyAttributeDontDelete },
            { "lineDashOffset",           CanvasRenderingContext2D::getLineDashOffset,           CanvasRenderingContext2D::setLineDashOffset,           kJSPropertyAttributeDontDelete },
            { "lineJoin",                 CanvasRenderingContext2D::getLineJoin,                 CanvasRenderingContext2D::setLineJoin,                 kJSPropertyAttributeDontDelete },
            { "lineWidth",                CanvasRenderingContext2D::getLineWidth,                CanvasRenderingContext2D::setLineWidth,                kJSPropertyAttributeDontDelete },
            { "miterLimit",               CanvasRenderingContext2D::getMiterLimit,               CanvasRenderingContext2D::setMiterLimit,               kJSPropertyAttributeDontDelete },
            { "shadowBlur",               CanvasRenderingContext2D::getShadowBlur,               CanvasRenderingContext2D::setShadowBlur,               kJSPropertyAttributeDontDelete },
            { "shadowColor",              CanvasRenderingContext2D::getShadowColor,              CanvasRenderingContext2D::setShadowColor,              kJSPropertyAttributeDontDelete },
            { "shadowOffsetX",            CanvasRenderingContext2D::getShadowOffsetX,            CanvasRenderingContext2D::setShadowOffsetX,            kJSPropertyAttributeDontDelete },
            { "shadowOffsetY",            CanvasRenderingContext2D::getShadowOffsetY,            CanvasRenderingContext2D::setShadowOffsetY,            kJSPropertyAttributeDontDelete },
            { "strokeStyle",              CanvasRenderingContext2D::getStrokeStyle,              CanvasRenderingContext2D::setStrokeStyle,              kJSPropertyAttributeDontDelete },
            { "textAlign",                CanvasRenderingContext2D::getTextAlign,                CanvasRenderingContext2D::setTextAlign,                kJSPropertyAttributeDontDelete },
            { "textBaseline",             CanvasRenderingContext2D::getTextBaseline,             CanvasRenderingContext2D::setTextBaseline,             kJSPropertyAttributeDontDelete },
            { 0, 0, 0, 0 }
        };

        static JSStaticFunction staticFunctions[] = {
            { "arc",                  CanvasRenderingContext2D::arc,                  kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "arcTo",                CanvasRenderingContext2D::arcTo,                kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "beginPath",            CanvasRenderingContext2D::beginPath,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "bezierCurveTo",        CanvasRenderingContext2D::bezierCurveTo,        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "clearHitRegions",      CanvasRenderingContext2D::clearHitRegions,      kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "clearRect",            CanvasRenderingContext2D::clearRect,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "clip",                 CanvasRenderingContext2D::clip,                 kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "closePath",            CanvasRenderingContext2D::closePath,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createImageData",      CanvasRenderingContext2D::createImageData,      kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createLinearGradient", CanvasRenderingContext2D::createLinearGradient, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createPattern",        CanvasRenderingContext2D::createPattern,        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createRadialGradient", CanvasRenderingContext2D::createRadialGradient, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawFocusIfNeeded",    CanvasRenderingContext2D::drawFocusIfNeeded,    kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawImage",            CanvasRenderingContext2D::drawImage,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawWidgetAsOnScreen", CanvasRenderingContext2D::drawWidgetAsOnScreen, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawWindow",           CanvasRenderingContext2D::drawWindow,           kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "ellipse",              CanvasRenderingContext2D::ellipse,              kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "fill",                 CanvasRenderingContext2D::fill,                 kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "fillRect",             CanvasRenderingContext2D::fillRect,             kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "fillText",             CanvasRenderingContext2D::fillText,             kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "getImageData",         CanvasRenderingContext2D::getImageData,         kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "getLineDash",          CanvasRenderingContext2D::getLineDash,          kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "isPointInPath",        CanvasRenderingContext2D::isPointInPath,        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "isPointInStroke",      CanvasRenderingContext2D::isPointInStroke,      kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "lineTo",               CanvasRenderingContext2D::lineTo,               kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "measureText",          CanvasRenderingContext2D::measureText,          kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "moveTo",               CanvasRenderingContext2D::moveTo,               kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "putImageData",         CanvasRenderingContext2D::putImageData,         kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "quadraticCurveTo",     CanvasRenderingContext2D::quadraticCurveTo,     kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "rect",                 CanvasRenderingContext2D::rect,                 kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "removeHitRegion",      CanvasRenderingContext2D::removeHitRegion,      kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "resetTransform",       CanvasRenderingContext2D::resetTransform,       kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "restore",              CanvasRenderingContext2D::restore,              kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "rotate",               CanvasRenderingContext2D::rotate,               kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "save",                 CanvasRenderingContext2D::save,                 kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "scale",                CanvasRenderingContext2D::scale,                kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "scrollPathIntoView",   CanvasRenderingContext2D::scrollPathIntoView,   kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "setLineDash",          CanvasRenderingContext2D::setLineDash,          kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "setTransform",         CanvasRenderingContext2D::setTransform,         kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "stroke",               CanvasRenderingContext2D::stroke,               kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "strokeRect",           CanvasRenderingContext2D::strokeRect,           kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "strokeText",           CanvasRenderingContext2D::strokeText,           kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "transform",            CanvasRenderingContext2D::transform,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "translate",            CanvasRenderingContext2D::translate,            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };


        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "CanvasRenderingContext2D";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = CanvasRenderingContext2D::Constructor;
        classDefinition.finalize = CanvasRenderingContext2D::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
