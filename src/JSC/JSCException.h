#ifndef JSCEXCEPTION_H
#define JSCEXCEPTION_H

#include <stdexcept>

#include <JavaScriptCore/JavaScript.h>

#include "JSCString.h"


namespace JSC
{

class Value;

class Exception : public std::exception
{
public:
    Exception(const char* msg);
    Exception(const std::string &msg);
    Exception(const Value &exception, const char* msg);
    Exception(const Value &exception, const std::string &msg);
    Exception(const Value &exception, const JSC::String &sourceURL);

    const std::string& getStack() const
    {
        return m_stack;
    }

    virtual const char* what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    void buildMessage(const Value &exception, const JSC::String &sourceURL, const char* errorMsg);

    std::string m_message;
    std::string m_stack;

};

}


#endif // JSCEXCEPTION_H
