#pragma once

#include "JSC/JSCHelpers.h"


JSC_BINDINGS(Navigator) {
    JSC::evaluateScript(JSC::globalContext(),
                        "var navigator = {\n"
                    #if defined(__linux__)
                        "    userAgent: \"Linux\""
                    #endif
                        "}\n", "Navigator.h");
}
