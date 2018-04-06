#include "HTMLCanvasElement.h"
#include "CanvasRenderingContext2D.h"
#include "WebGLRenderingContext.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeWindow.h"

JSC_CONSTRUCTOR(HTMLCanvasElement::Constructor) {
    HTMLCanvasElement &canvas = CreateNativeInstance();

    JSC::Object canvas2d = CanvasRenderingContext2D::CreateJSObject({canvas.object});
    JSC::Object gl = WebGLRenderingContext::CreateJSObject({canvas.object});

    canvas2d.setProperty("canvas", canvas.object);
    gl.setProperty("canvas", canvas.object);

    canvas.object.setProperty("2d", canvas2d, kJSPropertyAttributeDontEnum);
    canvas.object.setProperty("webgl", gl, kJSPropertyAttributeDontEnum);
    canvas.object.setProperty("experimental-webgl", gl, kJSPropertyAttributeDontEnum);

    return canvas.object;
}

JSC_FINALIZER(HTMLCanvasElement::Finalizer) {
    FreeNativeInstance(object);
}

JSC_FUNCTION(HTMLCanvasElement::getContext) {

    HTMLCanvasElement &instance = GetNativeInstance(object);

    JSC::String elementName = JSC::Value(argv[0]).toString();
    return instance.object.getProperty(elementName);
}

JSC_FUNCTION(HTMLCanvasElement::addEventListener)
{
    JSC::Value type = argv[0];
    JSC::Object listener = JSC::Value(argv[1]).toObject();

    std::string typeStr = type.toString().getUTF8String();
    SDL_Log("Registering listener for '%s'", typeStr.c_str());
    std::set<JSC::Object> &listeners = PhaserNativeEvent::eventListeners[typeStr];
    listeners.insert(listener);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(HTMLCanvasElement::removeEventListener)
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

JSC_FUNCTION(HTMLCanvasElement::getBoundingClientRect) {

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


JSC::Class &HTMLCanvasElement::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "getContext", HTMLCanvasElement::getContext, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "addEventListener", HTMLCanvasElement::addEventListener, kJSPropertyAttributeDontDelete },
            { "removeEventListener", HTMLCanvasElement::removeEventListener, kJSPropertyAttributeDontDelete },
            { "getBoundingClientRect", HTMLCanvasElement::getBoundingClientRect, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "HTMLCanvasElement";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = HTMLCanvasElement::Constructor;
        classDefinition.finalize = HTMLCanvasElement::Finalizer;
        _class = JSC::Class(&classDefinition);

    }

    return _class;
}
