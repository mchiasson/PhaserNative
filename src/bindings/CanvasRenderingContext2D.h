#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <SDL2/SDL.h>
#include <nanovg/nanovg.h>

class CanvasRenderingContext2D
{
public:

    static DUK_METHOD(fillRect);

    static DUK_METHOD(clearRect);
    static DUK_METHOD(createImageData);
    static DUK_METHOD(getImageData);
    static DUK_METHOD(putImageData);
    static DUK_METHOD(drawImage);

    static DUK_METHOD(measureText);
    static DUK_METHOD(fillText);

    static DUK_METHOD(save);
    static DUK_METHOD(restore);

    static DUK_METHOD(translate);

    static DUK_PROPERTY_GET(getFillStyle);
    static DUK_PROPERTY_SET(setFillStyle);

    static DUK_PROPERTY_GET(getCanvas);

    SDL_GLContext context = nullptr;
    uint32_t vao = 0;
    size_t canvasIndex = 0;

    NVGcontext *vg;
    int fontIcons = 0;
    int fontNormal = 0;
    int fontBold = 0;
    int fontEmoji = 0;

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<CanvasRenderingContext2D>);
        i.method("fillRect", &CanvasRenderingContext2D::fillRect);
        i.method("clearRect", &CanvasRenderingContext2D::clearRect);
        i.method("createImageData", &CanvasRenderingContext2D::createImageData);
        i.method("getImageData", &CanvasRenderingContext2D::getImageData);
        i.method("putImageData", &CanvasRenderingContext2D::putImageData);
        i.method("drawImage", &CanvasRenderingContext2D::drawImage);
        i.method("measureText", &CanvasRenderingContext2D::measureText);
        i.method("fillText", &CanvasRenderingContext2D::fillText);
        i.method("save", &CanvasRenderingContext2D::save);
        i.method("restore", &CanvasRenderingContext2D::restore);
        i.method("translate", &CanvasRenderingContext2D::translate);
    }

};
