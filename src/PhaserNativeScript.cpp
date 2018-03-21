#include "PhaserNativeScript.h"

#include <SDL2/SDL_log.h>
#include <string>
#include <cassert>

PhaserNativeScript::PhaserNativeScript() :
    m_contextGroup(JSContextGroupCreate()),
    m_globalContext(JSGlobalContextCreateInGroup(m_contextGroup, nullptr))
{
}

PhaserNativeScript::~PhaserNativeScript()
{
    JSGlobalContextRelease(m_globalContext);
    JSContextGroupRelease(m_contextGroup);
}

int PhaserNativeScript::evaluateFromFile(const char *path)
{
    FILE *f = NULL;
    int retval;
    char fnbuf[256];

    // memory overrun protection
    snprintf(fnbuf, sizeof(fnbuf), "%s", path);
    fnbuf[sizeof(fnbuf) - 1] = (char) 0;

    f = fopen(fnbuf, "rb");
    if (!f) {
        SDL_LogError(0, "Failed to open source file: %s\n", path);
        return -1;
    }

    retval = evaluateFromFileHandler(f);

    fclose(f);
    return retval;

}

int PhaserNativeScript::evaluateFromFileHandler(FILE *f)
{
    char* buffer;

    size_t buffer_size = 0;
    size_t buffer_capacity = 1024;
    buffer = (char*)malloc(buffer_capacity);

    while (!feof(f) && !ferror(f)) {
        buffer_size += fread(buffer + buffer_size, 1, buffer_capacity - buffer_size, f);
        if (buffer_size == buffer_capacity) { // guarantees space for trailing '\0'
            buffer_capacity *= 2;
            buffer = (char*)realloc(buffer, buffer_capacity);
        }

        assert(buffer_size < buffer_capacity);
    }

    int retval = evaluateFromString(buffer);

    if (buffer) {
        free(buffer);
        buffer = nullptr;
    }

    return retval;
}

int PhaserNativeScript::evaluateFromString(const char *scriptUTF8)
{

    JSStringRef script = JSStringCreateWithUTF8CString(scriptUTF8);
    JSValueRef exception;
    JSValueRef ret = JSEvaluateScript(m_globalContext, script, nullptr, nullptr, 1, &exception);
    JSStringRelease(script);

    if (ret == nullptr)
    {
        JSStringRef exceptionString = JSValueToStringCopy(m_globalContext, exception, NULL);

        size_t jsSize = JSStringGetMaximumUTF8CStringSize(exceptionString);
        char jsBuffer[jsSize];
        JSStringGetUTF8CString(exceptionString, jsBuffer, jsSize);
        SDL_LogError(0, "%s\n", jsBuffer);
        JSStringRelease(exceptionString);
        return -1;
    }

    return 0;
}
