#include "Document.h"
#include <SDL2/SDL_log.h>

#include "DocumentElement.h"
#include "Canvas.h"

JSC_INITIALIZER(Document::Initializer)
{
    size_t index = _AllocateInstance();
    JSC::Object instance = object;
    instance.setPrivate(index);

    instance.setProperty("documentElement", DocumentElement::Create());
    instance.setProperty("readyState", JSC::Value("complete")); // pretend the DOM is ready.
}

JSC_FINALIZER(Document::Finalizer)
{
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}

JSC_FUNCTION(Document::createElement) {
    JSC::String elementName = JSC::Value(argv[0]).toString();
    if (elementName == "canvas")
    {
        return Canvas::Create();
    }
    else
    {
        SDL_LogWarn(0, "Document.createElement('%s') is currently not supported.", elementName.getUTF8String().c_str());
        return JSC::Object::MakeDefault();
    }

}

JSC::Class &Document::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "createElement", Document::createElement, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };
    
        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Document";
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.initialize = Document::Initializer;
        classDefinition.finalize = Document::Finalizer;
        _class = JSC::Class(&classDefinition);
    }
    
    return _class;
}
