#include "DocumentElement.h"

JSC_INITIALIZER(DocumentElement::Initializer)
{
    _CreateInstance(object);
}

JSC_FINALIZER(DocumentElement::Finalizer)
{
    _FreeInstance(object);
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
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        classDefinition.staticValues = staticValues;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
