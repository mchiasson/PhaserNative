#include "JSCValue.h"
#include "JSCObject.h"
#include "JSCException.h"
#include "JSCHelpers.h"

#include <JavaScriptCore/JavaScript.h>

#include <SDL2/SDL_log.h>

#include <limits>
#include <sstream>

namespace JSC
{

Value Value::MakeUndefined()
{
    return JSValueMakeUndefined(JSC_GLOBAL_CTX);
}

Value Value::MakeNull()
{
    return JSValueMakeNull(JSC_GLOBAL_CTX);
}

Value::Value(JSValueRef value) :
    m_value(value)
{

}

Value::Value(bool value) :
    m_value(JSValueMakeBoolean(JSC_GLOBAL_CTX, value))
{

}

Value::Value(double value) :
    m_value(JSValueMakeNumber(JSC_GLOBAL_CTX, value))
{

}

Value::Value(int32_t value) :
    m_value(JSValueMakeNumber(JSC_GLOBAL_CTX, value))
{

}
Value::Value(uint32_t value) :
    m_value(JSValueMakeNumber(JSC_GLOBAL_CTX, value))
{

}

Value::Value(int64_t value) :
    m_value(JSValueMakeNumber(JSC_GLOBAL_CTX, value))
{

}
Value::Value(uint64_t value) :
    m_value(JSValueMakeNumber(JSC_GLOBAL_CTX, value))
{

}

Value::Value(const char * str) :
    m_value(JSValueMakeString(JSC_GLOBAL_CTX, JSC::String(str)))
{

}

Value::Value(const std::string &str) :
    m_value(JSValueMakeString(JSC_GLOBAL_CTX, JSC::String(str)))
{

}

Value::Value(JSStringRef str) :
    m_value(JSValueMakeString(JSC_GLOBAL_CTX, str))
{

}

bool Value::operator==(const Value &other)
{
    // string means '===' in javascript
    return JSValueIsStrictEqual(JSC_GLOBAL_CTX, m_value, other.m_value);
}

bool Value::operator!=(const Value &other)
{
    return !operator==(other);
}

Value::operator JSValueRef() const
{
    return m_value;
}

JSType Value::getType() const
{
    if (m_value == nullptr) {
        return kJSTypeUndefined;
    }
    return JSValueGetType(JSC_GLOBAL_CTX, m_value);
}


std::string Value::createJSONString(unsigned indent) const
{
    JSValueRef exception;
    String json = JSValueCreateJSONString(JSC_GLOBAL_CTX, m_value, indent, &exception);
    if (!json)
    {
        throw Exception(exception, "Exception creating JSON string");
    }
    return json.getUTF8String();
}

Value Value::MakeFromJSONString(const std::string& json) {
    Value result = JSValueMakeFromJSONString(JSC_GLOBAL_CTX, JSC::String(json));
    if (!result)
    {
        std::stringstream ss;
        ss << "Failed to create Value from JSON: " << json;
        throw Exception(ss.str().c_str());
    }
    return result;
}

bool Value::toBoolean() const
{
    return JSValueToBoolean(JSC_GLOBAL_CTX, m_value);
}

float Value::toFloat() const
{
    return static_cast<float>(toDouble());
}

double Value::toDouble() const
{
    double value = JSValueToNumber(JSC_GLOBAL_CTX, m_value, nullptr);
    if (isnan(value))
    {
        throw Exception("Failed to convert to number");
    }
    return value;
}

int32_t Value::toInteger() const
{
    return static_cast<int32_t>(toDouble());
}

uint32_t Value::toUnsignedInteger() const
{
    return static_cast<uint32_t>(toDouble());
}

int64_t Value::toLongInteger() const
{
    return static_cast<int64_t>(toDouble());
}

uint64_t Value::toUnsignedLongInteger() const
{
    return static_cast<uint64_t>(toDouble());
}

Object Value::toObject() const
{
    JSValueRef exception;
    Object jsObj = JSValueToObject(JSC_GLOBAL_CTX, m_value, &exception);
    if (!jsObj)
    {
        throw Exception(exception, "Failed to convert to object");
    }
    return jsObj;
}

String Value::toString() const
{
    JSValueRef exception;
    String jsStr = JSValueToStringCopy(JSC_GLOBAL_CTX, m_value, &exception);
    if (!jsStr)
    {
        throw Exception(exception, "Failed to convert to string");
    }
    return jsStr;
}

void Value::protect()
{
    if (m_value)
    {
        JSValueProtect(JSC_GLOBAL_CTX, m_value);
    }
}

void Value::unprotect()
{
    if (m_value)
    {
        JSValueUnprotect(JSC_GLOBAL_CTX, m_value);
    }
}


}
