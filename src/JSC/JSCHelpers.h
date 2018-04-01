#pragma once

#include "JSCContext.h"
#include "JSCClass.h"


#include <vector>

#define JSC_GLOBAL_CTX JSC::GlobalContext::GetInstance()
#define JSC_GLOBAL_OBJECT JSC::Object::getGlobalObject()

#define JSC_CONSTRUCTOR(_CONSTRUCTOR_NAME_) \
    JSObjectRef _CONSTRUCTOR_NAME_(JSContextRef ctx, JSObjectRef object, size_t argc, const JSValueRef argv[], JSValueRef* exception)

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
            *exception = JSC::Object::MakeError("Requires to be allocated with 'new' before use.", nullptr);\
            return JSC::Value::MakeUndefined();\
        } else {\
            return _GetInstance(index)._NAME_;\
        }\
    }\
    static bool set_##_NAME_(JSContextRef ctx, JSObjectRef object, JSStringRef /*propertyName*/, JSValueRef value, JSValueRef* exception) {\
        size_t index = (size_t)JSObjectGetPrivate(object);\
        if (index == 0) {\
            *exception = JSC::Object::MakeError("Requires to be allocated with 'new' before use.", nullptr);\
            return false;\
        } else {\
            _GetInstance(index)._NAME_ = value;\
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

    static JSC::Object Create()
    {
        // this is how we force subclasses to write their GetClassRef
        // implementation. If you get an error on this line, you forgot to
        // implement 'static JSC::Class &GetClassRef()' in the subclass.
        return JSC::Object::Make(C::GetClassRef());
    }

    operator JSObjectRef() const {
        return object;
    }

    operator JSValueRef() const {
        return object;
    }

protected:

    static size_t _GetNextIndex()
    {
        size_t index = 1; // 0 is reserved.
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

    static C &_CreateInstance(JSC::Object object) {
        size_t index = _GetNextIndex();
        C &instance = _pool[index];
        instance.object = std::move(object);
        instance.object.setPrivate(index);
        return instance;
    }

    static void _FreeInstance(JSC::Object object) {
        size_t index = object.getPrivate<size_t>();
        _pool[index]._inUse = false;
    }

    static C &_GetInstance(size_t index) {
        return _pool[index];
    }

    static C &_GetInstance(JSC::Object object) {
        return _pool[object.getPrivate<size_t>()];
    }

    static size_t _GetInstanceCount() {}

    static std::vector<JSStaticFunction> staticFunctions;
    static std::vector<JSStaticValue> staticValues;

    static Class _class;

    JSC::Object object;

private:
    bool _inUse = false;
    static std::vector<C> _pool;

};

template<class C> Class Binding<C>::_class;
template<class C> std::vector<C> JSC::Binding<C>::_pool;


Value evaluateScriptFromFile(const char *path);
Value evaluateScriptFromFileHandler(FILE *f, const char *sourceURL);
Value evaluateScriptFromString(const std::string &scriptUTF8, const char *sourceURL);
Value evaluateScript(const String &scriptUTF8, const String &sourceURL);

}
