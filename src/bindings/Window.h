#pragma once

#include "JSC/JSCHelpers.h"
#include <SDL2/SDL.h>

JSC_BINDINGS(Window)
{
    auto ctx = JSC::globalContext();

    JSClassDefinition windowClassDefinition = kJSClassDefinitionEmpty;
    windowClassDefinition.className = "window";
    windowClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    JSClassRef window_class = JSClassCreate(&windowClassDefinition);

    JSC::String windowName = JSC::String(ctx, "window");
    JSC::Object windowObject = JSC::Object::Make(ctx, window_class);
    JSObjectSetProperty(ctx,
                        JSContextGetGlobalObject(ctx),
                        windowName,
                        windowObject,
                        kJSPropertyAttributeDontDelete, nullptr);

    static JSStaticFunction performanceStaticFunctions[] = {
        { "now", [](JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef /*thiz*/, size_t /*argc*/, const JSValueRef /*argv*/[], JSValueRef* /*exception*/) -> JSValueRef {
              return JSC::Value::MakeNumber(ctx, SDL_GetTicks());
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { 0, 0, 0 }
    };

    JSClassDefinition performanceClassDefinition = kJSClassDefinitionEmpty;
    performanceClassDefinition.className = "performance";
    performanceClassDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    performanceClassDefinition.staticFunctions = performanceStaticFunctions;
    JSClassRef performance_class = JSClassCreate(&performanceClassDefinition);

    JSC::String performanceName = JSC::String(ctx, "performance");
    JSC::Object performanceObject = JSC::Object::Make(ctx, performance_class);
    JSObjectSetProperty(ctx,
                        windowObject,
                        performanceName,
                        performanceObject,
                        kJSPropertyAttributeDontDelete, nullptr);

}
