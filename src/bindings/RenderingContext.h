#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>

struct RenderingContext
{

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<RenderingContext>);
    }
};

DUK_CPP_DEF_CLASS_NAME(RenderingContext);
