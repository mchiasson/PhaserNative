#ifndef COLORUTIL_H
#define COLORUTIL_H

#include "nanovg/nanovg.h"

#include <string>

struct ColorUtil
{
    static NVGcolor stringToColor(const std::string &color);
};

#endif // COLORUTIL_H
