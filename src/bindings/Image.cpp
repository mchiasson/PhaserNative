#include "Image.h"

JSC_CONSTRUCTOR(Image::Constructor) {
    Image &instance = _CreateInstance(object);

    if (argc > 0) { instance.width = argv[0];}
    if (argc > 1) { instance.height = argv[1];}

    return object;
}

JSC_FINALIZER(Image::Finalizer) {
    _FreeInstance(object);
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
