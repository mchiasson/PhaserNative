#include "XMLHttpRequest.h"

#include "PhaserNativeEvent.h"

#include <sstream>
#include <SDL2/SDL_log.h>

JSC_CONSTRUCTOR(XMLHttpRequest::Constructor)
{
    CreateInstance(object);
    return object;
}

JSC_FINALIZER(XMLHttpRequest::Finalizer)
{
    FreeInstance(object);
}

JSC_FUNCTION(XMLHttpRequest::open)
{
    JSC::Value method = argv[0];
    JSC::Value url = argv[1];
    JSC::Value async = argv[2];
    JSC::Value user = argv[3];
    JSC::Value password = argv[4];

    XMLHttpRequest &request = GetInstance(object);

    RequestData requestData;

    if (method.isString())
    {
        requestData.m_method = method.toString().getUTF8String();
    }

    if (url.isString())
    {
        requestData.m_url = url.toString().getUTF8String();
    }

    if (async.isBoolean())
    {
        requestData.m_async = async;
    }

    if (user.isString())
    {
        requestData.m_user = user.toString().getUTF8String();
    }

    if (password.isString())
    {
        requestData.m_password = password.toString().getUTF8String();
    }

    request.m_requestQueue.push_back(requestData);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(XMLHttpRequest::send)
{
    XMLHttpRequest &request = GetInstance(object);

    RequestData &requestData = request.m_requestQueue.back();


    JSC::Value onLoadVal = request.object.getProperty("onload");
    JSC::Value onErrorVal = request.object.getProperty("onerror");
    JSC::Value onProgressVal = request.object.getProperty("onprogress");

    if (onLoadVal.isObject())
    {
        requestData.m_onLoad = onLoadVal.toObject();
    }

    if (onErrorVal.isObject())
    {
        requestData.m_onError = onErrorVal.toObject();
    }

    if (onProgressVal.isObject())
    {
        requestData.m_onProgress = onProgressVal.toObject();
    }

    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    event.type = PhaserNativeEvent::XHR;
    event.user.data1 = object;
    SDL_PushEvent(&event);

    return JSC::Value::MakeUndefined();
}

JSC::Class &XMLHttpRequest::GetClassRef()
{
    if (!_class)
    {
        static JSStaticFunction staticFunctions[] = {
            { "open", XMLHttpRequest::open, kJSPropertyAttributeDontDelete },
            { "send", XMLHttpRequest::send, kJSPropertyAttributeDontDelete },
            { 0, 0, 0 }
        };


        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "XMLHttpRequest";
        classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.callAsConstructor = XMLHttpRequest::Constructor;
        classDefinition.finalize = XMLHttpRequest::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}

void XMLHttpRequest::OnRequest(void* ptr)
{
    XMLHttpRequest &request = GetInstance((JSObjectRef)ptr);

    const RequestData &requestData = request.m_requestQueue.front();

    JSC::Object onLoad = requestData.m_onLoad;
    JSC::Object onError = requestData.m_onError;
    JSC::Object onProgress = requestData.m_onProgress;

    FILE *file = fopen(requestData.m_url.c_str(), "rb");

    if (!file)
    {
        SDL_LogError(0, "could not open %s\n", requestData.m_url.c_str());
        if (onError.isFunction())
        {
            onError.callAsFunction({});
        }
        return;
    }


    fseek(file, 0, SEEK_END); // seek to end of file
    size_t total = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file

    JSC::Object progressEvent;

    if (onProgress.isFunction())
    {
        progressEvent = JSC::Object::MakeDefault();
        progressEvent.setProperty("lengthComputable", true);
        progressEvent.setProperty("loaded", 0);
        progressEvent.setProperty("total", total);
        onProgress.callAsFunction({progressEvent});
    }

    void *buffer = malloc(total);
    size_t loaded = fread(buffer, sizeof(char), total, file);
    fclose(file);

    if (onProgress.isFunction())
    {
        progressEvent.setProperty("loaded", loaded);
        onProgress.callAsFunction({progressEvent});
    }

    JSC::Value responseType = request.object.getProperty("responseType");
    if (responseType && responseType.isString())
    {
        std::string type = responseType.toString().getUTF8String();
        if (type == "arraybuffer")
        {
            JSC::Object response = JSC::Object::MakeArrayBufferWithBytesNoCopy(buffer, total, [](void* buffer, void*) { free(buffer);}, nullptr);
            request.object.setProperty("response", response);
        }
        else if (type == "blob")
        {
            JSC::Object blob = JSC::Object::MakeDefault();
            blob.setProperty("size", total);
            blob.setProperty("type", "application/octet-binary");

            JSC::Object arrayBuffer = JSC::Object::MakeArrayBufferWithBytesNoCopy(buffer, total, [](void* buffer, void*) { free(buffer);}, nullptr);
            blob.setProperty("_buffer", arrayBuffer);

            request.object.setProperty("response", blob);
        }
        else if (type == "text")
        {
            std::string responseText = std::string((const char * )buffer, total);
            JSC::String response = JSC::String(responseText);
            request.object.setProperty("response", JSC::Value(response));
        }
        else
        {
            std::stringstream ss;
            SDL_LogError(0, "Unsupported xhr.responseType '%s'. Please conntact a developer!\n", type.c_str());
        }
    }
    else
    {
        // default is to treat it as text.
        std::string responseText = std::string((const char * )buffer, total);
        JSC::String response = JSC::String(responseText);
        request.object.setProperty("response", JSC::Value(response));
    }

    if (onLoad.isFunction())
    {
        JSC::Object loadEvent = JSC::Object::MakeDefault();
        JSC::Object target = JSC::Object::MakeDefault();
        target.setProperty("status", 200);
        loadEvent.setProperty("target", target);
        onLoad.callAsFunction({loadEvent});
    }

    request.m_requestQueue.pop_front();
}
