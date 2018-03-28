#include "JSCException.h"

#include <SDL2/SDL_log.h>
#include <sstream>

#include "JSCObject.h"
#include "JSCValue.h"

namespace JSC
{

void Exception::buildMessage(JSContextRef ctx, JSValueRef exn, const JSC::String &sourceURL, const char* errorMsg)
{
    std::stringstream msg;
    if (errorMsg && strlen(errorMsg) > 0)
    {
        msg << errorMsg << ": ";
    }

    JSC::Object exnObject = Value(ctx, exn).toObject();
    Value exnMessage = exnObject.getProperty("message");
    if (exnMessage.isString())
    {
        msg << exnMessage.toString().getUTF8String();
    }
    else
    {
        msg << ((Value)exnObject).toString().getUTF8String();
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
