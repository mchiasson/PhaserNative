#include "JSCContext.h"

namespace JSC
{

GlobalContext *GlobalContext::context = nullptr;

GlobalContext &GlobalContext::GetInstance() {
    if (!context)
    {
        context = new GlobalContext();
    }

    return *context;
}

void GlobalContext::DestroyInstance()
{
    delete context;
    context = nullptr;
}

GlobalContext::GlobalContext() :
    m_ctx(JSGlobalContextCreateInGroup(nullptr, nullptr))
{
}

GlobalContext::~GlobalContext()
{
    JSGlobalContextRelease(m_ctx);
}

}
