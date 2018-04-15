#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>


class Navigator
{
public:

    const char* userAgent() const;

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Navigator>);
        i.property("userAgent", &Navigator::userAgent);
    }

};

DUK_CPP_DEF_CLASS_NAME(Navigator);
