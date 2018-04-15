#include "Navigator.h"

#if defined(__ANDROID__)
    #define OS_NAME "Android"
#elif defined(__linux__)
    #define OS_NAME "Linux"
#elif defined(_WIN32)
    #define OS_NAME "Windows"
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
        #define OS_NAME "iPhone"
    #elif TARGET_OS_MAC == 1
        #define OS_NAME "Mac OS"
    #endif
#endif

#ifndef OS_NAME
    #error "Unknown/Unsupported OS"
#endif

const char * Navigator::userAgent() const
{
    return OS_NAME;
}
