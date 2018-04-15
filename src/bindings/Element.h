#pragma once

#include "Node.h"
#include "DOMRectReadOnly.h"

struct Element : public Node
{
    virtual ~Element() override {}

    int clientHeight = 0;
    int getClientHeight() const { return clientHeight; }

    int clientLeft = 0;
    int getClientLeft() const { return clientLeft; }

    int clientTop = 0;
    int getClientTop() const { return clientTop; }

    int clientWidth = 0;
    int getClientWidth() const { return clientWidth; }

    std::shared_ptr<DOMRectReadOnly> getBoundingClientRect() const {
        auto rect = std::make_shared<DOMRectReadOnly>();
        rect->x = clientLeft;
        rect->y = clientTop;
        rect->width = clientWidth;
        rect->height = clientHeight;
        return rect;
    }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Element>);
        i.property("clientHeight", &Element::getClientHeight);
        i.property("clientLeft", &Element::getClientLeft);
        i.property("clientTop", &Element::getClientTop);
        i.property("getClientWidth", &Element::getClientWidth);
    }

};

DUK_CPP_DEF_CLASS_NAME(Element);
DUK_CPP_DEF_BASE_CLASS(Element, Node);
