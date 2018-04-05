#pragma once

#include "JSC/JSCHelpers.h"


class FileReader : public JSC::Binding<FileReader> {
public:
    static JSC::Class &GetClassRef();

private:
    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);
};
