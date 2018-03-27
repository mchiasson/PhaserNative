#include "PhaserNativeScript.h"

#include <SDL2/SDL_log.h>
#include <cassert>

#include "JSC/JSCHelpers.h"

PhaserNativeScript::PhaserNativeScript()
{
}

PhaserNativeScript::~PhaserNativeScript()
{
    JSGlobalContextRelease(JSC::globalContext());
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

int PhaserNativeScript::evaluateFromString(const std::string &script, const char *sourceURL)
{
    JSC::evaluateScript(JSC::globalContext(), script, sourceURL);

    return 0;
}

void PhaserNativeScript::registerAllBindings()
{
    for (size_t i = 0; i < JSC::bindings().size(); ++i) {
        JSC::bindings()[i]();
    }

    JSC::bindings().clear();
    JSC::bindings().resize(0);
}

