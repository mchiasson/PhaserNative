#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>


class Performance
{
public:

    double now();

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Performance>);
        i.method("now", &Performance::now);
    }

};

DUK_CPP_DEF_CLASS_NAME(Performance);
