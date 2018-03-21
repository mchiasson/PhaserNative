#include "PhaserNativeScript.h"

#include <SDL2/SDL_log.h>
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

static void dumpException(JSContextRef context, JSValueRef exception)
{
    // line
    JSStringRef lineProperty = JSStringCreateWithUTF8CString("line");
    JSValueRef lineValue = JSObjectGetProperty(context, JSValueToObject(context, exception, NULL), lineProperty, NULL);
    int line = JSValueToNumber(context, lineValue, NULL);
    JSStringRelease(lineProperty);

    // source
    JSStringRef sourceProperty = JSStringCreateWithUTF8CString("sourceURL");
    JSValueRef sourceValue = JSObjectGetProperty(context, JSValueToObject(context, exception, NULL), sourceProperty, NULL);
    if (sourceValue)
    {
        char *sourceString = JSValueToCString(context, sourceValue, NULL);
        fprintf(stderr, "%s:%d\n", sourceString, line);
        free(sourceString);
    }
    JSStringRelease(sourceProperty);

    // description
    char *description = JSValueToCString(context, exception, NULL);
    fprintf(stderr, "%s\n", description);
    free(description);

    JSStringRef stackProperty = JSStringCreateWithUTF8CString("stack");
    JSValueRef stackValue = JSObjectGetProperty(context, JSValueToObject(context, exception, NULL), stackProperty, NULL);
    if (stackValue)
    {
        char *stack = JSValueToCString(context, stackValue, NULL);
        char *line = strtok(stack, "\n");
        while (line != NULL)
        {
            if (line[0] == '(')
            {
                fprintf(stderr, "%s\n", line);
            }
            line = strtok(NULL, "\n");
        }
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
    char *buf = NULL;
    size_t bufsz;
    size_t bufoff;
    size_t got;
    int retval = 0;

    buf = (char *) malloc(1024);
    if (!buf) {
        SDL_LogError(0, "Out of memory.\n");
        goto cleanup;
    }
    bufsz = 1024;
    bufoff = 0;

    /* Read until EOF, avoid fseek/stat because it won't work with stdin. */
    for (;;) {
        size_t avail;

        avail = bufsz - bufoff;
        if (avail < 1024) {
            size_t newsz;
            char *buf_new;
            newsz = bufsz + (bufsz >> 2) + 1024;  /* +25% and some extra */
            buf_new = (char *) realloc(buf, newsz);
            if (!buf_new) {
                SDL_LogError(0, "Out of memory.\n");
                goto cleanup;
            }
            buf = buf_new;
            bufsz = newsz;
        }

        avail = bufsz - bufoff;

        got = fread((void *) (buf + bufoff), (size_t) 1, avail, f);
        if (got == 0) {
            break;
        }
        bufoff += got;
    }


    retval = evaluateFromString(std::string(buf, bufoff), sourceURL);

cleanup:
    if (buf) {
        free(buf);
        buf = NULL;
    }
    return retval;
}

int PhaserNativeScript::evaluateFromString(const std::string &scriptUTF8, const char *sourceURL)
{

    JSStringRef script = JSStringCreateWithUTF8CString(scriptUTF8.c_str());
    JSStringRef source = JSStringCreateWithUTF8CString(sourceURL);
    JSValueRef exception = nullptr;
    JSEvaluateScript(m_globalContext, script, nullptr, source, 1, &exception);
    JSStringRelease(script);
    JSStringRelease(source);

    if (exception)
    {
        dumpException(m_globalContext, exception);
        return -1;
    }

    return 0;
}


