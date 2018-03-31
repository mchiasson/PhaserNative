#include "Canvas.h"
#include "CanvasRenderingContext2D.h"

JSC_INITIALIZER(Canvas::Initializer) {
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);
    JSC::Object(ctx, object).setProperty("2d", CanvasRenderingContext2D::Create(ctx));
}

JSC_FINALIZER(Canvas::Finalizer) {
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}

JSC_FUNCTION(Canvas::getContext) {
    JSC::String elementName = JSC::Value(ctx, argv[0]).toString();
    JSValueRef contextObject = JSObjectGetProperty(ctx, object, elementName, exception);
    return contextObject;
}

JSC::Class &Canvas::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction canvasStaticFunctions[] = {
            { "getContext", Canvas::getContext, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition canvasClassDefinition = kJSClassDefinitionEmpty;
        canvasClassDefinition.className = "Canvas";
        canvasClassDefinition.attributes = kJSClassAttributeNone;
        canvasClassDefinition.staticFunctions = canvasStaticFunctions;
        canvasClassDefinition.initialize = Canvas::Initializer;
        canvasClassDefinition.finalize = Canvas::Finalizer;
        _class = JSC::Class(&canvasClassDefinition);
    }

    return _class;
}
