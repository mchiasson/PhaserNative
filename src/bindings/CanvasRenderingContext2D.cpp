#include "CanvasRenderingContext2D.h"

#include "PhaserNativeWindow.h"
#include "ColorUtil.h"

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

JSC::Class &CanvasRenderingContext2D::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction context2DStaticFunctions[] = {
        { "fillRect", CanvasRenderingContext2D::fillRect, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
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
