#pragma once

#include "HTMLElement.h"

struct Image : public HTMLElement
{
    static void OnImageDecoded(void* imageData);

    virtual ~Image() override;

    std::string src;
    const std::string &getSrc() const { return src; }
    void setSrc(const std::string &src);

    bool complete = false;
    bool getComplete() const { return complete; }

    int naturalWidth = 0;
    int getNaturalWidth() const { return naturalWidth; }

    int naturalHeight = 0;
    int getNaturalHeight() const { return naturalHeight; }

    uint8_t *pixels = nullptr;

    std::function<void(void)> onload;
    const std::function<void(void)> &getOnload() const { return onload; }
    void setOnload(const std::function<void(void)> &onload) { this->onload = onload; }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Image>);
        i.property("src", &Image::getSrc, &Image::setSrc);
        i.property("complete", &Image::getComplete);
        i.property("naturalWidth", &Image::getNaturalWidth);
        i.property("naturalHeight", &Image::getNaturalHeight);
        i.property("onload", &Image::getOnload, &Image::setOnload);
    }

};

DUK_CPP_DEF_CLASS_NAME(Image);
DUK_CPP_DEF_BASE_CLASS(Image, HTMLElement);

