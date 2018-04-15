#include "EventTarget.h"
#include <SDL2/SDL_log.h>

template<typename T, typename... U>
static size_t getAddress(std::function<T(U...)> f) {
    typedef T(fnType)(U...);
    fnType ** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
}

void EventTarget::addEventListener(const char *type, const Callback &callback, bool /*useCapture*/)
{

#define ADD_LISTENER(_NAME_)\
    if(strcmp(type, #_NAME_) == 0) { \
        _NAME_##_listeners.push_back(callback);\
        return;\
    }

    ADD_LISTENER(keydown)
    ADD_LISTENER(keyup)
    ADD_LISTENER(mousemove)
    ADD_LISTENER(mousedown)
    ADD_LISTENER(mouseup)
    ADD_LISTENER(touchmove)
    ADD_LISTENER(touchstart)
    ADD_LISTENER(touchend)

    SDL_LogWarn(0, "Event type '%s' is not supported. Contact a developer!\n", type);
}

void EventTarget::removeEventListener(const char *type, const Callback &callback)
{

#define REMOVE_LISTENER(_NAME_)\
    if(strcmp(type, #_NAME_) == 0) { \
        for(size_t i = 0; i < _NAME_##_listeners.size(); ++i)\
        {\
            if (_NAME_##_listeners[i] == callback)\
            {\
                _NAME_##_listeners.erase(_NAME_##_listeners.begin()+i);\
                return;\
            }\
        }\
        return;\
    }

    REMOVE_LISTENER(keydown)
    REMOVE_LISTENER(keyup)
    REMOVE_LISTENER(mousemove)
    REMOVE_LISTENER(mousedown)
    REMOVE_LISTENER(mouseup)
    REMOVE_LISTENER(touchmove)
    REMOVE_LISTENER(touchstart)
    REMOVE_LISTENER(touchend)

    SDL_LogWarn(0, "Event type '%s' is not supported. Contact a developer!\n", type);
}

bool EventTarget::dispatchEvent(const std::shared_ptr<Event> &event)
{
#define DISPATCH_EVENT(_NAME_)\
    if (event->type == #_NAME_)\
    {\
        for(size_t i = 0; i < _NAME_##_listeners.size(); ++i)\
        {\
            _NAME_##_listeners[i](event);\
        }\
        return true;\
    }

    DISPATCH_EVENT(keydown)
    DISPATCH_EVENT(keyup)
    DISPATCH_EVENT(mousemove)
    DISPATCH_EVENT(mousedown)
    DISPATCH_EVENT(mouseup)
    DISPATCH_EVENT(touchmove)
    DISPATCH_EVENT(touchstart)
    DISPATCH_EVENT(touchend)

    SDL_LogWarn(0, "Event type '%s' is not supported. Contact a developer!\n", event->type.c_str());

    return false;
}
