#include "Console.h"

JSC_INITIALIZER(Console::Initializer)
{
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);

}

JSC_FINALIZER(Console::Finalizer)
{
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}


JSC_FUNCTION(Console::log) {
    try {
        std::string output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output += " "; }
            output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
        }
        SDL_Log("%s\n", output.c_str());
    } catch(const std::exception & e) {
        JSC::Value message = JSC::Value::MakeString(ctx, e.what());
        JSValueRef args[] { message, nullptr };
        *exception = JSObjectMakeError(ctx, 1, args, nullptr);
    }
    return JSC::Value::MakeUndefined(ctx);
}

JSC_FUNCTION(Console::info) {
    try {
        std::string output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output += " "; }
            output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
        }
        SDL_LogInfo(0, "%s\n", output.c_str());
    } catch(const std::exception & e) {
        JSC::Value message = JSC::Value::MakeString(ctx, e.what());
        JSValueRef args[] { message, nullptr };
        *exception = JSObjectMakeError(ctx, 1, args, nullptr);
    }
    return JSC::Value::MakeUndefined(ctx);
}

JSC_FUNCTION(Console::warn) {
    try {
        std::string output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output += " "; }
            output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
        }
        SDL_LogWarn(0, "%s\n", output.c_str());
    } catch(const std::exception & e) {
        JSC::Value message = JSC::Value::MakeString(ctx, e.what());
        JSValueRef args[] { message, nullptr };
        *exception = JSObjectMakeError(ctx, 1, args, nullptr);
    }
    return JSC::Value::MakeUndefined(ctx);
}

JSC_FUNCTION(Console::error) {
    try {
        std::string output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output += " "; }
            output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
        }
        SDL_LogError(0, "%s\n", output.c_str());
    } catch(const std::exception & e) {
        JSC::Value message = JSC::Value::MakeString(ctx, e.what());
        JSValueRef args[] { message, nullptr };
        *exception = JSObjectMakeError(ctx, 1, args, nullptr);
    }
    return JSC::Value::MakeUndefined(ctx);
}

void Console::Register(JSContextRef ctx)
{

    static JSStaticFunction staticFunctions[] = {
        { "log", Console::log, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "info", Console::info, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "warn", Console::warn, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "error", Console::error, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { 0, 0, 0 }
    };

    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
    classDefinition.className = "Console";
    classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    classDefinition.staticFunctions = staticFunctions;
    classDefinition.initialize = Console::Initializer;
    classDefinition.finalize = Console::Finalizer;
    _class = JSC::Class(&classDefinition);

    // Register global instance
    JSC::String propertyName = JSC::String(ctx, "console");
    JSObjectSetProperty(ctx,
                        JSContextGetGlobalObject(ctx),
                        propertyName,
                        JSC::Object::Make(ctx, _class),
                        kJSPropertyAttributeDontDelete, nullptr);
}
