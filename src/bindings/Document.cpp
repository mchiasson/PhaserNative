#include "Document.h"

#include <SDL2/SDL_log.h>

#include "HTMLCanvasElement.h"
//#include "Body.h"


duk_ret_t Document::createElement(duk::Context &ctx, duk_idx_t nargs) {

    const char *tagName;
    duk::Type<const char *>::get(ctx, tagName, 0);

    if (strcmp(tagName, "canvas") == 0)
    {
        duk::Type< std::shared_ptr<HTMLCanvasElement> >::push(ctx, std::make_shared<HTMLCanvasElement>());
        return 1;
    }


    SDL_LogWarn(0, "Element type '%s' is currently not supported. Contact a Developer!\n", tagName);
    duk_push_object(ctx);
    return 1;
}

//JSC_CONSTRUCTOR(Document::Constructor)
//{
//    Document &document = CreateNativeInstance();

//    document.object.setProperty("body", Body::CreateJSObject({}));
//    document.object.setProperty("readyState", JSC::Value("complete")); // pretend the DOM is ready.

//    return document.object;
//}

