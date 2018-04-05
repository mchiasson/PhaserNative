#include "FileReader.h"

JSC_CONSTRUCTOR(FileReader::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(FileReader::Finalizer)
{
    FreeNativeInstance(object);
}

JSC::Class &FileReader::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "FileReader";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = FileReader::Constructor;
        classDefinition.finalize = FileReader::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
