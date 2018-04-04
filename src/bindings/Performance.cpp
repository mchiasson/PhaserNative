#include "Performance.h"
#include "SDL2/SDL_timer.h"

JSC_INITIALIZER(Performance::Initializer)
{
    CreateInstance(object);
}

JSC_FINALIZER(Performance::Finalizer)
{
    FreeInstance(object);
}

JSC_FUNCTION(Performance::now)
{
    return JSC::Value(((double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency()));
}

JSC::Class &Performance::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction performanceStaticFunctions[] = {
            { "now", Performance::now, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition performanceClassDefinition = kJSClassDefinitionEmpty;
        performanceClassDefinition.className = "performance";
        performanceClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        performanceClassDefinition.staticFunctions = performanceStaticFunctions;
        _class = JSC::Class(&performanceClassDefinition);
    }

    return _class;
}
