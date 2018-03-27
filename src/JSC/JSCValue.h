#ifndef JSCVALUE_H
#define JSCVALUE_H

#include <JavaScriptCore/JavaScript.h>

#include "JSCString.h"
#include "JSCObject.h"

namespace JSC
{

class Value : public boost::noncopyable
{
public:
    Value(JSContextRef context, JSValueRef value);
    Value(JSContextRef context, JSStringRef value);

    Value(const Value &o) : Value(o.getContext(), o.m_value)
    {

    }

    Value(const JSC::String &o) : Value(o.getContext(), o)
    {

    }

    Value& operator=(Value&& other)
    {
        m_context = other.m_context;
        m_value = other.m_value;
        other.m_value = NULL;
        return *this;
    };

    operator JSValueRef() const
    {
        return m_value;
    }

    JSType getType() const
    {
        return JSValueGetType(m_context, m_value);
    }

    bool isBoolean() const
    {
        return getType() == kJSTypeBoolean;
    }

    bool toBoolean() const
    {
        return JSValueToBoolean(getContext(), m_value);
    }

    bool isNumber() const
    {
        return getType() == kJSTypeNumber;
    }

    bool isNull() const
    {
        return getType() == kJSTypeNull;
    }

    bool isUndefined() const
    {
        return getType() == kJSTypeUndefined;
    }

    double toNumber() const
    {
        return JSValueToNumber(getContext(), m_value, nullptr);
    }

    int32_t toInteger() const
    {
        return static_cast<int32_t>(toNumber());
    }

    uint32_t asUnsignedInteger() const
    {
        return static_cast<uint32_t>(toNumber());
    }

    bool isObject() const
    {
        return getType() == kJSTypeObject;
    }

    JSC::Object toObject() const;

    bool isString() const
    {
        return getType() == kJSTypeString;
    }

    JSC::String toString() const;

    static Value MakeNumber(JSContextRef ctx, double value)
    {
        return Value(ctx, JSValueMakeNumber(ctx, value));
    }

    static Value MakeUndefined(JSContextRef ctx)
    {
        return Value(ctx, JSValueMakeUndefined(ctx));
    }

    static Value MakeNull(JSContextRef ctx)
    {
        return Value(ctx, JSValueMakeNull(ctx));
    }

    static Value MakeBoolean(JSContextRef ctx, bool value)
    {
        return Value(ctx, JSValueMakeBoolean(ctx, value));
    }

    static Value MakeString(JSContextRef ctx, const char* utf8)
    {
        return Value(ctx, JSC::String(ctx, utf8));
    }

    static Value MakeString(JSContextRef ctx, const std::string &utf8)
    {
        return MakeString(ctx, utf8.c_str());
    }

    std::string createJSONString(unsigned indent = 0) const;

    static Value MakeFromJSONString(const JSC::String& json);

    JSContextRef getContext() const
    {
        return m_context;
    }


private:
    void throwTypeException(const std::string &expectedType) const;

    JSContextRef m_context;
    JSValueRef m_value;
};

}

#endif // JSCVALUE_H
