#include "Performance.h"
#include "SDL2/SDL_timer.h"

JSC_CONSTRUCTOR(Performance::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(Performance::Finalizer)
{
    FreeNativeInstance(object);
}

JSC_FUNCTION(Performance::now)
{
    return JSC::Value(((double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency()));
}

JSC::Class &Performance::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "now", Performance::now, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "performance";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = Performance::Constructor;
        classDefinition.finalize = Performance::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
