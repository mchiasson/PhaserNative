#include "URL.h"

JSC_CONSTRUCTOR(URL::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(URL::Finalizer)
{
    FreeNativeInstance(object);
}

JSC::Class &URL::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "URL";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = URL::Constructor;
        classDefinition.finalize = URL::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
