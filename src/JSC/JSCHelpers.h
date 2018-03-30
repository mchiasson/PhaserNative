#pragma once

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

struct JSCClass
{
    bool _inUse = false;
};

#define JSC_CLASS(_CLASS_NAME_)\
    struct _CLASS_NAME_;\
    static std::vector<_CLASS_NAME_> _instances;\
    struct _CLASS_NAME_ : public JSCClass

#define JSC_CONSTRUCTOR(_CONSTRUCTOR_NAME_) static JSObjectRef _CONSTRUCTOR_NAME_(JSContextRef ctx, JSObjectRef constructor, size_t argc, const JSValueRef argv[], JSValueRef* exception)
#define JSC_DESTRUCTOR(_DESTRUCTOR_NAME_) static void _DESTRUCTOR_NAME_(JSObjectRef object)

#define JSC_GET_INSTANCE(_INSTANCE_VAR_NAME_) auto &_INSTANCE_VAR_NAME_ = _instances[(size_t)JSObjectGetPrivate(object)]


#define JSC_CREATE_INSTANCE(_INSTANCE_VAR_NAME_)\
    size_t index = 0;\
    bool found = false;\
    for (index = 0; index < _instances.size(); ++index) { if (!_instances[index]._inUse) { found = true; break;} }\
    if (!found) {_instances.resize(_instances.size()+1);}\
    JSObjectSetPrivate(constructor, (void*)index);\
    auto &_INSTANCE_VAR_NAME_ = _instances[index];\
    _INSTANCE_VAR_NAME_._inUse = true

#define JSC_DESTROY_INSTANCE(_INSTANCE_VAR_NAME_) _INSTANCE_VAR_NAME_._inUse = false

#define JSC_RW_PROPERTY(_NAME_)\
    JSValueRef _NAME_ = nullptr;\
    static JSValueRef get_##_NAME_(JSContextRef /*ctx*/, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef* /*exception*/) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        return _instances[index]._NAME_;\
    }\
    static bool set_##_NAME_(JSContextRef /*ctx*/, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef value, JSValueRef* /*exception*/) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        _instances[index]._NAME_ = value;\
        return true;\
    }

#define JSC_R_PROPERTY(_NAME_)\
    JSValueRef _NAME_ = nullptr;\
    static JSValueRef get_##_NAME_(JSContextRef /*ctx*/, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef* /*exception*/) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        return _instances[index]._NAME_;\
    }

namespace JSC
{

void registerAllBindings();

int evaluateFromFile(const char *path);
int evaluateFromFileHandler(FILE *f, const char *sourceURL);
int evaluateFromString(const std::string &scriptUTF8, const char *sourceURL);

JSGlobalContextRef &globalContext();
std::vector<void(*)()> &bindings();
Value evaluateScript(JSContextRef ctx, const std::string &script, const std::string &sourceURL);

}
