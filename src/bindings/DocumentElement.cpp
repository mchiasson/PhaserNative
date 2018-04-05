#include "DocumentElement.h"

JSC_CONSTRUCTOR(DocumentElement::Constructor)
{
    return CreateNativeInstance().object;
}

JSC_FINALIZER(DocumentElement::Finalizer)
{
    FreeNativeInstance(object);
}

JSC::Class &DocumentElement::GetClassRef()
{
    if (!_class)
    {
        static JSStaticValue staticValues[] = {

            {"clientLeft", JSC_CONSTANT(0)},
            {"clientTop", JSC_CONSTANT(0)},

            {0, 0, 0, 0}
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "DocumentElement";
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = DocumentElement::Constructor;
        classDefinition.finalize = DocumentElement::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
