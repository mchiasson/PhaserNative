#include "PhaserNativeApp.h"
#include "PhaserNativeWindow.h"

PhaserNativeApp::PhaserNativeApp()
{
#ifndef NDEBUG
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
#endif

    int rc = 0;
    if((rc = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER)) < 0 )
    {
        SDL_LogError(0, "SDL could not initialize: %s\n", SDL_GetError() );
        throw rc;
    }
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

    for(auto javascriptFile : javascriptFiles) {

        SDL_LogInfo(0, "Compiling %s...\n", javascriptFile);

        if ((rc = m_scriptEngine.evaluateFromFile(javascriptFile)) != 0)
        {
            SDL_LogError(0, "Could not compile %s\n", javascriptFile);
            return rc;
        }
    }

    m_running = true;

    return rc;
}
