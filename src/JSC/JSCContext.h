#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <boost/noncopyable.hpp>

#include "JSCValue.h"

namespace JSC
{
    class GlobalContext : public boost::noncopyable
    {
        GlobalContext();

    public:

        typedef void(*BindingInitializer)(GlobalContext &ctx);

        static GlobalContext &GetNativeInstance();
        ~GlobalContext();

        operator JSGlobalContextRef() const
        {
            return m_ctx;
        }

        operator JSContextRef() const
        {
            return m_ctx;
        }

    private:

        JSGlobalContextRef m_ctx;

    };
}
