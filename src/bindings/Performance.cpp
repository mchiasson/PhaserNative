#include "Performance.h"
#include "SDL2/SDL_timer.h"

JSC_INITIALIZER(Performance::Initializer)
{
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);

}

JSC_FINALIZER(Performance::Finalizer)
{
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
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
