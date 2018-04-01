#include "JSCException.h"

#include <SDL2/SDL_log.h>
#include <sstream>

#include "JSCObject.h"
#include "JSCValue.h"

namespace JSC
{

Exception::Exception(const char* msg)
    : m_message(msg)
{

}

Exception::Exception(const std::string &msg)
    : m_message(msg)
{

}

Exception::Exception(const Value &exception, const char* msg)
{
    buildMessage(exception, JSC::String(), msg);
}

Exception::Exception(const Value &exception, const std::string &msg)
{
    buildMessage(exception, JSC::String(), msg.c_str());
}

Exception::Exception(const Value &exception, const JSC::String &sourceURL)
{
    buildMessage(exception, sourceURL, nullptr);
}

void Exception::buildMessage(const Value &exception, const JSC::String &sourceURL, const char* errorMsg)
{
    std::stringstream msg;
    if (errorMsg && strlen(errorMsg) > 0)
    {
        msg << errorMsg << ": ";
    }

    JSC::Object exnObject = exception.toObject();
    Value exnMessage = exnObject.getProperty("message");
    if (exnMessage.isString())
    {
        msg << exnMessage.toString().getUTF8String();
    }
    else
    {
        msg << exception.toString().getUTF8String();
    }

    std::string locationInfo = sourceURL.getUTF8String();

    if (locationInfo.empty())
    {
        locationInfo += "<unknown file>";
    }

    Value line = exnObject.getProperty("line");

    if (line && line.isNumber())
    {
        std::stringstream ss;
        ss << ":" << line.toInteger();

        Value column = exnObject.getProperty("column");
        if (column && column.isNumber())
        {
            ss << ":" << column.toInteger();
        }

        locationInfo += ss.str();
    }

    if (!locationInfo.empty())
    {
        msg << " (" << locationInfo << ")";
    }

    m_message = msg.str();
    SDL_LogError(0, "JSC::Exception: %s\n", m_message.c_str());

    Value jsStack = exnObject.getProperty("stack");
    if (jsStack.isString())
    {
        m_stack = jsStack.toString().getUTF8String();
        SDL_LogError(0, "Stack:\n%s\n", m_stack.c_str());
    }
}

}
