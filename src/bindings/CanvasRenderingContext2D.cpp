#include "CanvasRenderingContext2D.h"

#include "PhaserNativeWindow.h"
#include "ColorUtil.h"
#include "Image.h"

#include <PhaserGL.h>

JSC_CONSTRUCTOR(CanvasRenderingContext2D::Constructor) {
    return CreateNativeInstance().object;
}

JSC_FINALIZER(CanvasRenderingContext2D::Finalizer) {

    FreeNativeInstance(object);
}

JSC_FUNCTION(CanvasRenderingContext2D::fillRect) {

    JSC::Value x = argv[0];
    JSC::Value y = argv[1];
    JSC::Value w = argv[2];
    JSC::Value h = argv[3];

    CanvasRenderingContext2D &instance = GetNativeInstance(object);

    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, x.toFloat(), y.toFloat(), w.toFloat(), h.toFloat());
    nvgFillColor(PhaserNativeWindow::vg, ColorUtil::stringToColor(instance.m_fillStyle.toString().getUTF8String()));
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

    JSC::Object typedArray = JSC::Object::MakeTypedArrayWithBytesNoCopy(kJSTypedArrayTypeUint8Array, pixels, length, [](void* ptr, void*){free(ptr);}, nullptr);

    JSC::Object imageData = JSC::Object::MakeDefault();
    imageData.setProperty("data", typedArray, kJSPropertyAttributeReadOnly);
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

JSC_FUNCTION(CanvasRenderingContext2D::drawImage) {

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
    int img = nvgCreateImageMem(PhaserNativeWindow::vg, 0, (unsigned char*)image.m_pixels, (imgWidth * imgHeight * 4));

    NVGpaint imgPaint = nvgImagePattern(PhaserNativeWindow::vg,
                                        sx.toFloat(),
                                        sy.toFloat(),
                                        sWidth.toFloat(),
                                        sHeight.toFloat(),
                                        0.0f,
                                        img,
                                        1.0f);

    nvgBeginPath(PhaserNativeWindow::vg);
    nvgRect(PhaserNativeWindow::vg, dx.toFloat(), dy.toFloat(), dWidth.toFloat(), dHeight.toFloat());
    nvgFillPaint(PhaserNativeWindow::vg, imgPaint);
    nvgFill(PhaserNativeWindow::vg);

    nvgDeleteImage(PhaserNativeWindow::vg, img);

    return JSC::Value::MakeUndefined();

}

JSC_FUNCTION(CanvasRenderingContext2D::measureText) {
    float bounds[4];
    nvgTextBounds(PhaserNativeWindow::vg, 0, 0, JSC::Value(argv[0]).toString().getUTF8String().c_str(), nullptr, bounds);

    JSC::Object textMetric = JSC::Object::MakeDefault();
    textMetric.setProperty("width", JSC::Value(bounds[2] - bounds[0]));
    return textMetric;
}

JSC_FUNCTION(CanvasRenderingContext2D::fillText) {

    std::string text = JSC::Value(argv[0]).toString().getUTF8String();
    JSC::Value x = argv[1];
    JSC::Value y = argv[2];
    CanvasRenderingContext2D &canvas2d = GetNativeInstance(object);

    nvgFillColor(PhaserNativeWindow::vg, ColorUtil::stringToColor(canvas2d.m_fillStyle.toString().getUTF8String()));
    nvgText(PhaserNativeWindow::vg, x.toFloat(), y.toFloat(), text.c_str(), nullptr);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::save)
{
    nvgSave(PhaserNativeWindow::vg);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::restore)
{
    nvgRestore(PhaserNativeWindow::vg);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(CanvasRenderingContext2D::translate)
{
    JSC::Value x = argv[0];
    JSC::Value y = argv[1];

    nvgTranslate(PhaserNativeWindow::vg, x.toFloat(), y.toFloat());
    return JSC::Value::MakeUndefined();
}

JSC_PROPERTY_GET(CanvasRenderingContext2D::getFillStyle)
{
    return GetNativeInstance(object).m_fillStyle;
}

JSC_PROPERTY_SET(CanvasRenderingContext2D::setFillStyle)
{
    GetNativeInstance(object).m_fillStyle = value;
    return true;
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
            { "drawImage", CanvasRenderingContext2D::drawImage, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "measureText", CanvasRenderingContext2D::measureText, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "fillText", CanvasRenderingContext2D::fillText, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "save", CanvasRenderingContext2D::save, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "restore", CanvasRenderingContext2D::restore, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "translate", CanvasRenderingContext2D::translate, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        static JSStaticValue staticValues[] = {
            { "fillStyle", CanvasRenderingContext2D::getFillStyle, CanvasRenderingContext2D::setFillStyle, kJSPropertyAttributeDontDelete },
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
