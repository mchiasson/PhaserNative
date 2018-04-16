#include "CanvasGradient.h"

#include "ColorUtil.h"

JSC_CONSTRUCTOR(CanvasGradient::Constructor)
{
    CanvasGradient &gradient = CreateNativeInstance();
    return gradient.object;
}

JSC_FINALIZER(CanvasGradient::Finalizer)
{
    CanvasGradient &gradient = GetNativeInstance(object);
    gradient.x0 = 0;
    gradient.y0 = 0;
    gradient.r0 = 0;
    gradient.x1 = 0;
    gradient.y1 = 0;
    gradient.r1 = 0;
    gradient.isRadial = false;
    gradient.colorStop.clear();
    FreeNativeInstance(object);
}

JSC_FUNCTION(CanvasGradient::addColorStop)
{
    CanvasGradient &gradient = GetNativeInstance(object);

    float offset = JSC::Value(argv[0]).toFloat();
    NVGcolor color = ColorUtil::stringToColor(JSC::Value(argv[1]).toString().getUTF8String());

    gradient.colorStop[offset] = color;

    return JSC::Value::MakeUndefined();
}


JSC::Class &CanvasGradient::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "addColorStop", CanvasGradient::addColorStop, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 },
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "CanvasGradient";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = CanvasGradient::Constructor;
        classDefinition.finalize = CanvasGradient::Finalizer;
        _class = JSC::Class(&classDefinition);

    }

    return _class;
}
