#include "Console.h"

#include <sstream>

JSC_INITIALIZER(Console::Initializer)
{
    CreateInstance(object);
}

JSC_FINALIZER(Console::Finalizer)
{
    FreeInstance(object);
}

JSC_FUNCTION(Console::log) {
    try {
        std::stringstream output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output << ' '; }
            output << JSC::Value(argv[i]).toString().getUTF8String();
        }
        SDL_Log("%s\n", output.str().c_str());
    } catch(const std::exception & e) {
        *exception = JSC::Object::MakeError(e.what());
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Console::info) {
    try {
        std::stringstream output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output << ' '; }
            output << JSC::Value(argv[i]).toString().getUTF8String();
        }
        SDL_LogInfo(0, "%s\n", output.str().c_str());
    } catch(const std::exception & e) {
        *exception = JSC::Object::MakeError(e.what());
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Console::warn) {
    try {
        std::stringstream output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output << ' '; }
            output << JSC::Value(argv[i]).toString().getUTF8String();
        }
        SDL_LogWarn(0, "%s\n", output.str().c_str());
    } catch(const std::exception & e) {
        *exception = JSC::Object::MakeError(e.what());
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Console::error) {
    try {
        std::stringstream output;
        for(size_t i = 0; i < argc; i++) {
            if (i > 0){ output << ' '; }
            output << JSC::Value(argv[i]).toString().getUTF8String();
        }
        SDL_LogError(0, "%s\n", output.str().c_str());
    } catch(const std::exception & e) {
        *exception = JSC::Object::MakeError(e.what());
    }
    return JSC::Value::MakeUndefined();
}

JSC::Class &Console::GetClassRef()
{
    if (!_class) {

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
    }

    return _class;
}
