#include "Image.h"

#include <SDL2/SDL_log.h>
#include <SDL2/SDL_events.h>
#include "PhaserNativeEvent.h"

#include <thread>
#include <stb/stb_image.h>
#include <cstdio>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

std::string decode64(const std::string &val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
        return c == '\0';
    });
}

std::string encode64(const std::string &val) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

struct ImageData
{
    JSObjectRef object;
    int width, height, channels;
    uint8_t *pixels;
};

JSC_CONSTRUCTOR(Image::Constructor) {
    Image &image = CreateNativeInstance();

    if (argc > 0) { image.object.setProperty("width", argv[0]); }
    if (argc > 1) { image.object.setProperty("height", argv[1]); }

    return image.object;
}

JSC_FINALIZER(Image::Finalizer) {
    stbi_image_free(GetNativeInstance(object).m_pixels);
    FreeNativeInstance(object);
}

JSC_PROPERTY_GET(Image::getSrc)
{
    return GetNativeInstance(object).m_src;
}

JSC_PROPERTY_SET(Image::setSrc)
{
     GetNativeInstance(object).m_src = value;


     std::thread t([object] {

         ImageData *imageData = new ImageData();
         imageData->object = object;

         std::string src = GetNativeInstance(object).m_src.toString().getUTF8String();

         std::string base64pngMarker = "data:image/png;base64,";

         if(src.compare(0, base64pngMarker.length(), base64pngMarker) == 0)
         {
             std::string base64Data = src.substr(base64pngMarker.length());
             std::string data = decode64(base64Data);

             imageData->pixels = stbi_load_from_memory((stbi_uc*)data.c_str(), data.size(), &imageData->width, &imageData->height, &imageData->channels, 4);
         }
         else
         {
             FILE* imageFile = fopen(src.c_str(), "rb");

             if(!imageFile)
             {
                 SDL_LogError(0, "Unable to open file '%s'\n", src.c_str());
                 delete imageData;
                 return;
             }

             imageData->pixels = stbi_load_from_file(imageFile, &imageData->width, &imageData->height, &imageData->channels, 4);
             fclose(imageFile);
         }


         SDL_Event event;
         SDL_memset(&event, 0, sizeof(SDL_Event));
         event.type = PhaserNativeEvent::ImageDecoded;
         event.user.data1 = imageData;
         SDL_PushEvent(&event);

     });

     t.detach();


    return true;

}

JSC::Class &Image::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {
            { "src", Image::getSrc, Image::setSrc, kJSPropertyAttributeDontDelete },
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

void Image::OnImageDecoded(void* ptr)
{
    ImageData *imageData = reinterpret_cast<ImageData *>(ptr);

    Image &instance = GetNativeInstance(imageData->object);
    instance.object.setProperty("complete", JSC::Value(true));
    instance.object.setProperty("currentSrc", instance.m_src);
    instance.object.setProperty("naturalWidth", JSC::Value(imageData->width));
    instance.object.setProperty("naturalHeight", JSC::Value(imageData->height));

    if (instance.object.getProperty("width").isUndefined())
    {
        instance.object.setProperty("width", JSC::Value(imageData->width));
    }

    if (instance.object.getProperty("height").isUndefined())
    {
        instance.object.setProperty("height", JSC::Value(imageData->height));
    }

    instance.m_pixels = imageData->pixels;


    JSC::Value onloadVal = instance.object.getProperty("onload");
    if (onloadVal && onloadVal.isObject())
    {
        JSC::Object onload = onloadVal.toObject();
        if (onload.isFunction())
        {
            onload.callAsFunction({});
        }
    }


    delete imageData;
}
