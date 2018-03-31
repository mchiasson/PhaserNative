#include "DocumentElement.h"

JSC_INITIALIZER(DocumentElement::Initializer)
{
    size_t index = _AllocateInstance();
    JSObjectSetPrivate(object, (void*)index);

}

JSC_FINALIZER(DocumentElement::Finalizer)
{
    size_t index = (size_t)JSObjectGetPrivate(object);
    _FreeInstance(index);
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
