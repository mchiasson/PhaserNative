#include "KeyboardEvent.h"
#include "PhaserNativeEvent.h"

#include <SDL2/SDL_log.h>

JSC_CONSTRUCTOR(KeyboardEvent::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(KeyboardEvent::Finalizer)
{
    SDL_Log("KeyboardEvent count: %zu\n", KeyboardEvent::GetNativeInstanceCount());
    FreeNativeInstance(object);
}

JSC_FUNCTION(KeyboardEvent::preventDefault)
{
    return JSC::Value::MakeUndefined();
}

JSC::Class &KeyboardEvent::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "preventDefault", &KeyboardEvent::preventDefault, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "KeyboardEvent";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = KeyboardEvent::Constructor;
        classDefinition.finalize = KeyboardEvent::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}

static int scancodeToWebKeycode(SDL_Scancode scancode)
{
    switch (scancode)
    {
    case SDL_SCANCODE_BACKSPACE: return 8;
    case SDL_SCANCODE_TAB: return 9;
    case SDL_SCANCODE_RETURN: return 13;
    case SDL_SCANCODE_LSHIFT: return 16;
    case SDL_SCANCODE_RSHIFT: return 16;
    case SDL_SCANCODE_LCTRL: return 17;
    case SDL_SCANCODE_RCTRL: return 17;
    case SDL_SCANCODE_LALT: return 18;
    case SDL_SCANCODE_RALT: return 18;
    case SDL_SCANCODE_PAUSE: return 19;
    case SDL_SCANCODE_CAPSLOCK: return 20;
    case SDL_SCANCODE_ESCAPE: return 27;
    case SDL_SCANCODE_SPACE: return 32;
    case SDL_SCANCODE_PAGEUP: return 33;
    case SDL_SCANCODE_PAGEDOWN: return 34;
    case SDL_SCANCODE_END: return 35;
    case SDL_SCANCODE_HOME: return 36;
    case SDL_SCANCODE_LEFT: return 37;
    case SDL_SCANCODE_UP: return 38;
    case SDL_SCANCODE_RIGHT: return 39;
    case SDL_SCANCODE_DOWN: return 40;
    case SDL_SCANCODE_PRINTSCREEN: return 42;
    case SDL_SCANCODE_INSERT: return 45;
    case SDL_SCANCODE_DELETE: return 46;
    case SDL_SCANCODE_0: return 48;
    case SDL_SCANCODE_1: return 49;
    case SDL_SCANCODE_2: return 50;
    case SDL_SCANCODE_3: return 51;
    case SDL_SCANCODE_4: return 52;
    case SDL_SCANCODE_5: return 53;
    case SDL_SCANCODE_6: return 54;
    case SDL_SCANCODE_7: return 55;
    case SDL_SCANCODE_8: return 56;
    case SDL_SCANCODE_9: return 57;
    case SDL_SCANCODE_A: return 65;
    case SDL_SCANCODE_B: return 66;
    case SDL_SCANCODE_C: return 67;
    case SDL_SCANCODE_D: return 68;
    case SDL_SCANCODE_E: return 69;
    case SDL_SCANCODE_F: return 70;
    case SDL_SCANCODE_G: return 71;
    case SDL_SCANCODE_H: return 72;
    case SDL_SCANCODE_I: return 73;
    case SDL_SCANCODE_J: return 74;
    case SDL_SCANCODE_K: return 75;
    case SDL_SCANCODE_L: return 76;
    case SDL_SCANCODE_M: return 77;
    case SDL_SCANCODE_N: return 78;
    case SDL_SCANCODE_O: return 79;
    case SDL_SCANCODE_P: return 80;
    case SDL_SCANCODE_Q: return 81;
    case SDL_SCANCODE_R: return 82;
    case SDL_SCANCODE_S: return 83;
    case SDL_SCANCODE_T: return 84;
    case SDL_SCANCODE_U: return 85;
    case SDL_SCANCODE_V: return 86;
    case SDL_SCANCODE_W: return 87;
    case SDL_SCANCODE_X: return 88;
    case SDL_SCANCODE_Y: return 89;
    case SDL_SCANCODE_Z: return 90;
    case SDL_SCANCODE_F1: return 112;
    case SDL_SCANCODE_F2: return 113;
    case SDL_SCANCODE_F3: return 114;
    case SDL_SCANCODE_F4: return 115;
    case SDL_SCANCODE_F5: return 116;
    case SDL_SCANCODE_F6: return 117;
    case SDL_SCANCODE_F7: return 118;
    case SDL_SCANCODE_F8: return 119;
    case SDL_SCANCODE_F9: return 120;
    case SDL_SCANCODE_F10: return 121;
    case SDL_SCANCODE_F11: return 122;
    case SDL_SCANCODE_F12: return 123;
    case SDL_SCANCODE_SEMICOLON: return 186;
    case SDL_SCANCODE_EQUALS: return 187;
    case SDL_SCANCODE_COMMA: return 188;
    case SDL_SCANCODE_MINUS: return 189;
    case SDL_SCANCODE_PERIOD: return 190;
    case SDL_SCANCODE_SLASH: return 191;
    case SDL_SCANCODE_BACKSLASH: return 220;
    case SDL_SCANCODE_APOSTROPHE: return 222;
    case SDL_SCANCODE_GRAVE: return 192;
    case SDL_SCANCODE_LEFTBRACKET: return 219;
    case SDL_SCANCODE_RIGHTBRACKET: return 221;
    default: return 0;
    }
}

void KeyboardEvent::configure(SDL_Event &e)
{
    bool altKey = (e.key.keysym.mod & (KMOD_LALT | KMOD_RALT)) > 0;
    bool ctrlKey = (e.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) > 0;
    bool metaKey = (e.key.keysym.mod & (KMOD_LGUI | KMOD_RGUI)) > 0;
    bool shiftKey = (e.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)) > 0;
    int location = 0;
    if (e.key.keysym.mod & (KMOD_LALT | KMOD_LCTRL | KMOD_LGUI | KMOD_LSHIFT))
    {
        location = 1; // left;
    }
    else if (e.key.keysym.mod & (KMOD_RALT | KMOD_RCTRL | KMOD_RGUI | KMOD_RSHIFT))
    {
        location = 2; // right;
    }
    bool repeat = e.key.repeat > 0;

    SDL_Keycode key = e.key.keysym.sym;
    SDL_Scancode code = e.key.keysym.scancode;

    object.setProperty("altKey", altKey);
    object.setProperty("keyCode", scancodeToWebKeycode(code));
    object.setProperty("ctrlKey", ctrlKey);
    object.setProperty("key", key);
    object.setProperty("location", location);
    object.setProperty("metaKey", metaKey);
    object.setProperty("repeat", repeat);
    object.setProperty("shiftKey", shiftKey);
    object.setProperty("type", e.key.state == SDL_PRESSED ? "keydown" : "keyup");
}
