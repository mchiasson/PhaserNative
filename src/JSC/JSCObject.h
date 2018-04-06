#ifndef JSCOBJECT_H
#define JSCOBJECT_H

#include <chrono>
#include <unordered_map>
#include <vector>

#include "JSCString.h"

namespace JSC
{

class Class;
class Value;

class Object {
    friend class Value;
public:
    using TimeType = std::chrono::time_point<std::chrono::system_clock>;

    static Object MakeDefault();
    static Object Make(JSClassRef jsClass, void* data = nullptr);
    static Object MakeArray(JSValueRef* elements, unsigned length);
    static Object MakeTypedArray(JSTypedArrayType arrayType, size_t length);
    static Object MakeTypedArrayWithBytesNoCopy(JSTypedArrayType arrayType, void* bytes, size_t byteLength, JSTypedArrayBytesDeallocator bytesDeallocator, void* deallocatorContext);
    static Object MakeArrayBufferWithBytesNoCopy(void* bytes, size_t byteLength, JSTypedArrayBytesDeallocator bytesDeallocator, void* deallocatorContext);
    static Object MakeDate(TimeType time);
    static Object MakeError(const char *error, const char *stack = nullptr);
    static Object MakeFunctionWithCallback(const std::string &name, JSObjectCallAsFunctionCallback callAsFunction);
    static Object MakeConstructor(JSClassRef jsClass, JSObjectCallAsConstructorCallback callAsConstructor);

    static Object GetGlobalObject();

    Object() {}

    Object(JSObjectRef obj);
    Object(const Object& other);
    Object(Object&& other);
    ~Object();

    Object& operator=(Object&& other);
    bool operator==(const Object &other);
    bool operator!=(const Object &other);

    operator JSObjectRef() const;

    bool isFunction() const;
    Value callAsFunction(std::initializer_list<JSValueRef> args) const;
    Value callAsFunction(const Object& thiz, std::initializer_list<JSValueRef> args) const;
    Value callAsFunction(int nArgs, const JSValueRef args[]) const;
    Value callAsFunction(const Object& thiz, int nArgs, const JSValueRef args[]) const;


    bool isConstructor() const;
    Object callAsConstructor(std::initializer_list<JSValueRef> args) const;

    bool hasProperty(const String &propName) const;
    bool hasProperty(const char *propName) const;
    Value getProperty(const String& propName) const;
    Value getProperty(const char *propName) const;
    Value getPropertyAtIndex(unsigned int index) const;
    void setProperty(const String& propName, const Value& value, JSPropertyAttributes attr = kJSPropertyAttributeNone);
    void setProperty(const char *propName, const Value& value, JSPropertyAttributes attr = kJSPropertyAttributeNone);
    void setPropertyAtIndex(unsigned int index, const Value& value);
    bool deleteProperty(const String &propName);
    bool deleteProperty(const char *propName);
    std::vector<String> getPropertyNames() const;
    std::unordered_map<std::string, std::string> toJSONMap() const;

    void* getTypedArrayBytesPtr();
    size_t getTypedArrayLength();
    size_t getTypedArrayByteLength();
    size_t getTypedArrayByteOffset();
    Object getTypedArrayBuffer();

    void* getArrayBufferBytesPtr();
    size_t getArrayBufferByteLength();

    void protect();
    void unprotect();

    template<typename T>
    T getPrivate() const
    {
        return reinterpret_cast<T>(JSObjectGetPrivate(m_obj));
    }

    template<typename T>
    void setPrivate(T data)
    {
        JSObjectSetPrivate(m_obj, reinterpret_cast<void*>(data));
    }


private:

    JSObjectRef m_obj = nullptr;

};

}

#endif // JSCOBJECT_H
