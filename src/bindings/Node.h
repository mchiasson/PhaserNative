#pragma once

#include "EventTarget.h"

struct Node : public EventTarget
{
    virtual ~Node() override {}

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Node>);
    }

};

DUK_CPP_DEF_CLASS_NAME(Node);
DUK_CPP_DEF_BASE_CLASS(Node, EventTarget);
