#include "JSCHelpers.h"
#include "JSCException.h"

#include <SDL2/SDL_log.h>

namespace JSC
{

Value evaluateScriptFromFile(JSContextRef ctx, const char *path)
{
    FILE *f = NULL;
    Value retval;
    char fnbuf[256];

    // memory overrun protection
    snprintf(fnbuf, sizeof(fnbuf), "%s", path);
    fnbuf[sizeof(fnbuf) - 1] = (char) 0;

    f = fopen(fnbuf, "rb");
    if (f)
    {
        retval = evaluateScriptFromFileHandler(ctx, f, path);
        fclose(f);
    }
    else
    {
        SDL_LogError(0, "Failed to open source file: %s\n", path);
    }

    return retval;

}

Value evaluateScriptFromFileHandler(JSContextRef ctx, FILE *f, const char *sourceURL)
{
    char *buf = NULL;
    size_t bufsz;
    size_t bufoff;
    size_t got;
    Value retval;

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


    retval = evaluateScriptFromString(ctx, std::string(buf, bufoff), sourceURL);

cleanup:
    if (buf) {
        free(buf);
        buf = NULL;
    }
    return retval;
}

Value evaluateScriptFromString(JSContextRef ctx, const std::string &script, const char *sourceURL)
{
    return evaluateScript(ctx, String(ctx, script), String(ctx, sourceURL));
}

Value evaluateScript(JSContextRef ctx, const String &scriptUTF8, const String &sourceURL)
{
    JSValueRef exception = nullptr;
    Value result = Value(ctx, JSEvaluateScript(ctx, scriptUTF8, NULL, sourceURL, 0, &exception));
    if (!result)
    {
        throw Exception(ctx, exception, sourceURL);
    }
    return result;
}
}
