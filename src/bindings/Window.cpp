#include "Window.h"
#include "Performance.h"

JSC_INITIALIZER(Window::Initializer)
{
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);

    JSC::Object(ctx, object).setProperty("performance", Performance::Create(ctx));
}

JSC_FINALIZER(Window::Finalizer)
{
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}

JSC::Class &Window::GetClassRef()
{
    if (!_class)
    {
        JSClassDefinition windowClassDefinition = kJSClassDefinitionEmpty;
        windowClassDefinition.className = "Window";
        windowClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        windowClassDefinition.initialize = Window::Initializer;
        windowClassDefinition.finalize = Window::Finalizer;
        _class = JSC::Class(&windowClassDefinition);
    }

    return _class;
}
