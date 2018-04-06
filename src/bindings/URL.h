#pragma once

#include "JSC/JSCHelpers.h"


class URL {
public:
    static void RegisterClass();

    static JSC_FUNCTION(url);
    static JSC_FUNCTION(createObjectURL);
    static JSC_FUNCTION(revokeObjectURL);
};
