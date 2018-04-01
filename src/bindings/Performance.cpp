#include "Performance.h"
#include "SDL2/SDL_timer.h"

JSC_INITIALIZER(Performance::Initializer)
{
    _CreateInstance(object);
}

JSC_FINALIZER(Performance::Finalizer)
{
    _FreeInstance(object);
}

JSC_FUNCTION(Performance::now)
{
    return JSC::Value(SDL_GetTicks());
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
