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

    String() :
        m_string(nullptr)
    {
    }

    String(const JSStringRef &utf8) :
        m_string(utf8)
    {
    }

    String(const char* utf8) :
        m_string(JSStringCreateWithUTF8CString(utf8))
    {
    }

    String(const std::string &utf8) :
        m_string(JSStringCreateWithUTF8CString(utf8.c_str()))
    {
    }

    String(String&& other) :
        m_string(other.m_string)
    {
        other.m_string = nullptr;
    }

    ~String()
    {
        if (m_string)
        {
            JSStringRelease(m_string);
            m_string = nullptr;
        }
    }

    String& operator=(String&& other)
    {
        if (m_string)
        {
            JSStringRelease(m_string);
        }

        m_string = other.m_string;
        other.m_string = nullptr;

        return *this;
    }

    operator JSStringRef() const
    {
        return m_string;
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

    JSStringRef m_string;
};

}


#endif // JSCSTRING_H
