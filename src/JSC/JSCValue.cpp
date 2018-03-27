#include "JSCValue.h"
#include "JSCObject.h"
#include "JSCException.h"

#include <JavaScriptCore/JavaScript.h>

#include <SDL2/SDL_log.h>

#include <sstream>

namespace JSC
{

Value::Value(JSContextRef context, JSValueRef value)
    : m_context(context), m_value(value)
{

}

Value::Value(JSContextRef context, JSStringRef str)
    : m_context(context), m_value(JSValueMakeString(context, str))
{

}

std::string Value::createJSONString(unsigned indent) const
{
    JSValueRef exn;
    String json = String(m_context, JSValueCreateJSONString(m_context, m_value, indent, &exn));
    if (!json)
    {
        throw Exception(m_context, exn, "Exception creating JSON string");
    }
    return json.getUTF8String();
}

Value Value::MakeFromJSONString(const String& json) {
    JSContextRef ctx = json.getContext();
    auto result = JSValueMakeFromJSONString(ctx, json);
    if (!result)
    {
        std::stringstream ss;
        ss << "Failed to create Value from JSON: " << json.getUTF8String();
        throw Exception(ss.str().c_str());
    }
    return Value(ctx, result);
}

Object Value::toObject() const
{
    JSValueRef exn;
    Object jsObj = Object(m_context, JSValueToObject(m_context, m_value, &exn));
    if (!jsObj)
    {
        throw Exception(m_context, exn, "Failed to convert to object");
    }
    return jsObj;
}

String Value::toString() const
{
    JSValueRef exn;
    String jsStr = String(m_context, JSValueToStringCopy(m_context, m_value, &exn));
    if (!jsStr)
    {
        throw Exception(m_context, exn, "Failed to convert to string");
    }
    return jsStr;
}

void Value::throwTypeException(const std::string &expectedType) const
{
    std::string msg("TypeError: Expected ");
    msg += expectedType;
    msg += ", instead got '";
    msg += toString().getUTF8String();
    msg += "'";
    throw Exception(msg);
}

}
