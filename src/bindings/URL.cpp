#include "URL.h"

void URL::RegisterClass() {
    JSC::Object url = JSC::Object::MakeFunctionWithCallback("URL", URL::url);
    JSC::Object createObjectURL = JSC::Object::MakeFunctionWithCallback("createObjectURL", URL::createObjectURL);
    JSC::Object revokeObjectURL = JSC::Object::MakeFunctionWithCallback("revokeObjectURL", URL::revokeObjectURL);

    url.setProperty("createObjectURL", createObjectURL);
    url.setProperty("revokeObjectURL", revokeObjectURL);

    JSC_GLOBAL_OBJECT.setProperty("URL", url);
}

JSC_FUNCTION(URL::url)
{
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(URL::createObjectURL)
{
    // not sure how this is supposed to be implemented, but I'll just return the
    // Blob itself for now.
    return JSC::Value(argv[0]).toObject();
}

JSC_FUNCTION(URL::revokeObjectURL) {
    return JSC::Value::MakeUndefined();
}
