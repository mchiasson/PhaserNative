#include "PhaserNativeScript.h"

#include <SDL2/SDL_log.h>
#include <string>
#include <cassert>

static char* JSStringToCString (JSStringRef string) {
    size_t size = JSStringGetMaximumUTF8CStringSize(string);
    char *buffer = (char *)calloc(sizeof(char), size);
    JSStringGetUTF8CString(string, buffer, size);
    return buffer;
}

static char* JSValueToCString (JSContextRef context, JSValueRef value,
                               JSValueRef *exception)
{
    JSStringRef string = JSValueToStringCopy(context, value, exception);
    char *result = JSStringToCString(string);
    JSStringRelease(string);
    return result;
}

static void dumpException(JSContextRef context, JSValueRef exception) {
    // description
    char *description = JSValueToCString(context, exception, NULL);
    fprintf(stderr, "%s", description);
    free(description);

    // line
    JSStringRef lineProperty = JSStringCreateWithUTF8CString("line");
    JSValueRef lineValue =
            JSObjectGetProperty(context, (JSObjectRef)exception, lineProperty, NULL);
    int line = JSValueToNumber(context, lineValue, NULL);
    fprintf(stderr, " on line %d", line);
    JSStringRelease(lineProperty);

    // source
    JSStringRef sourceProperty = JSStringCreateWithUTF8CString("sourceURL");
    if (JSObjectGetProperty(context, (JSObjectRef)exception, sourceProperty, NULL)) {
        JSValueRef sourceValue =
                JSObjectGetProperty(context, (JSObjectRef)exception, sourceProperty, NULL);
        char *sourceString = JSValueToCString(context, sourceValue, NULL);
        fprintf(stderr, " in %s", sourceString);
        free(sourceString);
    }
    JSStringRelease(sourceProperty);

    JSStringRef stackProperty = JSStringCreateWithUTF8CString("stack");
    if (JSObjectGetProperty(context, (JSObjectRef)exception, stackProperty, NULL)) {
        fprintf(stderr, ":\n");
        JSValueRef stackValue =
                JSObjectGetProperty(context, (JSObjectRef)exception, stackProperty, NULL);
        char *stack = JSValueToCString(context, stackValue, NULL);
        char *line = strtok(stack, "\n");
        while (line != NULL) {
            if (line[0] == '(')
                fprintf(stderr, "%s\n", line);
            line = strtok(NULL, "\n");
        }
    } else {
        fprintf(stderr, "\n");
    }
    JSStringRelease(stackProperty);
}

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

    retval = evaluateFromFileHandler(f, path);

    fclose(f);
    return retval;

}

int PhaserNativeScript::evaluateFromFileHandler(FILE *f, const char *sourceURL)
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

    int retval = evaluateFromString(buffer, sourceURL);

    if (buffer) {
        free(buffer);
        buffer = nullptr;
    }

    return retval;
}

int PhaserNativeScript::evaluateFromString(const char *scriptUTF8, const char *sourceURL)
{

    JSStringRef script = JSStringCreateWithUTF8CString(scriptUTF8);
    JSStringRef source = JSStringCreateWithUTF8CString(sourceURL);
    JSValueRef exception;
    JSValueRef ret = JSEvaluateScript(m_globalContext, script, nullptr, source, 1, &exception);
    JSStringRelease(script);
    JSStringRelease(source);

    if (ret == nullptr)
    {
        dumpException(m_globalContext, exception);
        return -1;
    }

    return 0;
}


