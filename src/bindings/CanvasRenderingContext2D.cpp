#include "CanvasRenderingContext2D.h"

#include "PhaserNativeWindow.h"
#include "ColorUtil.h"

#include <PhaserGL.h>

JSC_INITIALIZER(CanvasRenderingContext2D::Initializer) {
    size_t index = _AllocateInstance();
    JSC::Object(object).setPrivate(index);
}

JSC_FINALIZER(CanvasRenderingContext2D::Finalizer) {
    size_t index = JSC::Object(object).getPrivate<size_t>();
    _FreeInstance(index);
}

JSC_FUNCTION(CanvasRenderingContext2D::fillRect) {

    size_t index = JSC::Object(object).getPrivate<size_t>();
    CanvasRenderingContext2D &instance = _GetInstance(index);

    JSC::Value x = argv[0];
    JSC::Value y = argv[1];
    JSC::Value w = argv[2];
    JSC::Value h = argv[3];

    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, x.toFloat(), y.toFloat(), w.toFloat(), h.toFloat());
    nvgFillColor(PhaserNativeWindow::vg, ColorUtil::stringToColor(instance.fillStyle.toString().getUTF8String()));
    nvgFill(PhaserNativeWindow::vg);

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

JSC_FUNCTION(CanvasRenderingContext2D::getImageData) {

    JSC::Value sx = argv[0];
    JSC::Value sy = argv[1];
    JSC::Value sw = argv[2];
    JSC::Value sh = argv[3];

    const unsigned length = sw.toInteger() * sh.toInteger() * 4;
    void* pixels = malloc(length);

    glReadBuffer(GL_BACK);
    glReadPixels(sx.toInteger(), sy.toInteger(), sw.toInteger(), sh.toInteger(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    JSC::Object data = JSC::Object::MakeTypedArrayWithBytesNoCopy(kJSTypedArrayTypeUint8Array, pixels, length, [](void* ptr, void*){free(ptr);}, nullptr);

    JSC::Object imageData = JSC::Object::MakeDefault();
    imageData.setProperty("data", data, kJSPropertyAttributeReadOnly);
    imageData.setProperty("width", sw, kJSPropertyAttributeReadOnly);
    imageData.setProperty("height", sh, kJSPropertyAttributeReadOnly);

    return imageData;
}

JSC_FUNCTION(CanvasRenderingContext2D::putImageData)
{
    JSC::Object imageData = JSC::Value(argv[0]).toObject();

    JSC::Value dx = argv[1];
    JSC::Value dy = argv[2];

    JSC::Object data = imageData.getProperty("data").toObject();
    float width = imageData.getProperty("width").toFloat();
    float height = imageData.getProperty("height").toFloat();
    void* pixels = data.getTypedArrayBytesPtr();

    int image = nvgCreateImageMem(PhaserNativeWindow::vg, 0, (unsigned char*)pixels, (width * height * 4));

    NVGpaint imgPaint = nvgImagePattern(PhaserNativeWindow::vg, 0, 0, width, height, 0.0f, image, 1.0f);
    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, dx.toFloat(), dy.toFloat(), width, height);
    nvgFillPaint(PhaserNativeWindow::vg, imgPaint);
    nvgFill(PhaserNativeWindow::vg);

    nvgDeleteImage(PhaserNativeWindow::vg, image);


    return JSC::Value::MakeUndefined();
}

JSC::Class &CanvasRenderingContext2D::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "fillRect", CanvasRenderingContext2D::fillRect, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "createImageData", CanvasRenderingContext2D::createImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "getImageData", CanvasRenderingContext2D::getImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "putImageData", CanvasRenderingContext2D::putImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        static JSStaticValue staticValues[] = {
            { "fillStyle", CanvasRenderingContext2D::get_fillStyle, CanvasRenderingContext2D::set_fillStyle, kJSPropertyAttributeDontDelete },
            { 0, 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "CanvasRenderingContext2D";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.initialize = CanvasRenderingContext2D::Initializer;
        classDefinition.finalize = CanvasRenderingContext2D::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
