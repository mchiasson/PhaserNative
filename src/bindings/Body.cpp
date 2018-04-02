#include "Body.h"

JSC_INITIALIZER(Body::Initializer)
{
    _CreateInstance(object);
}

JSC_FINALIZER(Body::Finalizer)
{
    _FreeInstance(object);
}

JSC_FUNCTION(Body::appendChild) {
    return JSC::Value::MakeUndefined(); //Fake. Don't care about DOM.
}

JSC::Class &Body::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "appendChild", Body::appendChild, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };


        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Body";
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.initialize = Body::Initializer;
        classDefinition.finalize = Body::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
