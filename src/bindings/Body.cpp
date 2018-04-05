#include "Body.h"

JSC_CONSTRUCTOR(Body::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(Body::Finalizer)
{
    FreeNativeInstance(object);
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
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = Body::Constructor;
        classDefinition.finalize = Body::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
