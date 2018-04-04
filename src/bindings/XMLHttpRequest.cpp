#include "XMLHttpRequest.h"

#include <sstream>

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

    if (method.isString())
    {
        request.m_method = method.toString().getUTF8String();
    }

    if (url.isString())
    {
        request.m_url = url.toString().getUTF8String();
    }

    if (async.isBoolean())
    {
        request.m_async = async;
    }

    if (user.isString())
    {
        request.m_user = user.toString().getUTF8String();
    }

    if (password.isString())
    {
        request.m_password = password.toString().getUTF8String();
    }

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(XMLHttpRequest::send)
{
    XMLHttpRequest &request = GetInstance(object);

    JSC::Object onLoad;
    {
        JSC::Value onLoadVal = request.object.getProperty("onLoad");
        if (onLoadVal.isObject()) {
            onLoad = onLoadVal.toObject();
        }
    }

    JSC::Object onError;
    {
        JSC::Value onErrorVal = request.object.getProperty("onError");
        if (onErrorVal.isObject()) {
            onError = onErrorVal.toObject();
        }
    }

    JSC::Object onProgress;
    {
        JSC::Value onProgressVal = request.object.getProperty("onProgress");
        if (onProgressVal.isObject()) {
            onProgress = onProgressVal.toObject();
        }
    }




    FILE *file = fopen(request.m_url.c_str(), "rb");

    if (!file)
    {
        if (onError.isFunction())
        {
            onError.callAsFunction({});
        }
        return JSC::Value::MakeUndefined();
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

    if (onLoad.isFunction())
    {
        JSC::Object loadEvent = JSC::Object::MakeDefault();
        JSC::Object target = JSC::Object::MakeDefault();
        target.setProperty("status", 200);
        loadEvent.setProperty("target", target);
        onLoad.callAsFunction({loadEvent});
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
            ss << "Unsupported xhr.responseType '"
                << type
                << "'. Please conntact a developer!";

            *exception = JSC::Object::MakeError(ss.str().c_str());
        }
    }
    else
    {
        // default is to treat it as text.
        std::string responseText = std::string((const char * )buffer, total);
        JSC::String response = JSC::String(responseText);
        request.object.setProperty("response", JSC::Value(response));
    }


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
