#ifndef JSCOBJECT_H
#define JSCOBJECT_H

#include <chrono>
#include <unordered_map>
#include <vector>

#include "JSCString.h"

namespace JSC
{

class Value;

class Object : public boost::noncopyable {
public:
    using TimeType = std::chrono::time_point<std::chrono::system_clock>;

    static Object MakeDefault(JSContextRef ctx);
    static Object Make(JSContextRef ctx, JSClassRef jsClass, void* data = nullptr);
    static Object MakeArray(JSContextRef ctx, JSValueRef* elements, unsigned length);
    static Object MakeDate(JSContextRef ctx, TimeType time);
    static Object MakeError(JSContextRef ctx, const char *error, const char *stack = nullptr);

    static Object getGlobalObject(JSContextRef ctx)
    {
        return Object(ctx, JSContextGetGlobalObject(ctx));
    }

    explicit Object(JSContextRef context, JSObjectRef obj) :
        m_context(context),
        m_obj(obj)
    {
    }

    Object(Object&& other) :
        m_context(other.m_context),
        m_obj(other.m_obj),
        m_isProtected(other.m_isProtected)
    {
        other.m_context = nullptr;
        other.m_obj = nullptr;
        other.m_isProtected = false;
    }

    ~Object()
    {
        if (m_obj)
        {
            if (m_isProtected)
            {
                JSValueUnprotect(m_context, m_obj);
            }
            m_obj = nullptr;
        }

        m_context = nullptr;
    }

    Object& operator=(Object&& other)
    {
        if (m_obj && m_isProtected)
        {
            JSValueUnprotect(m_context, m_obj);
        }

        m_context     = other.m_context;
        m_obj         = other.m_obj;
        m_isProtected = other.m_isProtected;

        other.m_context = nullptr;
        other.m_obj = nullptr;
        other.m_isProtected = false;

        return *this;
    }

    operator JSObjectRef() const
    {
        return m_obj;
    }

    operator Value() const;

    bool isFunction() const
    {
        return m_obj ? JSObjectIsFunction(m_context, m_obj) : false;
    }
    Value callAsFunction(std::initializer_list<JSValueRef> args) const;
    Value callAsFunction(const Object& thisObj, std::initializer_list<JSValueRef> args) const;
    Value callAsFunction(int nArgs, const JSValueRef args[]) const;
    Value callAsFunction(const Object& thisObj, int nArgs, const JSValueRef args[]) const;


    bool isConstructor() const
    {
        return m_obj ? JSObjectIsConstructor(m_context, m_obj) : false;
    }

    Object callAsConstructor(std::initializer_list<JSValueRef> args) const;

    Value getProperty(const JSC::String& propName) const;
    Value getProperty(const char *propName) const;
    Value getPropertyAtIndex(unsigned int index) const;
    void setProperty(const JSC::String& propName, const Value& value);
    void setProperty(const char *propName, const Value& value);
    void setReadOnlyProperty(const JSC::String& propName, const Value& value);
    void setReadOnlyProperty(const char *propName, const Value& value);
    void setPropertyAtIndex(unsigned int index, const Value& value);
    std::vector<JSC::String> getPropertyNames() const;
    std::unordered_map<std::string, std::string> toJSONMap() const;

    void protect()
    {
        if (m_obj && !m_isProtected)
        {
            JSValueProtect(m_context, m_obj);
            m_isProtected = true;
        }
    }

    void unprotect()
    {
        if (m_obj && m_isProtected)
        {
            JSValueUnprotect(m_context, m_obj);
            m_isProtected = false;
        }
    }

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

    JSContextRef context() const
    {
        return m_context;
    }


private:
    Value callAsFunction(JSObjectRef thisObj, int nArgs, const JSValueRef args[]) const;

    JSContextRef m_context;
    JSObjectRef m_obj;
    bool m_isProtected = false;

};

}

#endif // JSCOBJECT_H
