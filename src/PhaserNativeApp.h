#ifndef PHASERNATIVEAPP_H
#define PHASERNATIVEAPP_H

#include <vector>

#include <SDL2/SDL_gamecontroller.h>
#include <duktape-cpp/DuktapeCpp.h>

#include "PhaserNativeDebugRenderer.h"


class PhaserNativeApp
{
public:

    static duk::Context ctx;

    struct GameDevice
    {
        SDL_GameController *gameController = nullptr;
        SDL_JoystickID joystickID = 0;
    };


    PhaserNativeApp();
    ~PhaserNativeApp();

    int run(int argc, char* argv[]);

private:

    void processEvent();

    PhaserNativeDebugRenderer m_debugRenderer;
    std::vector<const char *> javascriptFiles;
    bool m_running = false;
    std::vector<GameDevice> m_gameDevices;

    int compileFile(const char * filename);
    int compileFileHandler(FILE *f);

    static duk_ret_t wrappedCompileExecute(duk_context *ctx, void *udata);
    static duk_ret_t getStackRaw(duk_context *ctx, void *udata);
    static int create_timer(duk_context *ctx);
    static int delete_timer(duk_context *ctx);
    static const char* timer_api_script;
};

#endif // PHASERNATIVEAPP_H
