#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <boost/noncopyable.hpp>

namespace JSC
{
    class Class : public boost::noncopyable
    {
    public:
        Class() :
            m_class(nullptr)
        {

        }

        Class(const JSClassDefinition* definition) :
            m_class(JSClassCreate(definition))
        {

        }

        ~Class()
        {
            if (m_class)
            {
                JSClassRelease(m_class);
                m_class = nullptr;
            }
        }

        Class& operator=(Class&& other)
        {
            if (m_class)
            {
                JSClassRelease(m_class);
            }
            m_class = other.m_class;
            other.m_class = NULL;
            return *this;
        };

        operator JSClassRef()
        {
            return m_class;
        }

    private:

        JSClassRef m_class;
    };

}

