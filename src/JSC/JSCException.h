#ifndef JSCEXCEPTION_H
#define JSCEXCEPTION_H

#include <stdexcept>

#include <JavaScriptCore/JavaScript.h>

#include "JSCString.h"


namespace JSC
{


class Exception : public std::exception
{
public:
    explicit Exception(const char* msg)
        : m_message(msg)
    {

    }

    explicit Exception(const std::string &msg)
        : m_message(msg)
    {

    }

    explicit Exception(JSContextRef ctx, JSValueRef exn, const char* msg)
    {
        buildMessage(ctx, exn, JSC::String(), msg);
    }

    explicit Exception(JSContextRef ctx, JSValueRef exn, const std::string &msg)
    {
        buildMessage(ctx, exn, JSC::String(), msg.c_str());
    }

    explicit Exception(JSContextRef ctx, JSValueRef exn, const JSC::String &sourceURL)
    {
        buildMessage(ctx, exn, sourceURL, nullptr);
    }

    const std::string& getStack() const
    {
        return m_stack;
    }

    virtual const char* what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    void buildMessage(JSContextRef ctx, JSValueRef exn, const JSC::String &sourceURL, const char* errorMsg);

    std::string m_message;
    std::string m_stack;

};

}


#endif // JSCEXCEPTION_H
