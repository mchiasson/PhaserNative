#include "JSCObject.h"
#include "JSCValue.h"
#include "JSCException.h"

#include <sstream>

namespace JSC
{

Object Object::MakeDefault(JSContextRef ctx)
{
    return Object(ctx, JSObjectMake(ctx, nullptr, nullptr));
}


Object Object::Make(JSContextRef ctx, JSClassRef jsClass, void* data)
{
    return Object(ctx, JSObjectMake(ctx, jsClass, data));
}

Object Object::MakeArray(JSContextRef ctx, JSValueRef* elements, unsigned length)
{
    JSValueRef exn;
    Object arrayObject = Object(ctx, JSObjectMakeArray(ctx, length, elements, &exn));
    if (!arrayObject)
    {
        throw JSC::Exception(ctx, exn, "Failed to create an Array");
    }
    return arrayObject;
}

Object Object::MakeDate(JSContextRef ctx, TimeType time)
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    JSValueRef arguments[] =
    {
        JSC::Value::MakeNumber(ctx, duration_cast<milliseconds>(time.time_since_epoch()).count())
    };

    JSValueRef exn;
    Object dateObject = Object(ctx, JSObjectMakeDate(ctx, 1, arguments, &exn));
    if (!dateObject)
    {
        throw Exception(ctx, exn, "Failed to create Date");
    }
    return dateObject;
}

Object Object::MakeError(JSContextRef ctx, const char *error, const char *stack)
{
    auto errorMsg = Value(ctx, String(ctx, error));
    JSValueRef args[] = {errorMsg};
    if (stack)
    {
        Object errorConstructor = Object::getGlobalObject(ctx).getProperty("Error").toObject();
        Object jsError = errorConstructor.callAsConstructor({errorMsg});
        std::string fullStack = std::string(stack) + jsError.getProperty("stack").toString().getUTF8String();
        jsError.setProperty("stack", String(ctx, fullStack.c_str()));
        return jsError;
    }
    else
    {
        JSValueRef exn;
        Object errorObj = Object(ctx, JSObjectMakeError(ctx, 1, args, &exn));
        if (!errorObj)
        {
            throw Exception(ctx, exn, "Exception making error");
        }
        return errorObj;
    }
}

Object::operator Value() const
{
    return Value(m_context, m_obj);
}

Value Object::callAsFunction(std::initializer_list<JSValueRef> args) const
{
    return callAsFunction(nullptr, args.size(), args.begin());
}

Value Object::callAsFunction(const Object& thisObj, std::initializer_list<JSValueRef> args) const
{
    return callAsFunction((JSObjectRef)thisObj, args.size(), args.begin());
}

Value Object::callAsFunction(int nArgs, const JSValueRef args[]) const
{
    return callAsFunction(nullptr, nArgs, args);
}

Value Object::callAsFunction(const Object& thisObj, int nArgs, const JSValueRef args[]) const
{
    return callAsFunction(static_cast<JSObjectRef>(thisObj), nArgs, args);
}

Value Object::callAsFunction(JSObjectRef thisObj, int nArgs, const JSValueRef args[]) const
{
    JSValueRef exn;
    Value result = Value(m_context, JSObjectCallAsFunction(m_context, m_obj, thisObj, nArgs, args, &exn));
    if (!result)
    {
        throw JSC::Exception(m_context, exn, "Exception calling object as function");
    }
    return result;
}

Object Object::callAsConstructor(std::initializer_list<JSValueRef> args) const
{
    JSValueRef exn;
    Object result = Object(m_context, JSObjectCallAsConstructor(m_context, m_obj, args.size(), args.begin(), &exn));
    if (!result)
    {
        throw JSC::Exception(m_context, exn, "Exception calling object as constructor");
    }
    return result;
}

Value Object::getProperty(const JSC::String& propName) const {
    JSValueRef exn;
    Value property = Value(m_context, JSObjectGetProperty(m_context, m_obj, propName, &exn));
    if (!property)
    {
        std::stringstream ss;
        ss << "Failed to get property '" << propName.getUTF8String() << "'";
        throw JSC::Exception(m_context, exn, ss.str());
    }
    return Value(m_context, property);
}

Value Object::getPropertyAtIndex(unsigned int index) const {
    JSValueRef exn;
    Value property = Value(m_context, JSObjectGetPropertyAtIndex(m_context, m_obj, index, &exn));
    if (!property)
    {
        std::stringstream ss;
        ss << "Failed to get property at index " << index;
        throw JSC::Exception(m_context, exn, ss.str());
    }
    return property;
}

Value Object::getProperty(const char *propName) const {
    return getProperty(JSC::String(m_context, propName));
}

void Object::setProperty(const JSC::String& propName, const Value& value) {
    JSValueRef exn = nullptr;
    JSObjectSetProperty(m_context, m_obj, propName, value, kJSPropertyAttributeNone, &exn);
    if (exn)
    {
        std::stringstream ss;
        ss << "Failed to set property '" << propName.getUTF8String() << "'";
        throw JSC::Exception(m_context, exn, ss.str());
    }
}

void Object::setProperty(const char *propName, const Value& value) {
    setProperty(JSC::String(m_context, propName), value);
}

void Object::setReadOnlyProperty(const JSC::String& propName, const Value& value) {
    JSValueRef exn = nullptr;
    JSObjectSetProperty(m_context, m_obj, propName, value, kJSPropertyAttributeReadOnly, &exn);
    if (exn)
    {
        std::stringstream ss;
        ss << "Failed to set property '" << propName.getUTF8String() << "'";
        throw JSC::Exception(m_context, exn, ss.str());
    }
}

void Object::setReadOnlyProperty(const char *propName, const Value& value) {
    setReadOnlyProperty(JSC::String(m_context, propName), value);
}


void Object::setPropertyAtIndex(unsigned int index, const Value& value) {
    JSValueRef exn = nullptr;
    JSObjectSetPropertyAtIndex(m_context, m_obj, index, value, &exn);
    if (exn)
    {
        std::stringstream ss;
        ss << "Failed to set property at index " << index;
        throw JSC::Exception(m_context, exn, ss.str());
    }
}


std::vector<JSC::String> Object::getPropertyNames() const {
    auto namesRef = JSObjectCopyPropertyNames(m_context, m_obj);
    size_t count = JSPropertyNameArrayGetCount(namesRef);
    std::vector<JSC::String> names;
    names.reserve(count);
    for (size_t i = 0; i < count; i++)
    {
        names.push_back(JSC::String(m_context, JSPropertyNameArrayGetNameAtIndex(namesRef, i)));
    }
    JSPropertyNameArrayRelease(namesRef);
    return names;
}

std::unordered_map<std::string, std::string> Object::toJSONMap() const {
    std::unordered_map<std::string, std::string> map;
    auto namesRef = JSObjectCopyPropertyNames(m_context, m_obj);
    size_t count = JSPropertyNameArrayGetCount(namesRef);
    for (size_t i = 0; i < count; i++)
    {
        auto key = JSC::String(m_context, JSPropertyNameArrayGetNameAtIndex(namesRef, i));
        map.emplace(key.getUTF8String(), getProperty(key).createJSONString());
    }
    JSPropertyNameArrayRelease(namesRef);
    return map;
}

}
