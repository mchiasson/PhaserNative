#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>

struct DOMRectReadOnly
{
    int x;
    int getX() const { return x; }

    int y;
    int getY() const { return y; }

    int width;
    int getWidth() const { return width; }

    int height;
    int getHeight() const { return height; }

    int getTop() const { return y; }
    int getRight() const { return x + width; }
    int getBottom() const { return y + height; }
    int getLeft() const { return x; }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<DOMRectReadOnly>);
        i.property("x", &DOMRectReadOnly::getX);
        i.property("y", &DOMRectReadOnly::getY);
        i.property("width", &DOMRectReadOnly::getWidth);
        i.property("height", &DOMRectReadOnly::getHeight);
        i.property("top", &DOMRectReadOnly::getTop);
        i.property("right", &DOMRectReadOnly::getRight);
        i.property("bottom", &DOMRectReadOnly::getBottom);
        i.property("left", &DOMRectReadOnly::getLeft);
    }

};

DUK_CPP_DEF_CLASS_NAME(DOMRectReadOnly);
