#include "Document.h"
#include <SDL2/SDL_log.h>

#include "DocumentElement.h"
#include "HTMLCanvasElement.h"
#include "Body.h"

JSC_CONSTRUCTOR(Document::Constructor)
{
    Document &document = CreateNativeInstance();

    document.object.setProperty("body", Body::CreateJSObject({}));
    document.object.setProperty("documentElement", DocumentElement::CreateJSObject({}));
    document.object.setProperty("readyState", JSC::Value("complete")); // pretend the DOM is ready.

    return document.object;
}

JSC_FINALIZER(Document::Finalizer)
{
    FreeNativeInstance(object);
}

JSC_FUNCTION(Document::createElement) {
    std::string elementName = JSC::Value(argv[0]).toString().getUTF8String();
    SDL_Log("Document.createElement('%s')\n", elementName.c_str());
    if (elementName == "canvas")
    {
        return HTMLCanvasElement::CreateJSObject({});
    }
    else
    {
        SDL_LogWarn(0, "Document.createElement('%s') is currently not supported.\n", elementName.c_str());
        return JSC::Object::MakeDefault();
    }
}

JSC::Class &Document::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "createElement", Document::createElement, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };
    
        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Document";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = Document::Constructor;
        classDefinition.finalize = Document::Finalizer;
        _class = JSC::Class(&classDefinition);
    }
    
    return _class;
}
