#include "PhaserNativeApp.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeGraphics.h"
#include "PhaserNativeDebugRenderer.h"

#include <stdexcept>

extern "C" {
#include <duk_console.h>
}

// DOM/HTML5 object subs/wrappers/implementations
//#include "bindings/Body.h"
//#include "bindings/CanvasRenderingContext2D.h"
//#include "bindings/Console.h"
#include "bindings/Document.h"
#include "bindings/DOMRectReadOnly.h"
#include "bindings/HTMLCanvasElement.h"
#include "bindings/Image.h"
//#include "bindings/KeyboardEvent.h"
#include "bindings/Navigator.h"
#include "bindings/Performance.h"
//#include "bindings/URL.h"
//#include "bindings/WebGLRenderingContext.h"
#include "bindings/Window.h"
//#include "bindings/XMLHttpRequest.h"

duk::Context PhaserNativeApp::ctx = duk::Context("PhaserNative");

PhaserNativeApp::PhaserNativeApp()
{
#ifndef NDEBUG
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
#endif

    int rc = 0;
    if((rc = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER)) < 0 )
    {
        throw std::runtime_error(SDL_GetError());
    }

    PhaserNativeEvent::Timeout = SDL_RegisterEvents(1);
    PhaserNativeEvent::ImageDecoded = SDL_RegisterEvents(1);
    PhaserNativeEvent::RequestAnimationFrame = SDL_RegisterEvents(1);
}

PhaserNativeApp::~PhaserNativeApp()
{
    for(auto &gameDevice : m_gameDevices)
    {
        SDL_GameControllerClose(gameDevice.gameController);
    }

    m_gameDevices.clear();

    SDL_Quit();
}

int PhaserNativeApp::run(int argc, char* argv[])
{
    int rc = 0;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        javascriptFiles.push_back(arg);
    }

    if (javascriptFiles.size() == 0)
    {
        SDL_LogError(0, "No javascript file argument specified.\n");
        return -1;
    }

    // register console.
    duk_console_init(ctx, 0 /*flags*/);

    // register our native create_timer and delete_timer functions
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, create_timer, 3);
    duk_put_global_string(ctx, "_create_timer");
    duk_push_c_function(ctx, delete_timer, 1);
    duk_put_global_string(ctx, "_delete_timer");
    duk_pop(ctx);

    // add setTimeout, clearTimeout, setInterval, and clearInterval javascript code.
    duk_push_global_stash(ctx);
    duk_eval_string(ctx, timer_api_script);
    duk_put_global_string(ctx, "global");

    // Initialize global stash 'eventTimers'.
    duk_push_global_stash(ctx);
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, "eventTimers");
    duk_pop(ctx);



//    // register constructors. Without them, users won't be able to 'new' them
//    // from javascript.
//    ctx.registerClass<Body>();
//    ctx.registerClass<CanvasRenderingContext2D>();
//    ctx.registerClass<Console>();

    ctx.registerClass<Event>();
    ctx.registerClass<DOMRectReadOnly>();
    ctx.registerClass<RenderingContext>();

    ctx.registerClass<EventTarget>();
    ctx.registerClass<Node>();
    ctx.registerClass<Element>();
    ctx.registerClass<HTMLElement>();
    ctx.registerClass<HTMLCanvasElement>();

    ctx.registerClass<Document>();

    ctx.registerClass<Image>();
//    ctx.registerClass<KeyboardEvent>();
    ctx.registerClass<Navigator>();
    ctx.registerClass<Performance>();
//    ctx.registerClass<URL>();
//    ctx.registerClass<WebGLRenderingContext>();
    ctx.registerClass<Window>();
//    ctx.registerClass<XMLHttpRequest>();

//    // install default instances to mimic a web browser
//    ctx.addGlobal("console", std::make_shared<Console>());
    ctx.addGlobal("document", std::make_shared<Document>());
    ctx.addGlobal("navigator", std::make_shared<Navigator>());
    ctx.addGlobal("performance", std::make_shared<Performance>());
    ctx.addGlobal("window", std::make_shared<Window>());
//
//    globalObject.setProperty("_keyboardEvent", KeyboardEvent::CreateJSObject({}), kJSPropertyAttributeDontEnum);

//    // evaluate phaser.js
    compileFile("phaser.js");

    for(auto javascriptFile : javascriptFiles) {

        SDL_LogInfo(0, "Compiling %s...\n", javascriptFile);
        compileFile(javascriptFile);
    }

    m_running = true;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(processEvent, 0, 1);
#else
    while(m_running)
    {
        processEvent();
    }
#endif

    return rc;
}

void PhaserNativeApp::processEvent()
{
    SDL_Event e;

    if(SDL_WaitEvent( &e ) != 0)
    {
        switch(e.type)
        {
        case SDL_KEYUP:
            break;
        case SDL_KEYDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEWHEEL:
            break;
        case SDL_FINGERUP:
            break;
        case SDL_FINGERDOWN:
            break;
        case SDL_FINGERMOTION:
            break;
        case SDL_CONTROLLERAXISMOTION:
            break;
        case SDL_CONTROLLERBUTTONUP:
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            break;
        case SDL_CONTROLLERDEVICEADDED:
            if(SDL_IsGameController(e.cdevice.which))
            {
                GameDevice device;
                device.gameController = SDL_GameControllerOpen(e.cdevice.which);
                if (device.gameController)
                {
                    device.joystickID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(device.gameController));
                    m_gameDevices.push_back(device);
                }
            }
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
        {
            auto it = m_gameDevices.begin();
            auto end = m_gameDevices.end();
            while(it != end)
            {
                if ((*it).joystickID == e.cdevice.which)
                {
                    SDL_GameControllerClose((*it).gameController);
                    m_gameDevices.erase(it);
                    break;
                }
                ++it;
            }

            break;
        }
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_MOVED:
            {
                break;
            }
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                break;
            }
            case SDL_WINDOWEVENT_CLOSE:
            {
                m_running = false;
                break;
            }
            default:
                break;
            }

            break;
        case SDL_QUIT:
            m_running = false;
            break;
        default:
            /* USER EVENTS SECTION */
            if (e.type == PhaserNativeEvent::Timeout) {

                bool oneshot = e.user.code > 0;
                int timer_id = static_cast<int>(reinterpret_cast<intptr_t>(e.user.data1));

                duk_push_global_stash(ctx);
                duk_get_prop_string(ctx, -1, "eventTimers");
                duk_push_int(ctx, timer_id);
                duk_get_prop(ctx, -2);  /* -> [ ... stash eventTimers func ] */
                duk_pcall(ctx, 0 /*nargs*/);  /* -> [ ... stash eventTimers retval ] */
                duk_pop(ctx);    /* ignore errors for now -> [ ... stash eventTimers ] */
                if (oneshot) {
                    auto it = PhaserNativeEvent::Timers.begin();
                    auto end = PhaserNativeEvent::Timers.end();
                    while (it != end){
                        SDL_Event *timer = *it;
                        if (timer->user.data1 == e.user.data1) {
                            delete timer;
                            PhaserNativeEvent::Timers.erase(it);
                            break;
                        }
                        ++it;
                    }
                    duk_push_int(ctx, static_cast<int>(reinterpret_cast<intptr_t>(e.user.data1)));
                    duk_del_prop(ctx, -2);
                }

            }
            else if (e.type == PhaserNativeEvent::ImageDecoded)
            {
                Image::OnImageDecoded(e.user.data1);
            }
            else if (e.type == PhaserNativeEvent::RequestAnimationFrame)
            {
                PhaserNativeBeginFrame();
                m_debugRenderer.startProfiling();
//                Window::OnRequestAnimationFrame(e.user.data1, ((double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency() * 1000.0));
                m_debugRenderer.stopProfiling();
                m_debugRenderer.renderStats();
                PhaserNativeEndFrame();

                SDL_Window *window = SDL_GL_GetCurrentWindow();

                if (window)
                {
                    SDL_GL_SwapWindow(window);
                }

            }
            else if (e.type == PhaserNativeEvent::XHR)
            {
//                XMLHttpRequest::OnRequest(e.user.data1);
            }

            break;
        }
    }
}

int PhaserNativeApp::compileFile(const char * filename)
{
    FILE *f = NULL;
    int retval;
    char fnbuf[256];

    /* Example of sending an application specific debugger notification. */
    duk_push_string(ctx, "DebuggerHandleFile");
    duk_push_string(ctx, filename);
    duk_debugger_notify(ctx, 2);

    snprintf(fnbuf, sizeof(fnbuf), "%s", filename);

    fnbuf[sizeof(fnbuf) - 1] = (char) 0;

    f = fopen(fnbuf, "rb");
    if (!f) {
        std::stringstream error;
        error << "failed to open source file: " << filename;
        throw std::runtime_error(error.str());
    }

    retval = compileFileHandler(f);

    fclose(f);
    return retval;
}

int PhaserNativeApp::compileFileHandler(FILE *f)
{
    char *buf = NULL;
    size_t bufsz;
    size_t bufoff;
    size_t got;
    int retval = 0;

    buf = (char *) malloc(1024);
    if (!buf) {
        SDL_LogError(0, "Out of memory.\n");
        goto cleanup;
    }
    bufsz = 1024;
    bufoff = 0;

    /* Read until EOF, avoid fseek/stat because it won't work with stdin. */
    for (;;) {
        size_t avail;

        avail = bufsz - bufoff;
        if (avail < 1024) {
            size_t newsz;
            char *buf_new;
            newsz = bufsz + (bufsz >> 2) + 1024;  /* +25% and some extra */
            buf_new = (char *) realloc(buf, newsz);
            if (!buf_new) {
                SDL_LogError(0, "Out of memory.\n");
                goto cleanup;
            }
            buf = buf_new;
            bufsz = newsz;
        }

        avail = bufsz - bufoff;

        got = fread((void *) (buf + bufoff), (size_t) 1, avail, f);
        if (got == 0) {
            break;
        }
        bufoff += got;
    }

    duk_push_pointer(ctx, (void *) buf);
    duk_push_uint(ctx, (duk_uint_t) bufoff);

    retval = duk_safe_call(ctx, wrappedCompileExecute, NULL /*udata*/, 2 /*nargs*/, 1 /*nret*/);

    if (retval != DUK_EXEC_SUCCESS) {
        (void) duk_safe_call(ctx, getStackRaw, NULL /*udata*/, 1 /*nargs*/, 1 /*nrets*/);
        throw std::runtime_error(duk_safe_to_string(ctx, -1));
    }

    duk_pop(ctx);

cleanup:
    if (buf) {
        free(buf);
        buf = NULL;
    }
    return retval;
}

duk_ret_t PhaserNativeApp::wrappedCompileExecute(duk_context *ctx, void *udata) {
    const char *src_data;
    duk_size_t src_len;
    duk_uint_t comp_flags;

    (void) udata;

    src_data = (const char *) duk_require_pointer(ctx, -2);
    src_len = (duk_size_t) duk_require_uint(ctx, -1);

    if (src_data != NULL && src_len >= 1 && src_data[0] == (char) 0xbf) {
        /* Bytecode. */
        void *buf;
        buf = duk_push_fixed_buffer(ctx, src_len);
        memcpy(buf, (const void *) src_data, src_len);
        duk_load_function(ctx);
    } else {
        /* Source code. */
        comp_flags = DUK_COMPILE_SHEBANG;
        duk_compile_lstring_filename(ctx, comp_flags, src_data, src_len);
    }

    duk_push_global_object(ctx);  /* 'this' binding */
    duk_call_method(ctx, 0);

    return 0;
}


duk_ret_t PhaserNativeApp::getStackRaw(duk_context *ctx, void *udata) {
    (void) udata;

    if (!duk_is_object(ctx, -1)) {
        return 1;
    }
    if (!duk_has_prop_string(ctx, -1, "stack")) {
        return 1;
    }
    if (!duk_is_error(ctx, -1)) {
        /* Not an Error instance, don't read "stack". */
        return 1;
    }

    duk_get_prop_string(ctx, -1, "stack");  /* caller coerces */
    duk_remove(ctx, -2);
    return 1;
}

int PhaserNativeApp::create_timer(duk_context *ctx)
{
    /* indexes:
     *   0 = function (callback)
     *   1 = delay
     *   2 = boolean: oneshot
     */
    unsigned int delay = duk_require_uint(ctx, 1);
    if (delay < 1) {
        delay = 1;
    }

    SDL_Event *event = new SDL_Event();
    SDL_memset(event, 0, sizeof(SDL_Event));
    event->type = PhaserNativeEvent::Timeout;
    event->user.code = static_cast<int>(duk_require_boolean(ctx, 2)); // storing 'oneshot' into user.code

    PhaserNativeEvent::Timers.push_back(event);

    event->user.data1 = reinterpret_cast<void*>(
                SDL_AddTimer(delay, +[](Uint32 interval, void *param) -> Uint32 {
                    SDL_Event *event = reinterpret_cast<SDL_Event *>(param);
                    SDL_PushEvent(event);
                    if (event->user.code) { // if oneshot
                        return 0; // timer will self-destruct.
                    } else {
                        return interval; // repeat this timer.
                    }
                }, event));

    /* Finally, register the callback to the global stash 'eventTimers' object. */

    duk_push_global_stash(ctx);
    duk_get_prop_string(ctx, -1, "eventTimers");  /* -> [ func delay oneshot stash eventTimers ] */
    duk_push_int(ctx, static_cast<int>(reinterpret_cast<intptr_t>(event->user.data1)));
    duk_dup(ctx, 0);
    duk_put_prop(ctx, -3);  /* eventTimers[timer_id] = callback */

    /* Return timer id. */

    duk_push_int(ctx, static_cast<int>(reinterpret_cast<intptr_t>(event->user.data1)));
    return 1;
}

int PhaserNativeApp::delete_timer(duk_context *ctx)
{
    int timer_id = duk_require_int(ctx, 0);

    SDL_bool found = SDL_RemoveTimer(timer_id);

    if (found) {

        auto it = PhaserNativeEvent::Timers.begin();
        auto end = PhaserNativeEvent::Timers.end();
        while (it != end){
            SDL_Event *timer = *it;
            int other_timer_id = static_cast<int>(reinterpret_cast<intptr_t>(timer->user.data1));
            if (timer_id == other_timer_id){
                delete timer;
                PhaserNativeEvent::Timers.erase(it);
                break;
            }
            ++it;
        }

        duk_push_global_stash(ctx);
        duk_get_prop_string(ctx, -1, "eventTimers");  /* -> [ timer_id stash eventTimers ] */
        duk_push_int(ctx, timer_id);
        duk_del_prop(ctx, -2);  /* delete eventTimers[timer_id] */

    }
    duk_push_boolean(ctx, found);
    return 1;
}

/*
 *  Ecmascript code to initialize the exposed API (setTimeout() etc)
 *
 *  https://developer.mozilla.org/en-US/docs/Web/JavaScript/Timers
 */
const char * PhaserNativeApp::timer_api_script =
"function setTimeout(func, delay) {\n"
"    var cb_func;\n"
"    var bind_args;\n"
"    var timer_id;\n"
"\n"
     // Delay can be optional at least in some contexts, so tolerate that.
     // https://developer.mozilla.org/en-US/docs/Web/API/WindowOrWorkerGlobalScope/setTimeout
"    if (typeof delay !== 'number') {\n"
"        if (typeof delay === 'undefined') {\n"
"            delay = 0;\n"
"        } else {\n"
"            throw new TypeError('invalid delay');\n"
"        }\n"
"    }\n"
"\n"
"    if (typeof func === 'string') {\n"
         // Legacy case: callback is a string.
"        cb_func = eval.bind(this, func);\n"
"    } else if (typeof func !== 'function') {\n"
"        throw new TypeError('callback is not a function/string');\n"
"    } else if (arguments.length > 2) {\n"
         // Special case: callback arguments are provided.
"        bind_args = Array.prototype.slice.call(arguments, 2);\n"  // [ arg1, arg2, ... ]
"        bind_args.unshift(this);\n"  // [ global(this), arg1, arg2, ... ]
"        cb_func = func.bind.apply(func, bind_args);\n"
"    } else {\n"
         // Normal case: callback given as a function without arguments.
"        cb_func = func;\n"
"    }\n"
"\n"
"    timer_id = _createTimer(cb_func, delay, true /*oneshot*/);\n"
"\n"
"    return timer_id;\n"
"}\n"
"\n"
"function clearTimeout(timer_id) {\n"
"    if (typeof timer_id !== 'number') {\n"
"        throw new TypeError('timer ID is not a number');\n"
"    }\n"
"    var success = _deleteTimer(timer_id);  /* retval ignored */\n"
"}\n"
"\n"
"function setInterval(func, delay) {\n"
"    var cb_func;\n"
"    var bind_args;\n"
"    var timer_id;\n"
"\n"
"    if (typeof delay !== 'number') {\n"
"        if (typeof delay === 'undefined') {\n"
"            delay = 0;\n"
"        } else {\n"
"            throw new TypeError('invalid delay');\n"
"        }\n"
"    }\n"
"\n"
"    if (typeof func === 'string') {\n"
         // Legacy case: callback is a string.
"        cb_func = eval.bind(this, func);\n"
"    } else if (typeof func !== 'function') {\n"
"        throw new TypeError('callback is not a function/string');\n"
"    } else if (arguments.length > 2) {\n"
         // Special case: callback arguments are provided.
"        bind_args = Array.prototype.slice.call(arguments, 2);\n"  // [ arg1, arg2, ... ]
"        bind_args.unshift(this);\n"  // [ global(this), arg1, arg2, ... ]
"        cb_func = func.bind.apply(func, bind_args);\n"
"    } else {\n"
         // Normal case: callback given as a function without arguments.
"        cb_func = func;\n"
"    }\n"
"\n"
"    timer_id = _createTimer(cb_func, delay, false /*oneshot*/);\n"
"\n"
"    return timer_id;\n"
"}\n"
"\n"
"function clearInterval(timer_id) {\n"
"    if (typeof timer_id !== 'number') {\n"
"        throw new TypeError('timer ID is not a number');\n"
"    }\n"
"    _deleteTimer(timer_id);\n"
"}\n";
