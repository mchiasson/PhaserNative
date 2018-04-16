#include "CanvasPattern.h"

JSC_CONSTRUCTOR(CanvasPattern::Constructor)
{
    CanvasPattern &Pattern = CreateNativeInstance();
    return Pattern.object;
}

JSC_FINALIZER(CanvasPattern::Finalizer)
{
    CanvasPattern &pattern = GetNativeInstance(object);

    pattern.imageID = 0;

    pattern.ox = 0;
    pattern.oy = 0;
    pattern.w = 0;
    pattern.h = 0;
    pattern.angle = 0;
    pattern.alpha = 1;

    FreeNativeInstance(object);
}

JSC::Class &CanvasPattern::GetClassRef()
{
    if (!_class)
    {
        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "CanvasPattern";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.callAsConstructor = CanvasPattern::Constructor;
        classDefinition.finalize = CanvasPattern::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
