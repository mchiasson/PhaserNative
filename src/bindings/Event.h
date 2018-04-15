#pragma once

#include <duktape-cpp/DuktapeCpp.h>
#include <memory>

struct EventTarget;

struct Event
{
    /*!
     * \brief A Boolean indicating whether the event is cancelable
     */
    bool cancelable = true;
    bool getCancellable() const { return cancelable; }

    /*!
     * \brief Indicates whether or not event.preventDefault() has been called on the event.
     */
    bool defaultPrevented = false;
    bool getDefaultPrevented() const { return defaultPrevented; }

    /*!
     * \brief A reference to the target to which the event was originally dispatched.
     */
    std::weak_ptr<EventTarget> target;
    std::shared_ptr<EventTarget> getEventTarget() const {return target.lock(); }

    /*!
     * \brief The time at which the event was created (in milliseconds). By specification, this value is time since epoch, but in reality browsers' definitions vary; in addition, work is underway to change this to be a DOMHighResTimeStamp instead.
     */
    int timeStamp = 0;
    int getTimeStamp() const { return timeStamp; }

    /*!
     * \brief The name of the event (case-insensitive)
     */
    std::string type;
    const std::string &getType() const { return type; }

    /*!
     * Cancels the event (if it is cancelable).
     */
    void preventDefault() { if (cancelable) { defaultPrevented = true; } }

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Event>);
        i.property("cancelable", &Event::getCancellable);
        i.property("defaultPrevented", &Event::getDefaultPrevented);
        i.property("target", &Event::getEventTarget);
        i.property("timeStamp", &Event::getTimeStamp);
        i.property("type", &Event::getType);
        i.method("preventDefault", &Event::preventDefault);
    }
};

DUK_CPP_DEF_CLASS_NAME(Event);
