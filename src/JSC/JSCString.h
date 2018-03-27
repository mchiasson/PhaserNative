#ifndef JSCSTRING_H
#define JSCSTRING_H

#include <cassert>
#include <string>

#include <boost/noncopyable.hpp>
#include <JavaScriptCore/JavaScript.h>

namespace JSC
{

class String : public boost::noncopyable {
public:

    explicit String() :
        m_context(nullptr),
        m_string(nullptr)
    {
    }

    explicit String(JSContextRef context, const JSStringRef &utf8) :
        m_context(context),
        m_string(utf8)
    {
    }

    explicit String(JSContextRef context, const char* utf8) :
        m_context(context),
        m_string(JSStringCreateWithUTF8CString(utf8))
    {
    }

    explicit String(JSContextRef context, const std::string &utf8) :
        m_context(context),
        m_string(JSStringCreateWithUTF8CString(utf8.c_str()))
    {
    }

    String(String&& other) :
        m_context(other.m_context),
        m_string(other.m_string)
    {
        other.m_context = nullptr;
        other.m_string = nullptr;
    }

    ~String()
    {
        if (m_string)
        {
            JSStringRelease(m_string);
            m_string = nullptr;
        }
        m_context = nullptr;
    }

    String& operator=(String&& other)
    {
        if (m_string)
        {
            JSStringRelease(m_string);
        }

        m_context = other.m_context;
        m_string = other.m_string;
        other.m_context = nullptr;
        other.m_string = nullptr;

        return *this;
    }

    operator JSStringRef() const
    {
        return m_string;
    }

    JSContextRef getContext() const
    {
        return m_context;
    }

    size_t getLength() const
    {
        return m_string ? JSStringGetLength(m_string) : 0;
    }

    size_t getMaximumUTF8StringSize() const
    {
        return m_string ? JSStringGetMaximumUTF8CStringSize(m_string) : 0;
    }

    std::string getUTF8String() const
    {
        std::string string;
        if (m_string)
        {
            size_t maxBufferSize = JSStringGetMaximumUTF8CStringSize(m_string);
            char utf8Buffer[maxBufferSize];
            size_t bytesWritten = JSStringGetUTF8CString(m_string, utf8Buffer, maxBufferSize);
            string = std::string (utf8Buffer, bytesWritten -1);
        }
        return string;
    }

    bool operator==(const char* utf8)
    {
        return m_string ? JSStringIsEqualToUTF8CString(m_string, utf8) : false;
    }

    bool operator==(const std::string &utf8)
    {
        return operator==(utf8.c_str());
    }

    bool operator==(const String &other)
    {
        return m_string ? other ? JSStringIsEqual(m_string, other) : false : false;
    }

private:

    JSContextRef m_context;
    JSStringRef m_string;
};

}


#endif // JSCSTRING_H
