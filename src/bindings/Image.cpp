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
    std::shared_ptr<Image> object;
    int channels;
};

Image::~Image() {
    stbi_image_free(pixels);
}


void Image::setSrc(const std::string &src)
{
     this->src = src;

    ImageData *imageData = new ImageData();
    imageData->object = std::dynamic_pointer_cast<Image>(shared_from_this());

     std::thread t([imageData] {

         std::string base64pngMarker = "data:image/png;base64,";

         if(imageData->object->src.compare(0, base64pngMarker.length(), base64pngMarker) == 0)
         {
             std::string base64Data = imageData->object->src.substr(base64pngMarker.length());
             std::string data = decode64(base64Data);

             imageData->object->pixels = stbi_load_from_memory((stbi_uc*)data.c_str(), data.size(), &imageData->object->naturalWidth, &imageData->object->naturalHeight, &imageData->channels, STBI_rgb_alpha);
         }
         else
         {
             FILE* imageFile = fopen(imageData->object->src.c_str(), "rb");

             if(!imageFile)
             {
                 SDL_LogError(0, "Unable to open file '%s'\n", imageData->object->src.c_str());
                 delete imageData;
                 return;
             }

             imageData->object->pixels = stbi_load_from_file(imageFile, &imageData->object->naturalWidth, &imageData->object->naturalHeight, &imageData->channels, STBI_rgb_alpha);
             fclose(imageFile);
         }

//         else if (image.m_src.isObject())
//         {
//             JSC::Object blob = image.m_src.toObject();

//             JSC::Object arrayBuffer = blob.getProperty("_buffer").toObject();

//             stbi_uc *buffer = (stbi_uc *)arrayBuffer.getArrayBufferBytesPtr();
//             int size = arrayBuffer.getArrayBufferByteLength();
//             imageData->pixels = stbi_load_from_memory(buffer, size, &imageData->width, &imageData->height, &imageData->channels, STBI_rgb_alpha);
//         }
//         else
//         {
//             SDL_LogError(0, "Unsupported image source type. Contact a Developer!\n");
//             delete imageData;
//             image.m_src.unprotect();
//             image.object.unprotect();
//             return;
//         }

         SDL_Event event;
         SDL_memset(&event, 0, sizeof(SDL_Event));
         event.type = PhaserNativeEvent::ImageDecoded;
         event.user.data1 = imageData;
         SDL_PushEvent(&event);
     });

     t.detach();
}

void Image::OnImageDecoded(void* ptr)
{
    ImageData *imageData = reinterpret_cast<ImageData *>(ptr);

    imageData->object->complete = true;

    if (imageData->object->clientWidth == 0)
    {
        imageData->object->clientWidth = imageData->object->naturalWidth;
    }

    if (imageData->object->clientHeight == 0)
    {
        imageData->object->clientHeight = imageData->object->naturalHeight;
    }

    if (imageData->object->onload)
    {
        imageData->object->onload();
    }

    delete imageData;
}
