#pragma once

#include "JSC/JSCHelpers.h"

class XMLHttpRequest : public JSC::Binding<XMLHttpRequest> {
public:

    static JSC::Class &GetClassRef();

private:

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(open);
    static JSC_FUNCTION(send);

    std::string m_method;
    std::string m_url;
    bool m_async = true;
    std::string m_user;
    std::string m_password;
};
