#pragma once

#include "JSC/JSCHelpers.h"

class Image : public JSC::Binding<Image>
{
    friend class WebGLRenderingContext;

public:

    static JSC::Class &GetClassRef();

    static void OnImageDecoded(void* imageData);

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_PROPERTY_GET(getSrc);
    static JSC_PROPERTY_SET(setSrc);

    JSC::Value m_src;

    uint8_t *m_pixels = nullptr;
};

