#include "JSCObject.h"
#include "JSCValue.h"
#include "JSCException.h"
#include "JSCHelpers.h"

#include <sstream>

namespace JSC
{

Object Object::MakeDefault()
{
    return JSObjectMake(JSC_GLOBAL_CTX, nullptr, nullptr);
}

Object Object::Make(JSClassRef jsClass, void* data)
{
    return JSObjectMake(JSC_GLOBAL_CTX, jsClass, data);
}

Object Object::MakeArray(JSValueRef* elements, unsigned length)
{
    JSValueRef exception;
    Object arrayObject = JSObjectMakeArray(JSC_GLOBAL_CTX, length, elements, &exception);
    if (!arrayObject)
    {
        throw Exception(exception, "Failed to create an Array");
    }
    return arrayObject;
}

Object Object::MakeTypedArray(JSTypedArrayType arrayType, size_t length)
{
    JSValueRef exception;
    Object typedArrayObject = JSObjectMakeTypedArray(JSC_GLOBAL_CTX, arrayType, length, &exception);
    if (!typedArrayObject)
    {
        throw Exception(exception, "Failed to create a Typed Array");
    }
    return typedArrayObject;
}

Object Object::MakeTypedArrayWithBytesNoCopy(JSTypedArrayType arrayType, void* bytes, size_t byteLength, JSTypedArrayBytesDeallocator bytesDeallocator, void* deallocatorContext)
{
    JSValueRef exception;

    Object typedArrayObject = JSObjectMakeTypedArrayWithBytesNoCopy(JSC_GLOBAL_CTX, arrayType, bytes, byteLength, bytesDeallocator, deallocatorContext, &exception);
    if (!typedArrayObject)
    {
        throw Exception(exception, "Failed to create a Typed Array with bytes");
    }
    return typedArrayObject;
}

Object Object::MakeArrayBufferWithBytesNoCopy(void* bytes, size_t byteLength, JSTypedArrayBytesDeallocator bytesDeallocator, void* deallocatorContext)
{
    JSValueRef exception;

    Object arrayBuffer = JSObjectMakeArrayBufferWithBytesNoCopy(JSC_GLOBAL_CTX, bytes, byteLength, bytesDeallocator, deallocatorContext, &exception);
    if (!arrayBuffer)
    {
        throw Exception(exception, "Failed to create a Typed Array with bytes");
    }
    return arrayBuffer;
}

Object Object::MakeDate(TimeType time)
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    JSValueRef arguments[] =
    {
        Value(static_cast<double>(duration_cast<milliseconds>(time.time_since_epoch()).count()))
    };

    JSValueRef exception;
    Object dateObject = JSObjectMakeDate(JSC_GLOBAL_CTX, 1, arguments, &exception);
    if (!dateObject)
    {
        throw Exception(exception, "Failed to create Date");
    }
    return dateObject;
}

Object Object::MakeError(const char *error, const char *stack)
{
    if (stack)
    {
        Object errorConstructor = JSC_GLOBAL_OBJECT.getProperty("Error").toObject();
        Object jsError = errorConstructor.callAsConstructor({Value(error)});
        std::string fullStack = std::string(stack) + jsError.getProperty("stack").toString().getUTF8String();
        jsError.setProperty("stack", Value(fullStack));
        return jsError;
    }
    else
    {
        JSValueRef args[] = {Value(error)};
        JSValueRef exception;

        Object errorObj = JSObjectMakeError(JSC_GLOBAL_CTX, 1, args, &exception);
        if (!errorObj)
        {
            throw Exception(exception, "Failed to create error"); // seriously?! lol!!!
        }
        return errorObj;
    }
}

Object Object::MakeFunctionWithCallback(const std::string &name, JSObjectCallAsFunctionCallback callAsFunction)
{
    return JSObjectMakeFunctionWithCallback(JSC_GLOBAL_CTX, String(name), callAsFunction);
}

Object Object::MakeConstructor(JSClassRef jsClass, JSObjectCallAsConstructorCallback callAsConstructor)
{
    return JSObjectMakeConstructor(JSC_GLOBAL_CTX, jsClass, callAsConstructor);
}

Object::Object(JSObjectRef obj) :
    m_obj(obj)
{
}

Object::Object(const Object& other) :
    m_obj(other.m_obj)
{

}

Object::Object(Object&& other) :
    m_obj(std::move(other.m_obj))
{
}


Object::~Object()
{
    if (m_obj)
    {
        m_obj = nullptr;
    }
}

Object& Object::operator=(const Object& other)
{
    m_obj = other.m_obj;
    return *this;

}

Object& Object::operator=(Object&& other)
{
    m_obj = other.m_obj;
    other.m_obj = nullptr;
    return *this;
}

bool Object::operator==(const Object &other)
{
    return JSValueIsStrictEqual(JSC_GLOBAL_CTX, m_obj, other.m_obj);
}

bool Object::operator!=(const Object &other)
{
    return !operator==(other);
}

Object Object::GetGlobalObject()
{
    return JSContextGetGlobalObject(JSC_GLOBAL_CTX);
}

Object::operator JSObjectRef() const
{
    return m_obj;
}

bool Object::isFunction() const
{
    return m_obj ? JSObjectIsFunction(JSC_GLOBAL_CTX, m_obj) : false;
}

Value Object::callAsFunction(std::initializer_list<JSValueRef> args) const
{
    return callAsFunction(nullptr, args.size(), args.begin());
}

Value Object::callAsFunction(const Object& thiz, std::initializer_list<JSValueRef> args) const
{
    return callAsFunction((JSObjectRef)thiz, args.size(), args.begin());
}

Value Object::callAsFunction(int nArgs, const JSValueRef args[]) const
{
    return callAsFunction(nullptr, nArgs, args);
}

Value Object::callAsFunction(const Object& thiz, int nArgs, const JSValueRef args[]) const
{
    JSValueRef exception;
    Value result = JSObjectCallAsFunction(JSC_GLOBAL_CTX, m_obj, thiz, nArgs, args, &exception);
    if (!result)
    {
        throw Exception(exception, "Exception calling object as function");
    }
    return result;
}

bool Object::isConstructor() const
{
    return m_obj ? JSObjectIsConstructor(JSC_GLOBAL_CTX, m_obj) : false;
}

Object Object::callAsConstructor(std::initializer_list<JSValueRef> args) const
{
    JSValueRef exception;
    Object result = JSObjectCallAsConstructor(JSC_GLOBAL_CTX, m_obj, args.size(), args.begin(), &exception);
    if (!result)
    {
        throw Exception(exception, "Exception calling object as constructor");
    }
    return result;
}

bool Object::hasProperty(const String &propName) const
{
    return JSObjectHasProperty(JSC_GLOBAL_CTX, m_obj, propName);
}

bool Object::hasProperty(const char *propName) const
{
    return hasProperty(String(propName));
}

Value Object::getProperty(const String& propName) const {
    JSValueRef exception;
    Value property = JSObjectGetProperty(JSC_GLOBAL_CTX, m_obj, propName, &exception);
    if (!property)
    {
        std::stringstream ss;
        ss << "Failed to get property '" << propName.getUTF8String() << "'";
        throw Exception(exception, ss.str());
    }
    return property;
}

Value Object::getPropertyAtIndex(unsigned int index) const {
    JSValueRef exception;
    Value property = JSObjectGetPropertyAtIndex(JSC_GLOBAL_CTX, m_obj, index, &exception);
    if (!property)
    {
        std::stringstream ss;
        ss << "Failed to get property at index " << index;
        throw Exception(exception, ss.str());
    }
    return property;
}

Value Object::getProperty(const char *propName) const {
    return getProperty(String(propName));
}

void Object::setProperty(const String& propName, const Value& value, JSPropertyAttributes attr) {
    JSValueRef exception = nullptr;
    JSObjectSetProperty(JSC_GLOBAL_CTX, m_obj, propName, value, attr, &exception);
    if (exception)
    {
        std::stringstream ss;
        ss << "Failed to set property '" << propName.getUTF8String() << "'";
        throw Exception(exception, ss.str());
    }
}

void Object::setProperty(const char *propName, const Value& value, JSPropertyAttributes attr) {
    setProperty(String(propName), value, attr);
}

void Object::setPropertyAtIndex(unsigned int index, const Value& value) {
    JSValueRef exception = nullptr;
    JSObjectSetPropertyAtIndex(JSC_GLOBAL_CTX, m_obj, index, value, &exception);
    if (exception)
    {
        std::stringstream ss;
        ss << "Failed to set property at index " << index;
        throw Exception(exception, ss.str());
    }
}

bool Object::deleteProperty(const String &propName)
{
    JSValueRef exception = nullptr;
    bool result = JSObjectDeleteProperty(JSC_GLOBAL_CTX, m_obj, propName, &exception);
    if (!result)
    {
        std::stringstream ss;
        ss << "Failed to delete property " << propName.getUTF8String();
        throw Exception(exception, ss.str());
    }
    return result;
}

bool Object::deleteProperty(const char *propName)
{
    return deleteProperty(String(propName));
}

std::vector<String> Object::getPropertyNames() const {
    auto namesRef = JSObjectCopyPropertyNames(JSC_GLOBAL_CTX, m_obj);
    size_t count = JSPropertyNameArrayGetCount(namesRef);
    std::vector<String> names;
    names.reserve(count);
    for (size_t i = 0; i < count; i++)
    {
        names.push_back(String(JSPropertyNameArrayGetNameAtIndex(namesRef, i)));
    }
    JSPropertyNameArrayRelease(namesRef);
    return names;
}

std::unordered_map<std::string, std::string> Object::toJSONMap() const {
    std::unordered_map<std::string, std::string> map;
    auto namesRef = JSObjectCopyPropertyNames(JSC_GLOBAL_CTX, m_obj);
    size_t count = JSPropertyNameArrayGetCount(namesRef);
    for (size_t i = 0; i < count; i++)
    {
        auto key = String(JSPropertyNameArrayGetNameAtIndex(namesRef, i));
        map.emplace(key.getUTF8String(), getProperty(key).createJSONString());
    }
    JSPropertyNameArrayRelease(namesRef);
    return map;
}

void* Object::getTypedArrayBytesPtr()
{
    void* ptr;
    JSValueRef exception;
    ptr = JSObjectGetTypedArrayBytesPtr(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!ptr)
    {
        throw Exception(exception, "Failed to get Typed Array bytes pointer");
    }
    return ptr;
}

size_t Object::getTypedArrayLength()
{
    size_t length;
    JSValueRef exception;
    length = JSObjectGetTypedArrayLength(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!length)
    {
        throw Exception(exception, "Failed to get Typed Array length");
    }
    return length;
}

size_t Object::getTypedArrayByteLength()
{
    size_t length;
    JSValueRef exception;
    length = JSObjectGetTypedArrayByteLength(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!length)
    {
        throw Exception(exception, "Failed to get Typed Array byte length");
    }
    return length;
}

size_t Object::getTypedArrayByteOffset()
{
    size_t offset;
    JSValueRef exception;
    offset = JSObjectGetTypedArrayByteOffset(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!offset)
    {
        throw Exception(exception, "Failed to get Typed Array byte offset");
    }
    return offset;
}

Object Object::getTypedArrayBuffer()
{
    JSValueRef exception;
    Object buffer = JSObjectGetTypedArrayBuffer(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!buffer)
    {
        throw Exception(exception, "Failed to get Typed Array buffer");
    }
    return buffer;
}

void* Object::getArrayBufferBytesPtr()
{
    JSValueRef exception;
    void* ptr = JSObjectGetArrayBufferBytesPtr(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!ptr)
    {
        throw Exception(exception, "Failed to get Typed Array buffer");
    }
    return ptr;
}

size_t Object::getArrayBufferByteLength()
{
    size_t length;
    JSValueRef exception;
    length = JSObjectGetArrayBufferByteLength(JSC_GLOBAL_CTX, m_obj, &exception);
    if (!length)
    {
        throw Exception(exception, "Failed to get Typed Array byte length");
    }
    return length;
}


void Object::protect()
{
    if (m_obj)
    {
        JSValueProtect(JSC_GLOBAL_CTX, m_obj);
    }
}

void Object::unprotect()
{
    if (m_obj)
    {
        JSValueUnprotect(JSC_GLOBAL_CTX, m_obj);
    }
}

}
