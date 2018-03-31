#include "Image.h"

JSC_CONSTRUCTOR(Image::Constructor) {
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(constructor, (void*)index);

    Image &instance = _GetInstance(index);

    if (argc > 0) { instance.width = JSC::Value(ctx, argv[0]);}
    if (argc > 1) { instance.height = JSC::Value(ctx, argv[1]);}

    return constructor;
}

JSC_FINALIZER(Image::Finalizer) {
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
}

JSC::Class &Image::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {
            { "src", Image::get_src, Image::set_src, kJSPropertyAttributeDontDelete },
            { "width", Image::get_width, Image::set_width, kJSPropertyAttributeDontDelete },
            { "height", Image::get_height, Image::set_width, kJSPropertyAttributeDontDelete },
            { 0, 0, 0, 0 }
        };

        JSClassDefinition imageClassDefinition = kJSClassDefinitionEmpty;
        imageClassDefinition.className = "Image";
        imageClassDefinition.attributes = kJSClassAttributeNone;
        imageClassDefinition.staticValues = staticValues;
        imageClassDefinition.callAsConstructor = Image::Constructor;
        imageClassDefinition.finalize = Image::Finalizer;
        _class = JSC::Class(&imageClassDefinition);
    }

    return _class;
}
