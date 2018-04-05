#include "JSCContext.h"

namespace JSC
{

GlobalContext &GlobalContext::GetNativeInstance() {
    static GlobalContext context;
    return context;
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
