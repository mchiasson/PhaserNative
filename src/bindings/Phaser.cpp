#include "Phaser.h"

void Phaser::Register(JSContextRef ctx)
{
    JSC::evaluateScriptFromFile(ctx, "phaser.js");
}
