#ifndef JSCVALUE_H
#define JSCVALUE_H

#include <JavaScriptCore/JavaScript.h>

#include "JSCString.h"
#include "JSCObject.h"

namespace JSC
{

class Value
{
public:

    static Value MakeUndefined();
    static Value MakeNull();
    static Value MakeFromJSONString(const std::string& json);


    Value() :
        m_value(nullptr)
    {
    }

    Value(JSValueRef value);
    Value(bool value);
    Value(double value);
    Value(int32_t value);
    Value(uint32_t value);
    Value(const char * str);
    Value(const std::string &str);
    Value(JSStringRef str);

    Value(const Value &o) :
        m_value(o.m_value)
    {
    }

    Value& operator=(const Value& other)
    {
        m_value = other.m_value;
        return *this;
    };


    Value& operator=(Value&& other)
    {
        m_value = other.m_value;
        other.m_value = nullptr;
        return *this;
    };

    operator JSValueRef() const;

    JSType getType() const;
    bool isUndefined() const { return getType() == kJSTypeUndefined; }
    bool isNull() const      { return getType() == kJSTypeNull; }
    bool isBoolean() const   { return getType() == kJSTypeBoolean; }
    bool isNumber() const    { return getType() == kJSTypeNumber; }
    bool isString() const    { return getType() == kJSTypeString; }
    bool isObject() const    { return getType() == kJSTypeObject; }

    bool toBoolean() const;
    float toFloat() const;
    double toDouble() const;
    int32_t toInteger() const;
    uint32_t toUnsignedInteger() const;
    JSC::String toString() const;
    JSC::Object toObject() const;

    std::string createJSONString(unsigned indent = 0) const;

private:

    JSValueRef m_value;
};

}

#endif // JSCVALUE_H
