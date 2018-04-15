#pragma once

#include "Element.h"

struct HTMLElement : public Element
{
    virtual ~HTMLElement() override {}

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<HTMLElement>);
    }

};

DUK_CPP_DEF_CLASS_NAME(HTMLElement);
DUK_CPP_DEF_BASE_CLASS(HTMLElement, Element);
