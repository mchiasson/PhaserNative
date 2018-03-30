#pragma once

#include "JSC/JSCHelpers.h"

#include <vector>


JSC_BINDINGS(Image)
{

    JSC_CLASS(HTMLImageElement) {
        JSC_CONSTRUCTOR(Constructor) {
            JSC_CREATE_INSTANCE(instance);

            if (argc > 0) { instance.width = argv[0];}
            if (argc > 1) { instance.height = argv[1];}

            return constructor;
        }

        JSC_DESTRUCTOR(Destructor) {
            JSC_GET_INSTANCE(instance);

            instance.src = nullptr;
            instance.width = nullptr;
            instance.height = nullptr;

            JSC_DESTROY_INSTANCE(instance);
        }

        JSC_RW_PROPERTY(src)
        JSC_RW_PROPERTY(width)
        JSC_RW_PROPERTY(height)
    };


    static JSStaticValue staticValues[] = {
        { "src", HTMLImageElement::get_src, HTMLImageElement::set_src, kJSPropertyAttributeDontDelete },
        { "width", HTMLImageElement::get_width, HTMLImageElement::set_width, kJSPropertyAttributeDontDelete },
        { "height", HTMLImageElement::get_height, HTMLImageElement::set_width, kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
    };

    auto ctx = JSC::globalContext();

    JSClassDefinition imageClassDefinition = kJSClassDefinitionEmpty;
    imageClassDefinition.className = "Image";
    imageClassDefinition.attributes = kJSClassAttributeNone;
    imageClassDefinition.staticValues = staticValues;
    imageClassDefinition.callAsConstructor = HTMLImageElement::Constructor;
    imageClassDefinition.finalize = HTMLImageElement::Destructor;
    JSClassRef image_class = JSClassCreate(&imageClassDefinition);

    JSC::String imageName = JSC::String(ctx, "Image");
    JSC::Object imageObject = JSC::Object::Make(ctx, image_class);
    JSObjectSetProperty(ctx,
                        JSContextGetGlobalObject(ctx),
                        imageName,
                        imageObject,
                        kJSPropertyAttributeDontDelete, nullptr);
}
