#include "CanvasRenderingContext2D.h"

#include "ColorUtil.h"
#include "Image.h"
#include "HTMLCanvasElement.h"

#include "PhaserNativeGraphics.h"

JSC_CONSTRUCTOR(CanvasRenderingContext2D::Constructor) {

    CanvasRenderingContext2D &canvas2d = CreateNativeInstance();

    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(JSC::Value(argv[0]).toUnsignedInteger());

    canvas2d.canvasIndex = JSC::Value(argv[0]);
    canvas2d.context = PhaserNativeCreateGLContext(canvas.window);

#ifdef NANOVG_GL3_IMPLEMENTATION
    // hack for core profile to make things work without using VAOs: create a
    // blank VAO and bind it.
    glGenVertexArrays(1, &canvas2d.vao);
    glBindVertexArray(canvas2d.vao);
#endif

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

    return canvas2d.object;

}

JSC_FINALIZER(CanvasRenderingContext2D::Finalizer) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

#ifdef NANOVG_GL3_IMPLEMENTATION
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &canvas2d.vao);
    canvas2d.vao = 0;
#endif

    PhaserNativeDestroyNanoVGContext(canvas2d.vg);
    canvas2d.vg = nullptr;
    canvas2d.fontIcons = 0;
    canvas2d.fontNormal = 0;
    canvas2d.fontBold = 0;
    canvas2d.fontEmoji = 0;

    PhaserNativeDestroyGLContext(canvas2d.context);
    canvas2d.context = nullptr;
    canvas2d.canvasIndex = 0;

    FreeNativeInstance(object);
}

JSC_FUNCTION(CanvasRenderingContext2D::fillRect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

    JSC::Value x = argv[0];
    JSC::Value y = argv[1];
    JSC::Value w = argv[2];
    JSC::Value h = argv[3];

    CanvasRenderingContext2D &instance = GetNativeInstance(object);

    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, x.toFloat(), y.toFloat(), w.toFloat(), h.toFloat());
    nvgFillColor(canvas2d.vg, ColorUtil::stringToColor(instance.m_fillStyle.toString().getUTF8String()));
    nvgFill(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::clearRect) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    JSC::Value x = argv[0];
    JSC::Value y = argv[1];
    JSC::Value w = argv[2];
    JSC::Value h = argv[3];

    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, x.toFloat(), y.toFloat(), w.toFloat(), h.toFloat());
    nvgFillColor(canvas2d.vg, {{{.0f, .0f, .0f, .0f}}});
    nvgFill(canvas2d.vg);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::createImageData) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


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

JSC_FUNCTION(CanvasRenderingContext2D::getImageData) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


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

JSC_FUNCTION(CanvasRenderingContext2D::putImageData) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    JSC::Object imageData = JSC::Value(argv[0]).toObject();

    JSC::Value dx = argv[1];
    JSC::Value dy = argv[2];

    JSC::Object data = imageData.getProperty("data").toObject();
    float width = imageData.getProperty("width").toFloat();
    float height = imageData.getProperty("height").toFloat();
    void* pixels = data.getTypedArrayBytesPtr();

    int image = nvgCreateImageMem(canvas2d.vg, 0, (unsigned char*)pixels, (width * height * 4));

    NVGpaint imgPaint = nvgImagePattern(canvas2d.vg, 0, 0, width, height, 0.0f, image, 1.0f);
    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, dx.toFloat(), dy.toFloat(), width, height);
    nvgFillPaint(canvas2d.vg, imgPaint);
    nvgFill(canvas2d.vg);

    nvgDeleteImage(canvas2d.vg, image);


    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::drawImage) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    Image &image = Image::GetNativeInstance(JSC::Value(argv[0]).toObject());

    JSC::Value sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight;

    if(argc == 3)
    {
        sx = JSC::Value(0);
        sy = JSC::Value(0);
        sWidth = image.object.getProperty("width");
        sHeight = image.object.getProperty("width");
        dx = argv[1];
        dy = argv[2];
        dWidth = image.object.getProperty("width");
        dHeight = image.object.getProperty("width");
    }
    else if (argc == 5)
    {
        sx = JSC::Value(0);
        sy = JSC::Value(0);
        sWidth = image.object.getProperty("width");
        sHeight = image.object.getProperty("width");
        dx = argv[1];
        dy = argv[2];
        dWidth = argv[3];
        dHeight = argv[4];
    }
    else if (argc == 9)
    {
        sx = argv[1];
        sy = argv[2];
        sWidth = argv[3];
        sHeight = argv[4];
        dx = argv[5];
        dy = argv[6];
        dWidth = argv[7];
        dHeight = argv[8];
    }

    int imgWidth = image.object.getProperty("width").toInteger();
    int imgHeight = image.object.getProperty("height").toInteger();
    int img = nvgCreateImageMem(canvas2d.vg, 0, (unsigned char*)image.m_pixels, (imgWidth * imgHeight * 4));

    NVGpaint imgPaint = nvgImagePattern(canvas2d.vg,
                                        sx.toFloat(),
                                        sy.toFloat(),
                                        sWidth.toFloat(),
                                        sHeight.toFloat(),
                                        0.0f,
                                        img,
                                        1.0f);

    nvgBeginPath(canvas2d.vg);
    nvgRect(canvas2d.vg, dx.toFloat(), dy.toFloat(), dWidth.toFloat(), dHeight.toFloat());
    nvgFillPaint(canvas2d.vg, imgPaint);
    nvgFill(canvas2d.vg);

    nvgDeleteImage(canvas2d.vg, img);

    return JSC::Value::MakeUndefined();

}

JSC_FUNCTION(CanvasRenderingContext2D::measureText) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    float bounds[4];
    nvgTextBounds(canvas2d.vg, 0, 0, JSC::Value(argv[0]).toString().getUTF8String().c_str(), nullptr, bounds);

    JSC::Object textMetric = JSC::Object::MakeDefault();
    textMetric.setProperty("width", JSC::Value(bounds[2] - bounds[0]));
    return textMetric;
}

JSC_FUNCTION(CanvasRenderingContext2D::fillText) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    std::string text = JSC::Value(argv[0]).toString().getUTF8String();
    JSC::Value x = argv[1];
    JSC::Value y = argv[2];

    nvgFontSize(canvas2d.vg, 24.0f);
    nvgFontFace(canvas2d.vg, "sans");
    nvgTextAlign(canvas2d.vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(canvas2d.vg, ColorUtil::stringToColor(canvas2d.m_fillStyle.toString().getUTF8String()));
    nvgText(canvas2d.vg, x.toFloat(), y.toFloat(), text.c_str(), nullptr);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::save) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    nvgSave(canvas2d.vg);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::restore) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);


    nvgRestore(canvas2d.vg);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::translate) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

    JSC::Value x = argv[0];
    JSC::Value y = argv[1];

    nvgTranslate(canvas2d.vg, x.toFloat(), y.toFloat());
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::rotate) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

    JSC::Value angle = argv[0];

    nvgRotate(canvas2d.vg, angle.toFloat());
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::scale) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

    JSC::Value x = argv[0];
    JSC::Value y = argv[1];

    nvgScale(canvas2d.vg, x.toFloat(), y.toFloat());

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::setTransform)
{

    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    PhaserNativeMakeCurrent(canvas.window, canvas2d.context, canvas2d.vg);

    JSC::Value a = argv[0];
    JSC::Value b = argv[1];
    JSC::Value c = argv[2];
    JSC::Value d = argv[3];
    JSC::Value e = argv[4];
    JSC::Value f = argv[5];

    nvgTransform(canvas2d.vg, a.toFloat(), b.toFloat(), c.toFloat(), d.toFloat(), e.toFloat(), f.toFloat());
    return JSC::Value::MakeUndefined();
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getFillStyle) {
    return GetNativeInstance(object).m_fillStyle;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setFillStyle) {
    GetNativeInstance(object).m_fillStyle = value;
    return true;
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getCanvas) {
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(canvas2d.canvasIndex);
    return canvas.object;
}

JSC::Class &CanvasRenderingContext2D::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "fillRect", CanvasRenderingContext2D::fillRect, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "clearRect", CanvasRenderingContext2D::clearRect, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createImageData", CanvasRenderingContext2D::createImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "getImageData", CanvasRenderingContext2D::getImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "putImageData", CanvasRenderingContext2D::putImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawImage", CanvasRenderingContext2D::drawImage, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "measureText", CanvasRenderingContext2D::measureText, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "fillText", CanvasRenderingContext2D::fillText, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "save", CanvasRenderingContext2D::save, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "restore", CanvasRenderingContext2D::restore, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "translate", CanvasRenderingContext2D::translate, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "rotate", CanvasRenderingContext2D::rotate, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "scale", CanvasRenderingContext2D::scale, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "setTransform", CanvasRenderingContext2D::setTransform, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        static JSStaticValue staticValues[] = {
            { "fillStyle", CanvasRenderingContext2D::getFillStyle, CanvasRenderingContext2D::setFillStyle, kJSPropertyAttributeDontDelete },
            { "canvas", CanvasRenderingContext2D::getCanvas, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0, 0 }
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
