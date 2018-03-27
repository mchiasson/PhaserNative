#include "JSCHelpers.h"

#include "JSCException.h"

namespace JSC
{

JSGlobalContextRef &globalContext() {
    static JSGlobalContextRef _ctx = nullptr;
    if (!_ctx) {
        _ctx = JSGlobalContextCreateInGroup(nullptr, nullptr);
    }
    return _ctx;
}

std::vector<void(*)()> &bindings() {
    static std::vector<void(*)()> _bindings;
    return _bindings;
}

Value evaluateScript(JSContextRef ctx, const std::string &script, const std::string &sourceURL)
{
    String jsScript = String(ctx, script);
    String jsSourceURL = String(ctx, sourceURL);
    JSValueRef exception = nullptr;
    Value result = Value(ctx, JSEvaluateScript(ctx, jsScript, NULL, jsSourceURL, 0, &exception));
    if (!result)
    {
        throw Exception(ctx, exception, jsSourceURL);
    }
    return result;
}

}
