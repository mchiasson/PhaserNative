#include "CanvasRenderingContext2D.h"

#include "PhaserNativeWindow.h"
#include "ColorUtil.h"

#include <PhaserGL.h>

static void freePtr(void* ptr, void*)
{
    free(ptr);
}


JSC_INITIALIZER(CanvasRenderingContext2D::Initializer) {
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);
}

JSC_FINALIZER(CanvasRenderingContext2D::Finalizer) {
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}

JSC_FUNCTION(CanvasRenderingContext2D::fillRect) {

    size_t index = (size_t)JSObjectGetPrivate(object);
    CanvasRenderingContext2D &instance = _GetInstance(index);

    float x = JSC::Value(ctx, argv[0]).toNumber();
    float y = JSC::Value(ctx, argv[1]).toNumber();
    float w = JSC::Value(ctx, argv[2]).toNumber();
    float h = JSC::Value(ctx, argv[3]).toNumber();

    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, x, y, w, h);
    nvgFillColor(PhaserNativeWindow::vg, ColorUtil::stringToColor(instance.fillStyle.toString().getUTF8String()));
    nvgFill(PhaserNativeWindow::vg);

    return JSC::Value::MakeUndefined(ctx);
}

JSC_FUNCTION(CanvasRenderingContext2D::getImageData) {

    JSC::Value sx = JSC::Value(ctx, argv[0]);
    JSC::Value sy = JSC::Value(ctx, argv[1]);
    JSC::Value sw = JSC::Value(ctx, argv[2]);
    JSC::Value sh = JSC::Value(ctx, argv[3]);

    const unsigned length = sw.toInteger() * sh.toInteger() * 4;
    void* pixels = malloc(length);

    glReadBuffer(GL_BACK);
    glReadPixels(sx.toInteger(), sy.toInteger(), sw.toInteger(), sh.toInteger(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    JSC::Object data = JSC::Object(ctx, JSObjectMakeTypedArrayWithBytesNoCopy(ctx, kJSTypedArrayTypeUint8Array, pixels, length, freePtr, nullptr, nullptr));

    JSC::Object imageData = JSC::Object::MakeDefault(ctx);
    imageData.setReadOnlyProperty("data", data);
    imageData.setReadOnlyProperty("width", sw);
    imageData.setReadOnlyProperty("height", sh);

    return imageData;
}

JSC_FUNCTION(CanvasRenderingContext2D::putImageData)
{
    JSC::Object imageData = JSC::Value(ctx, argv[0]).toObject();

    float dx = JSC::Value(ctx, argv[1]).toNumber();
    float dy = JSC::Value(ctx, argv[2]).toNumber();

    JSC::Object data = imageData.getProperty("data").toObject();
    float width = imageData.getProperty("width").toNumber();
    float height = imageData.getProperty("height").toNumber();
    void* pixels = JSObjectGetTypedArrayBytesPtr(ctx, data, nullptr);

    int image = nvgCreateImageMem(PhaserNativeWindow::vg, 0, (unsigned char*)pixels, (width * height * 4));

    NVGpaint imgPaint = nvgImagePattern(PhaserNativeWindow::vg, 0, 0, width, height, 0.0f, image, 1.0f);
    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, dx, dy, width, height);
    nvgFillPaint(PhaserNativeWindow::vg, imgPaint);
    nvgFill(PhaserNativeWindow::vg);

    nvgDeleteImage(PhaserNativeWindow::vg, image);


    return JSC::Value::MakeUndefined(ctx);
}

JSC::Class &CanvasRenderingContext2D::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction context2DStaticFunctions[] = {
            { "fillRect", CanvasRenderingContext2D::fillRect, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "getImageData", CanvasRenderingContext2D::getImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "putImageData", CanvasRenderingContext2D::putImageData, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        static JSStaticValue context2DStaticValues[] = {
        { "fillStyle", CanvasRenderingContext2D::get_fillStyle, CanvasRenderingContext2D::set_fillStyle, kJSPropertyAttributeDontDelete },
        { 0, 0,0, 0 }
        };

        JSClassDefinition context2DClassDefinition = kJSClassDefinitionEmpty;
        context2DClassDefinition.className = "CanvasRenderingContext2D";
        context2DClassDefinition.attributes = kJSClassAttributeNone;
        context2DClassDefinition.staticFunctions = context2DStaticFunctions;
        context2DClassDefinition.staticValues = context2DStaticValues;
        context2DClassDefinition.initialize = CanvasRenderingContext2D::Initializer;
        context2DClassDefinition.finalize = CanvasRenderingContext2D::Finalizer;
        _class = JSC::Class(&context2DClassDefinition);
    }

    return _class;
}
