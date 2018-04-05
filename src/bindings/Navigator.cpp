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

JSC_CONSTRUCTOR(Navigator::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(Navigator::Finalizer)
{
    FreeNativeInstance(object);
}

JSC::Class &Navigator::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {
            {"userAgent", JSC_CONSTANT(OS_NAME)},
            {0, 0, 0, 0}
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "Navigator";
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = Navigator::Constructor;
        classDefinition.finalize = Navigator::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;

}
