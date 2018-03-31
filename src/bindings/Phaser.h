#pragma once

#include "JSC/JSCHelpers.h"

class Phaser : public JSC::Binding<Phaser>
{
public:

    static void Register(JSContextRef ctx);

};
