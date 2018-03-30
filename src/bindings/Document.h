#pragma once

#include "JSC/JSCHelpers.h"
#include <SDL2/SDL_log.h>

JSC_BINDINGS(Document) {

    auto ctx = JSC::globalContext();

    static JSStaticFunction documentStaticFunctions[] = {
        { "createElement", [](JSContextRef ctx, JSObjectRef function, JSObjectRef thiz, size_t argc, const JSValueRef argv[], JSValueRef* exception) -> JSValueRef {
              JSC::String elementName = JSC::Value(ctx, argv[0]).toString();
              SDL_LogWarn(0, "Document.createElement('%s') is currently not supported.", elementName.getUTF8String().c_str());
              return JSC::Object::Make(ctx, nullptr, nullptr);
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { 0, 0, 0 }
    };


    JSClassDefinition documentClassDefinition = kJSClassDefinitionEmpty;
    documentClassDefinition.className = "document";
    documentClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    documentClassDefinition.staticFunctions = documentStaticFunctions;
    JSClassRef document_class = JSClassCreate(&documentClassDefinition);

    JSC::String documentName = JSC::String(ctx, "document");
    JSC::Object documentObject = JSC::Object::Make(ctx, document_class);
    JSObjectSetProperty(ctx,
                        JSContextGetGlobalObject(ctx),
                        documentName,
                        documentObject,
                        kJSPropertyAttributeDontDelete, nullptr);



    JSClassDefinition documentElementClassDefinition = kJSClassDefinitionEmpty;
    documentElementClassDefinition.className = "documentElement";
    documentElementClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    JSClassRef documentElement_class = JSClassCreate(&documentElementClassDefinition);

    JSC::String documentElementName = JSC::String(ctx, "documentElement");
    JSC::Object documentElementObject = JSC::Object::Make(ctx, documentElement_class);
    JSObjectSetProperty(ctx,
                        documentObject,
                        documentElementName,
                        documentElementObject,
                        kJSPropertyAttributeDontDelete, nullptr);



}
