#include "HTMLCanvasElement.h"
#include "CanvasRenderingContext2D.h"
#include "WebGLRenderingContext.h"
#include "PhaserNativeEvent.h"
#include "PhaserNativeGraphics.h"

#include <sstream>

JSC_CONSTRUCTOR(HTMLCanvasElement::Constructor) {

    HTMLCanvasElement &canvas = CreateNativeInstance();

    canvas.window = PhaserNativeCreateWindow();

    return canvas.object;
}

JSC_FINALIZER(HTMLCanvasElement::Finalizer) {
    HTMLCanvasElement &canvas = GetNativeInstance(object);
    PhaserNativeDestroyWindow(canvas.window);
    canvas.window = nullptr;
    FreeNativeInstance(object);
}

JSC_FUNCTION(HTMLCanvasElement::getContext) {

    HTMLCanvasElement &canvas = GetNativeInstance(object);

    JSC::String elementName = JSC::Value(argv[0]).toString();

    JSC::Object context;

    if (elementName == "2d")
    {
        if (canvas.canvas2dIndex == 0)
        {
            context = CanvasRenderingContext2D::CreateJSObject({JSC::Value(canvas.index)});
            canvas.object.setProperty("_2d", context, kJSPropertyAttributeDontEnum);
            canvas.canvas2dIndex = CanvasRenderingContext2D::GetNativeInstance(context).index;
        }
        else
        {
            context = CanvasRenderingContext2D::GetNativeInstance(canvas.canvas2dIndex).object;
        }

    } else if (elementName == "webgl" || "experimental-webgl") {

        if (canvas.glIndex == 0)
        {
            context = WebGLRenderingContext::CreateJSObject({JSC::Value(canvas.index)});
            canvas.object.setProperty("_gl", context, kJSPropertyAttributeDontEnum);
            canvas.glIndex = WebGLRenderingContext::GetNativeInstance(context).index;
        }
        else
        {
            context = WebGLRenderingContext::GetNativeInstance(canvas.glIndex).object;
        }

    } else {

        std::stringstream ss;
        ss << "Unsupported context '" << elementName << "'. Please contact a developer!";
        *exception = JSC::Object::MakeError(ss.str().c_str());
        return JSC::Value::MakeUndefined();
    }

    return context;
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

    HTMLCanvasElement &canvas = GetNativeInstance(object);

    int x, y;
    SDL_GetWindowPosition(canvas.window, &x, &y);

    int w, h;
    SDL_GetWindowSize(canvas.window, &w, &h);

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

JSC_PROPERTY_GET(HTMLCanvasElement::getWidth)
{
    HTMLCanvasElement &canvas = GetNativeInstance(object);

    int w, h;
    SDL_GetWindowSize(canvas.window, &w, &h);
    return JSC::Value(w);
}

JSC_PROPERTY_SET(HTMLCanvasElement::setWidth)
{
    HTMLCanvasElement &canvas = GetNativeInstance(object);

    int w, h;
    SDL_GetWindowSize(canvas.window, &w, &h);
    w = JSC::Value(value);
    SDL_SetWindowSize(canvas.window, w, h);
    return JSC::Value::MakeUndefined();
}

JSC_PROPERTY_GET(HTMLCanvasElement::getHeight)
{
    HTMLCanvasElement &canvas = GetNativeInstance(object);

    int w, h;
    SDL_GetWindowSize(canvas.window, &w, &h);
    return JSC::Value(h);
}

JSC_PROPERTY_SET(HTMLCanvasElement::setHeight)
{
    HTMLCanvasElement &canvas = GetNativeInstance(object);

    int w, h;
    SDL_GetWindowSize(canvas.window, &w, &h);
    h = JSC::Value(value);
    SDL_SetWindowSize(canvas.window, w, h);
    return JSC::Value::MakeUndefined();
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

        static JSStaticValue staticValues[] = {
            { "width", HTMLCanvasElement::getWidth, HTMLCanvasElement::setWidth, kJSPropertyAttributeDontDelete },
            { "height", HTMLCanvasElement::getHeight, HTMLCanvasElement::setHeight, kJSPropertyAttributeDontDelete },
            { 0, 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "HTMLCanvasElement";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = HTMLCanvasElement::Constructor;
        classDefinition.finalize = HTMLCanvasElement::Finalizer;
        _class = JSC::Class(&classDefinition);

    }

    return _class;
}
