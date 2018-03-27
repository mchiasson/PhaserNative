#pragma once

#include <JavaScriptCore/JavaScript.h>

#include "JSCValue.h"

#define JSC_STRIGNIFY(a) _JSC_STRIGNIFY(a)
#define _JSC_STRIGNIFY(a) #a

#define JSC_BINDINGS(name)                                                     \
    static struct JSCBindingInitializer_##name {                               \
        JSCBindingInitializer_##name();                                        \
        static void registerBindings();                                        \
    } JSCBindingInitializer_##name##_instance;                                 \
    JSCBindingInitializer_##name::JSCBindingInitializer_##name() {             \
        JSC::bindings().push_back(&JSCBindingInitializer_##name::registerBindings); \
    }                                                                          \
    void JSCBindingInitializer_##name::registerBindings()


namespace JSC
{

JSGlobalContextRef &globalContext();
std::vector<void(*)()> &bindings();
Value evaluateScript(JSContextRef ctx, const std::string &script, const std::string &sourceURL);

}
