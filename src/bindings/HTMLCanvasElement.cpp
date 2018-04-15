#include "HTMLCanvasElement.h"
//#include "CanvasRenderingContext2D.h"
//#include "WebGLRenderingContext.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeGraphics.h"

#include <algorithm>
#include <sstream>

HTMLCanvasElement::HTMLCanvasElement() {
    window = PhaserNativeCreateWindow();
    SDL_GetWindowPosition(window, &clientTop, &clientLeft);
    SDL_GetWindowSize(window, &clientWidth, &clientHeight);
}

HTMLCanvasElement::~HTMLCanvasElement() {
    PhaserNativeDestroyWindow(window);
    window = nullptr;
}

duk_ret_t HTMLCanvasElement::getContext(duk::Context &ctx, duk_idx_t nargs) {

    const char *contextType;
    duk::Type<const char *>::get(ctx, contextType, 0);

//    if (strcmp(contextType, "2d") == 0)
//    {
//        duk::Type< std::shared_ptr<CanvasRenderingContext2D> >::push(std::make_shared<CanvasRenderingContext2D>(shared_from_this()));
//        return 1;
//    }

//    if (strcmp(contextType, "webgl") == 0 ||
//             strcmp(contextType, "webgl2") == 0 ||
//             strcmp(contextType, "experimental-webgl") == 0)
//    {
//        duk::Type< std::shared_ptr<WebGLRenderingContext> >::push(std::make_shared<WebGLRenderingContext>(shared_from_this()));
//        return 1;
//    }

    SDL_LogWarn(0, "Unsupported context type '%s'. Contact a Developer!\n", contextType);

    return 0;
}

int HTMLCanvasElement::getWidth() const
{
    return clientWidth;
}

void HTMLCanvasElement::setWidth(int width)
{
    SDL_SetWindowSize(window, width, clientHeight);
    SDL_GetWindowSize(window, &clientWidth, &clientHeight);
}

int HTMLCanvasElement::getHeight() const
{
    return clientHeight;
}

void HTMLCanvasElement::setHeight(int height)
{
    SDL_SetWindowSize(window, clientWidth, height);
    SDL_GetWindowSize(window, &clientWidth, &clientHeight);

}
