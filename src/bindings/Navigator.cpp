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

JSC_INITIALIZER(Navigator::Initializer)
{
    Navigator &navigator = CreateInstance(object);
    navigator.object.setProperty("userAgent", JSC::Value(OS_NAME));
}

JSC_FINALIZER(Navigator::Finalizer)
{
    FreeInstance(object);
}

JSC::Class &Navigator::GetClassRef()
{
    if (!_class)
    {
        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Navigator";
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        classDefinition.initialize = Navigator::Initializer;
        classDefinition.finalize = Navigator::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;

}
