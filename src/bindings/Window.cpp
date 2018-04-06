#include "Window.h"
#include "Performance.h"

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_log.h>

#include "PhaserNativeEvent.h"
#include "WebGLRenderingContext.h"

JSC_CONSTRUCTOR(Window::Constructor)
{
    Window &window = CreateNativeInstance();

    // install a few global function into the instance.
    // Not sure why it has to exist in both cases...
    JSC::Object globalObject = JSC_GLOBAL_OBJECT;
    window.object.setProperty("setTimeout", globalObject.getProperty("setTimeout"));
    window.object.setProperty("clearTimeout", globalObject.getProperty("clearTimeout"));
    window.object.setProperty("setInterval", globalObject.getProperty("setInterval"));
    window.object.setProperty("clearInterval", globalObject.getProperty("clearInterval"));
    window.object.setProperty("performance", globalObject.getProperty("performance"));
    window.object.setProperty("WebGLRenderingContext", globalObject.getProperty("WebGLRenderingContext"));

    return window.object;
}

JSC_FINALIZER(Window::Finalizer)
{
    FreeNativeInstance(object);
}

JSC_FUNCTION(Window::createTimer) {
    JSC::Value callbackVal = argv[0];
    JSC::Value delayVal    = argv[1];
    JSC::Value oneshotVal  = argv[2];

    unsigned int delay = delayVal.toInteger();
    bool oneshot = oneshotVal.toBoolean();

    if (delay < 1) { delay = 1; }

    SDL_Event *event = new SDL_Event();
    SDL_memset(event, 0, sizeof(SDL_Event));
    event->type = PhaserNativeEvent::Timeout;
    event->user.code = oneshot ? 1 : 0;

    PhaserNativeEvent::Timers.push_back(event);

    event->user.data1 = (void*)((JSValueRef)callbackVal);
    event->user.data2 = reinterpret_cast<void*>(
                SDL_AddTimer(delay, +[](Uint32 interval, void *param) -> Uint32 {
                    SDL_Event *event = reinterpret_cast<SDL_Event *>(param);
                    SDL_PushEvent(event);
                    if (event->user.code) { // if oneshot
                                            return 0; // timer will self-destruct.
                    } else {
                        return interval; // repeat this timer.
                    }
                }, event));

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Window::deleteTimer) {
    JSC::Value timerIDVal = argv[0];
    int timer_id = timerIDVal.toInteger();

    if (SDL_RemoveTimer(timer_id)) {
        auto it = PhaserNativeEvent::Timers.begin();
        auto end = PhaserNativeEvent::Timers.end();
        while (it != end) {
            SDL_Event *timeoutEvent = *it;
            int other_timer_id = static_cast<int>(reinterpret_cast<intptr_t>(timeoutEvent->user.data2));
            if (timer_id == other_timer_id) {
                delete timeoutEvent;
                PhaserNativeEvent::Timers.erase(it);
                break;
            }
            ++it;
        }
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Window::addEventListener)
{
    JSC::Value type = argv[0];
    JSC::Object listener = JSC::Value(argv[1]).toObject();

    std::string typeStr = type.toString().getUTF8String();
    SDL_Log("Registering listener for '%s'", typeStr.c_str());
    std::set<JSC::Object> &listeners = PhaserNativeEvent::eventListeners[typeStr];
    listeners.insert(listener);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Window::removeEventListener)
{
    JSC::Value type = argv[0];
    JSC::Object listener = JSC::Value(argv[1]).toObject();

    std::string typeStr = type.toString().getUTF8String();
    SDL_Log("Unregistering listener for '%s'", typeStr.c_str());
    std::set<JSC::Object> &listeners = PhaserNativeEvent::eventListeners[typeStr];
    auto it = listeners.find(listener);
    if (it != listeners.end())
    {
        listeners.erase(it);
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Window::requestAnimationFrame)
{
    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    event.type = PhaserNativeEvent::RequestAnimationFrame;
    event.user.data1 = (void*)argv[0];
    SDL_PushEvent(&event);
    return JSC::Value(SDL_GetTicks());
}

void Window::OnRequestAnimationFrame(void* ptr, double timestamp)
{
    JSC::Object callbcack = JSC::Object((JSObjectRef)ptr);
    callbcack.callAsFunction({JSC::Value(timestamp)});
}


JSC::Class &Window::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {

            {"pageXOffset", JSC_CONSTANT(0)},
            {"pageYOffset", JSC_CONSTANT(0)},

            {0, 0, 0, 0}
        };

        static JSStaticFunction staticFunctions[] = {
            { "addEventListener", Window::addEventListener, kJSPropertyAttributeDontDelete },
            { "removeEventListener", Window::removeEventListener, kJSPropertyAttributeDontDelete },
            { "requestAnimationFrame", Window::requestAnimationFrame, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Window";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = Window::Constructor;
        classDefinition.finalize = Window::Finalizer;
        _class = JSC::Class(&classDefinition);

        JSC::Object createTimerFunctionObject = JSC::Object::MakeFunctionWithCallback("__createTimer", createTimer);
        JSC::Object deleteTimerFunctionObject = JSC::Object::MakeFunctionWithCallback("__deleteTimer", deleteTimer);

        JSC::Object globalObject = JSC_GLOBAL_OBJECT;
        globalObject.setProperty("__createTimer", createTimerFunctionObject, kJSPropertyAttributeDontEnum);
        globalObject.setProperty("__deleteTimer", deleteTimerFunctionObject, kJSPropertyAttributeDontEnum);

        JSC::evaluateScriptFromString(
                    "function setTimeout(func, delay) {\n"
                    "    var cb_func;\n"
                    "    var bind_args;\n"
                    "    var timer_id;\n"
                    "    if (typeof delay !== 'number') {\n"
                    "        if (typeof delay === 'undefined') {\n"
                    "            delay = 0;\n"
                    "        } else {\n"
                    "            throw new TypeError('invalid delay');\n"
                    "        }\n"
                    "    }\n"
                    "    if (typeof func === 'string') {\n"
                    "        cb_func = eval.bind(this, func);\n"
                    "    } else if (typeof func !== 'function') {\n"
                    "        throw new TypeError('callback is not a function/string');\n"
                    "    } else if (arguments.length > 2) {\n"
                    "        bind_args = Array.prototype.slice.call(arguments, 2);\n"
                    "        bind_args.unshift(this);\n"
                    "        cb_func = func.bind.apply(func, bind_args);\n"
                    "    } else {\n"
                    "        cb_func = func;\n"
                    "    }\n"
                    "    timer_id = __createTimer(cb_func, delay, true);\n"
                    "    return timer_id;\n"
                    "}\n"

                    "function clearTimeout(timer_id) {\n"
                    "    if (typeof timer_id !== 'number') {\n"
                    "        throw new TypeError('timer ID is not a number');\n"
                    "    }\n"
                    "    var success = __deleteTimer(timer_id);\n"
                    "}\n"

                    "function setInterval(func, delay) {\n"
                    "    var cb_func;\n"
                    "    var bind_args;\n"
                    "    var timer_id;\n"
                    "    if (typeof delay !== 'number') {\n"
                    "        if (typeof delay === 'undefined') {\n"
                    "            delay = 0;\n"
                    "        } else {\n"
                    "            throw new TypeError('invalid delay');\n"
                    "        }\n"
                    "    }\n"
                    "    if (typeof func === 'string') {\n"
                    "        cb_func = eval.bind(this, func);\n"
                    "    } else if (typeof func !== 'function') {\n"
                    "        throw new TypeError('callback is not a function/string');\n"
                    "    } else if (arguments.length > 2) {\n"
                    "        bind_args = Array.prototype.slice.call(arguments, 2);\n"
                    "        bind_args.unshift(this);\n"
                    "        cb_func = func.bind.apply(func, bind_args);\n"
                    "    } else {\n"
                    "        cb_func = func;\n"
                    "    }\n"
                    "    timer_id = __createTimer(cb_func, delay, false);\n"
                    "    return timer_id;\n"
                    "}\n"

                    "function clearInterval(timer_id) {\n"
                    "    if (typeof timer_id !== 'number') {\n"
                    "        throw new TypeError('timer ID is not a number');\n"
                    "    }\n"
                    "    __deleteTimer(timer_id);\n"
                    "}\n",

                    "bindings/Window.cpp");
    }

    return _class;
}
