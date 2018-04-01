#include "WebGLRenderingContext.h"

JSC_INITIALIZER(WebGLRenderingContext::Initializer) {
    _CreateInstance(object);
}

JSC_FINALIZER(WebGLRenderingContext::Finalizer) {
    _FreeInstance(object);
}

JSC::Class &WebGLRenderingContext::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { 0, 0, 0 }
        };

        static JSStaticValue staticValues[] = {
        { 0, 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "WebGLRenderingContext";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.initialize = WebGLRenderingContext::Initializer;
        classDefinition.finalize = WebGLRenderingContext::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
