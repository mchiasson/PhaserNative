#ifndef PHASERNATIVESCRIPT_H
#define PHASERNATIVESCRIPT_H

#include <JavaScriptCore/JavaScript.h>

#include <cstdio>
#include <string>

class PhaserNativeScript
{
public:
    PhaserNativeScript();
    ~PhaserNativeScript();

    int evaluateFromFile(const char *path);
    int evaluateFromFileHandler(FILE *f, const char *sourceURL);
    int evaluateFromString(const std::string &scriptUTF8, const char *sourceURL);

    void registerAllBindings();
};

#endif // PHASERNATIVESCRIPT_H
