#pragma once

#include "HTMLElement.h"

#include "RenderingContext.h"

#include <SDL2/SDL_video.h>

struct HTMLCanvasElement : public HTMLElement
{
    HTMLCanvasElement();
    virtual ~HTMLCanvasElement();

    static duk_ret_t getContext(duk::Context &ctx, duk_idx_t nargs);

    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);

    SDL_Window *window = nullptr;

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<HTMLCanvasElement>);
        i.property("width", &HTMLCanvasElement::getWidth, &HTMLCanvasElement::setWidth);
        i.property("height", &HTMLCanvasElement::getHeight, &HTMLCanvasElement::setHeight);
        i.function("getContext", &HTMLCanvasElement::getContext);
    }

};

DUK_CPP_DEF_CLASS_NAME(HTMLCanvasElement);
DUK_CPP_DEF_BASE_CLASS(HTMLCanvasElement, HTMLElement);
