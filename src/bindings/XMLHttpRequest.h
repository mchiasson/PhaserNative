#pragma once

#include "JSC/JSCHelpers.h"
#include <deque>

class XMLHttpRequest : public JSC::Binding<XMLHttpRequest> {
public:

    struct RequestData
    {
        std::string m_method;
        std::string m_url;
        bool m_async = true;
        std::string m_user;
        std::string m_password;
        JSObjectRef m_onLoad = nullptr;
        JSObjectRef m_onError = nullptr;
        JSObjectRef m_onProgress = nullptr;
    };


    static JSC::Class &GetClassRef();

    static void OnRequest(void* ptr);

private:

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(open);
    static JSC_FUNCTION(send);

    std::deque<RequestData> m_requestQueue;

};
