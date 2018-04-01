#include "WebGLRenderingContext.h"

JSC_INITIALIZER(WebGLRenderingContext::Initializer) {
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);
}

JSC_FINALIZER(WebGLRenderingContext::Finalizer) {
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
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
