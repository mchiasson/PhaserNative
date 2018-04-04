#include "Canvas.h"
#include "CanvasRenderingContext2D.h"
#include "WebGLRenderingContext.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeWindow.h"

JSC_INITIALIZER(Canvas::Initializer) {
    Canvas &instance = CreateInstance(object);

    JSC::Object canvas2d = CanvasRenderingContext2D::CreateObject();
    JSC::Object gl = WebGLRenderingContext::CreateObject();

    canvas2d.setProperty("canvas", instance.object);
    gl.setProperty("canvas", instance.object);

    instance.object.setProperty("2d", canvas2d);
    instance.object.setProperty("webgl", gl);
    instance.object.setProperty("experimental-webgl", gl);
}

JSC_FINALIZER(Canvas::Finalizer) {
    FreeInstance(object);
}

JSC_FUNCTION(Canvas::getContext) {
    JSC::String elementName = JSC::Value(argv[0]).toString();
    JSC::Object instance = object;
    return instance.getProperty(elementName);
}

JSC_FUNCTION(Canvas::addEventListener)
{
    JSC::Value type = argv[0];
    JSC::Object listener = JSC::Value(argv[1]).toObject();

    std::string typeStr = type.toString().getUTF8String();
    SDL_Log("Registering listener for '%s'", typeStr.c_str());
    std::set<JSC::Object> &listeners = PhaserNativeEvent::eventListeners[typeStr];
    listeners.insert(listener);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Canvas::removeEventListener)
{
    JSC::Value type = argv[0];
    JSC::Object listener = JSC::Value(argv[1]).toObject();

    std::string typeStr = type.toString().getUTF8String();
    SDL_Log("Unregistering listener for '%s'", typeStr.c_str());
    std::set<JSC::Object> &listeners = PhaserNativeEvent::eventListeners[typeStr];
    auto it = listeners.find(listener);
    if (it != listeners.end())
    {
        listeners.erase(it);
    }
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(Canvas::getBoundingClientRect) {

    int x, y;
    SDL_GetWindowPosition(PhaserNativeWindow::window, &x, &y);

    int w, h;
    SDL_GetWindowSize(PhaserNativeWindow::window, &w, &h);

    JSC::Object rect = JSC::Object::MakeDefault();
    rect.setProperty("x", JSC::Value(x), kJSPropertyAttributeReadOnly);
    rect.setProperty("y", JSC::Value(y), kJSPropertyAttributeReadOnly);
    rect.setProperty("width", JSC::Value(w), kJSPropertyAttributeReadOnly);
    rect.setProperty("height", JSC::Value(h), kJSPropertyAttributeReadOnly);
    rect.setProperty("top", JSC::Value(h > 0 ? y : y+h), kJSPropertyAttributeReadOnly);
    rect.setProperty("right", JSC::Value(w > 0 ? x+w : x), kJSPropertyAttributeReadOnly);
    rect.setProperty("bottom", JSC::Value(h > 0 ? y+h : y), kJSPropertyAttributeReadOnly);
    rect.setProperty("left", JSC::Value(w > 0 ? x : x+w), kJSPropertyAttributeReadOnly);

    return rect;
}


JSC::Class &Canvas::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "getContext", Canvas::getContext, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "addEventListener", Canvas::addEventListener, kJSPropertyAttributeDontDelete },
            { "removeEventListener", Canvas::removeEventListener, kJSPropertyAttributeDontDelete },
            { "getBoundingClientRect", Canvas::getBoundingClientRect, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "HTMLCanvasElement";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.initialize = Canvas::Initializer;
        classDefinition.finalize = Canvas::Finalizer;
        _class = JSC::Class(&classDefinition);

        JSC_GLOBAL_OBJECT.setProperty("HTMLCanvasElement", CreateObject());
    }

    return _class;
}
