#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>
#include <vector>

#include "Event.h"

struct EventTarget : public std::enable_shared_from_this<EventTarget> {

    using Callback = duk::JSFunction<void(std::shared_ptr<Event>)>;

    virtual ~EventTarget() {}

    std::vector< Callback > keydown_listeners;
    std::vector< Callback > keyup_listeners;
    std::vector< Callback > mousemove_listeners;
    std::vector< Callback > mousedown_listeners;
    std::vector< Callback > mouseup_listeners;
    std::vector< Callback > touchmove_listeners;
    std::vector< Callback > touchstart_listeners;
    std::vector< Callback > touchend_listeners;

    void addEventListener(const char *type, const Callback &callback, bool useCapture);
    void removeEventListener(const char *type, const Callback &callback);
    bool dispatchEvent(const std::shared_ptr<Event> &event);

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<EventTarget>);
        i.method("addEventListener", &EventTarget::addEventListener);
        i.method("removeEventListener", &EventTarget::removeEventListener);
    }

};

DUK_CPP_DEF_CLASS_NAME(EventTarget);
