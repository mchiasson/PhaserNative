#ifndef PHASERNATIVEWINDOW_H
#define PHASERNATIVEWINDOW_H

#include <SDL2/SDL.h>

class PhaserNativeWindow
{
public:
    static int Init();
    static void ShutDown();

    PhaserNativeWindow();

private:

    SDL_Window *window = nullptr;
    SDL_GLContext context = nullptr;

};

#endif // PHASERNATIVEWINDOW_H
