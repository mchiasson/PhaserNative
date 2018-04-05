#include "PhaserNativeApp.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeWindow.h"

// DOM/HTML5 object subs/wrappers/implementations
#include "bindings/Body.h"
#include "bindings/CanvasRenderingContext2D.h"
#include "bindings/Console.h"
#include "bindings/Document.h"
#include "bindings/DocumentElement.h"
#include "bindings/HTMLCanvasElement.h"
#include "bindings/Image.h"
#include "bindings/Navigator.h"
#include "bindings/Performance.h"
#include "bindings/URL.h"
#include "bindings/WebGLRenderingContext.h"
#include "bindings/Window.h"
#include "bindings/XMLHttpRequest.h"

PhaserNativeApp::PhaserNativeApp()
{
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

    JSC::Object globalObject = JSC_GLOBAL_OBJECT;

    // register constructors. Without them, users won't be able to 'new' them
    // from javascript.
    globalObject.setProperty("Image", Image::GetJSConstructor());
    globalObject.setProperty("URL", URL::GetJSConstructor());
    globalObject.setProperty("XMLHttpRequest", XMLHttpRequest::GetJSConstructor());

    // install default instances to mimic a web browser
    globalObject.setProperty("console", Console::CreateJSObject({}));
    globalObject.setProperty("document", Document::CreateJSObject({}));
    globalObject.setProperty("navigator", Navigator::CreateJSObject({}));
    globalObject.setProperty("performance", Performance::CreateJSObject({}));
    globalObject.setProperty("window", Window::CreateJSObject({}));

    // evaluate phaser.js
    JSC::evaluateScriptFromFile("phaser.js");



    for(auto javascriptFile : javascriptFiles) {

        SDL_LogInfo(0, "Evaluating %s...\n", javascriptFile);

        JSC::evaluateScriptFromFile(javascriptFile);
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

    while(SDL_PollEvent( &e ) != 0)
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

                JSObjectRef callbackObject = reinterpret_cast<JSObjectRef>(e.user.data1);
                if (JSObjectIsFunction(JSC_GLOBAL_CTX, callbackObject))
                {
                    JSObjectCallAsFunction(JSC_GLOBAL_CTX, callbackObject, JSC_GLOBAL_OBJECT, 0, nullptr, nullptr);
                }

                if (oneshot) {
                    auto it = PhaserNativeEvent::Timers.begin();
                    auto end = PhaserNativeEvent::Timers.end();
                    while (it != end) {
                        SDL_Event *timer = *it;
                        if (timer->user.data2 == e.user.data2) {
                            delete timer;
                            PhaserNativeEvent::Timers.erase(it);
                            break;
                        }
                        ++it;
                    }
                }
            }
            else if (e.type == PhaserNativeEvent::ImageDecoded)
            {
                Image::OnImageDecoded(e.user.data1);
            }
            else if (e.type == PhaserNativeEvent::RequestAnimationFrame)
            {
                Window::OnRequestAnimationFrame(e.user.data1, ((double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency()));
                m_window.renderStats();
                m_window.swap();
            }
            else if (e.type == PhaserNativeEvent::XHR)
            {
                XMLHttpRequest::OnRequest(e.user.data1);
            }

            break;
        }
    }
}
