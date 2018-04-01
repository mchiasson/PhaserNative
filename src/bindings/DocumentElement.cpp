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
        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "DocumentElement";
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
