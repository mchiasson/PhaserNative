#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>

class Window
{
public:

    static void OnRequestAnimationFrame(void* ptr, double timestamp);

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Window>);
    }
};

DUK_CPP_DEF_CLASS_NAME(Window);
