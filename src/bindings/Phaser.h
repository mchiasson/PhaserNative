#pragma once

#include "JSC/JSCHelpers.h"

JSC_BINDINGS(Phaser)
{
    JSC::evaluateFromFile("phaser.js");
}
