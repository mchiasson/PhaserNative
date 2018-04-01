#include "Document.h"
#include <SDL2/SDL_log.h>

#include "DocumentElement.h"
#include "Canvas.h"

JSC_INITIALIZER(Document::Initializer)
{
    Document &instance = _CreateInstance(object);

    instance.object.setProperty("documentElement", DocumentElement::Create());
    instance.object.setProperty("readyState", JSC::Value("complete")); // pretend the DOM is ready.
}

JSC_FINALIZER(Document::Finalizer)
{
    _FreeInstance(object);
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
