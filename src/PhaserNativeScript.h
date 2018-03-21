#ifndef PHASERNATIVESCRIPT_H
#define PHASERNATIVESCRIPT_H

#include <JavaScriptCore/JavaScript.h>

#include <cstdio>

class PhaserNativeScript
{
public:
    PhaserNativeScript();
    ~PhaserNativeScript();

    int evaluateFromFile(const char *path);
    int evaluateFromFileHandler(FILE *f, const char *sourceURL);
    int evaluateFromString(const char *scriptUTF8, const char *sourceURL);

private:
    JSContextGroupRef m_contextGroup;
    JSGlobalContextRef m_globalContext;
};

#endif // PHASERNATIVESCRIPT_H
