#include "Body.h"
#include "HTMLCanvasElement.h"
#include <SDL2/SDL.h>

JSC_CONSTRUCTOR(Body::Constructor)
{
    Body &body = CreateNativeInstance();

    body.object.setProperty("_children", JSC::Object::MakeArray(nullptr, 0), kJSPropertyAttributeDontEnum);

    return body.object;
}

JSC_FINALIZER(Body::Finalizer)
{
    FreeNativeInstance(object);
}

JSC_FUNCTION(Body::appendChild) {

    JSC::Value child = JSC::Value(argv[0]);

    if (child.isObjectOfClass(HTMLCanvasElement::GetClassRef())) {
        SDL_ShowWindow(SDL_GL_GetCurrentWindow());
    }

    Body &body = GetNativeInstance(object);

    JSC::Object array = body.object.getProperty("_children").toObject();
    JSC::Object arrayPush = array.getProperty("push").toObject();

    arrayPush.callAsFunction({child});

    return JSC::Value::MakeUndefined();
}

JSC::Class &Body::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "appendChild", Body::appendChild, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };


        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Body";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = Body::Constructor;
        classDefinition.finalize = Body::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
