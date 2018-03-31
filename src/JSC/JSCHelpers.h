#pragma once

#include "JSCContext.h"
#include "JSCClass.h"


#include <vector>

#define JSC_CONSTRUCTOR(_CONSTRUCTOR_NAME_) \
    JSObjectRef _CONSTRUCTOR_NAME_(JSContextRef ctx, JSObjectRef constructor, size_t argc, const JSValueRef argv[], JSValueRef* exception)

#define JSC_INITIALIZER(_INITIALIZER_NAME_) \
    void _INITIALIZER_NAME_(JSContextRef ctx, JSObjectRef object)

#define JSC_FINALIZER(_FINALIZER_NAME_) \
    void _FINALIZER_NAME_(JSObjectRef object)

#define JSC_FUNCTION(_FUNC_NAME_) \
    JSValueRef _FUNC_NAME_(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argc, const JSValueRef argv[], JSValueRef* exception)

#define JSC_RW_PROPERTY(_NAME_)\
    static JSValueRef get_##_NAME_(JSContextRef ctx, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef* exception) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        if (index == 0) {\
            JSC::Value message = JSC::Value::MakeString(ctx, "Requires to be allocated with 'new' before use.");\
            JSValueRef args[] { message, nullptr };\
            *exception = JSObjectMakeError(ctx, 1, args, nullptr);\
            return message;\
        } else {\
            return _GetInstance(index)._NAME_;\
        }\
    }\
    static bool set_##_NAME_(JSContextRef ctx, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef value, JSValueRef* exception) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        if (index == 0) {\
            JSC::Value message = JSC::Value::MakeString(ctx, "Requires to be allocated with 'new' before use.");\
            JSValueRef args[] { message, nullptr };\
            *exception = JSObjectMakeError(ctx, 1, args, nullptr);\
            return false;\
        } else {\
            _GetInstance(index)._NAME_ = JSC::Value(ctx, value);\
            return true;\
        }\
    }\
    JSC::Value _NAME_

#define JSC_R_PROPERTY(_NAME_)\
    static JSValueRef get_##_NAME_(JSContextRef /*ctx*/, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef* /*exception*/) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        return GetInstance(index)._NAME_;\
    }\
    JSC::Value _NAME_

namespace JSC
{

template<class C>
class Binding {
public:

    static JSC::Object Create(JSContextRef ctx)
    {
        // this is how we force subclasses to write their GetClassRef
        // implementation. If you get an error on this line, you forgot to
        // implement 'static JSC::Class &GetClassRef()' in the subclass.
        return JSC::Object::Make(ctx, C::GetClassRef());
    }

protected:

    static size_t _AllocateInstance()
    {
        size_t index = 1;
        bool found = false;
        for (index = 1; index < _pool.size(); ++index) {
            if (!_pool[index]._inUse) {
                found = true;
                break;
            }
        }
        if (!found) {
            _pool.resize(index+1);
        }
        _pool[index]._inUse = true;
        return index;
    }

    static void _FreeInstance(size_t index) {
        _pool[index]._inUse = false;
    }

    static C &_GetInstance(size_t index) {
        return _pool[index];
    }

    static size_t _GetInstanceCount() {}

    static std::vector<JSStaticFunction> staticFunctions;
    static std::vector<JSStaticValue> staticValues;

    static Class _class;

private:
    bool _inUse = false;
    static std::vector<C> _pool;

};

template<class C> Class Binding<C>::_class;
template<class C> std::vector<C> JSC::Binding<C>::_pool;


Value evaluateScriptFromFile(JSContextRef ctx, const char *path);
Value evaluateScriptFromFileHandler(JSContextRef ctx, FILE *f, const char *sourceURL);
Value evaluateScriptFromString(JSContextRef ctx, const std::string &scriptUTF8, const char *sourceURL);
Value evaluateScript(JSContextRef ctx, const String &scriptUTF8, const String &sourceURL);

}
