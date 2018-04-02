#include "WebGLRenderingContext.h"
#include "PhaserGL.h"

#include <cstring>

#include <SDL2/SDL_log.h>

JSC_INITIALIZER(WebGLRenderingContext::Initializer) {
    _CreateInstance(object);
}

JSC_FINALIZER(WebGLRenderingContext::Finalizer) {
    _FreeInstance(object);
}

JSC_FUNCTION(WebGLRenderingContext::attachShader) {
    JSC::Value program = argv[0];
    JSC::Value shader = argv[1];

    glAttachShader(program, shader);

    return JSC::Value::MakeUndefined();
}


JSC_FUNCTION(WebGLRenderingContext::bindBuffer) {
    JSC::Value target = argv[0];
    JSC::Value buffer = argv[1];
    glBindBuffer(target, buffer);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::blendEquation) {

    JSC::Value mode = argv[0];

#ifndef NDEBUG
    if (mode.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported mode! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    glBlendEquation(mode);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::blendFunc) {

    JSC::Value sfactor = argv[0];
    JSC::Value dfactor = argv[1];

#ifndef NDEBUG
    if (sfactor.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported sfactor! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (dfactor.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported dfactor! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    glBlendFunc(sfactor, dfactor);
    return JSC::Value::MakeUndefined();
}



JSC_FUNCTION(WebGLRenderingContext::bufferData) {

    JSC::Value target     = argv[0];
    JSC::Value dataOrSize = argv[1];
    JSC::Value usage      = argv[2];

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (usage.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported usage! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    if (dataOrSize.isNumber())
    {
        glBufferData(target, dataOrSize, nullptr, usage);
    }
    else if (dataOrSize.isObject())
    {
        JSC::Object arrayBuffer = dataOrSize.toObject();
        void* data = arrayBuffer.getArrayBufferBytesPtr();
        GLsizeiptr size = arrayBuffer.getArrayBufferByteLength();
        glBufferData(target, size,  data, usage);
    }
    else
    {
        *exception = JSC::Object::MakeError("Unknown type! Please contact a developer");
    }

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::clearColor) {
    JSC::Value r = argv[0];
    JSC::Value g = argv[0];
    JSC::Value b = argv[0];
    JSC::Value a = argv[0];

    glClearColor(r, g, b, a);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::compileShader) {
    JSC::Value shader = argv[0];
    glCompileShader(shader);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::createBuffer)
{
    GLuint buffer;
    glCreateBuffers(1, &buffer);
    return JSC::Value(buffer);
}

JSC_FUNCTION(WebGLRenderingContext::createProgram)
{
    return JSC::Value(glCreateProgram());
}

JSC_FUNCTION(WebGLRenderingContext::createShader)
{
    JSC::Value type = argv[0];

#ifndef NDEBUG
    if (type.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported type! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    return JSC::Value(glCreateShader(type));
}

JSC_FUNCTION(WebGLRenderingContext::disable) {
    JSC::Value cap = argv[0];

#ifndef NDEBUG
    if (cap.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported capability! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    glDisable(cap);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::enable) {

    JSC::Value cap = argv[0];

#ifndef NDEBUG
    if (cap.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported capability! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    glEnable(cap);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::getProgramParameter) {
    JSC::Value shader = argv[0];
    JSC::Value pname = argv[1];

    GLint params;
    glGetProgramiv(shader, pname, &params);

    return JSC::Value(params);
}

JSC_FUNCTION(WebGLRenderingContext::getShaderParameter) {
    JSC::Value shader = argv[0];
    JSC::Value pname = argv[1];

    GLint params;
    glGetShaderiv(shader, pname, &params);

    return JSC::Value(params);
}

JSC_FUNCTION(WebGLRenderingContext::getSupportedExtensions) {

    std::vector<JSValueRef> extensions;

    if (phaserGLSupport(GL_VERSION_3_0) || phaserGLSupport(GL_ES_VERSION_3_0)) {
        GLint i = 0;
        GLint nExtensionsCount = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensionsCount);
        while(i < nExtensionsCount) {
            JSC::Value extension = (const char*)glGetStringi(GL_EXTENSIONS, i++);
            extensions.push_back(extension);
        }
    } else {
        const char* extensionsStr = (const char*)glGetString(GL_EXTENSIONS);
        while (extensionsStr) {
            const char* spacerPos = strchr(extensionsStr, ' ');
            size_t extensionsStrLength = spacerPos ? (khronos_usize_t)(spacerPos++ - extensionsStr) : strlen(extensionsStr);

            extensions.push_back(JSC::Value(std::string(extensionsStr, extensionsStrLength)));

            extensionsStr = spacerPos;
        }
    }

    return JSC::Object::MakeArray(&extensions.front(), extensions.size());
}

JSC_FUNCTION(WebGLRenderingContext::linkProgram) {
    JSC::Value program = argv[0];
    glLinkProgram(program);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::shaderSource)
{
    JSC::Value shader = argv[0];
    std::string source = JSC::Value(argv[1]).toString().getUTF8String();

    const char *string = source.c_str();
    GLint length = source.size();

    glShaderSource(shader, 1, &string, &length);

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::viewport) {
    JSC::Value x      = argv[0];
    JSC::Value y      = argv[1];
    JSC::Value width  = argv[2];
    JSC::Value height = argv[3];
    glViewport(x, y, width, height);
    return JSC::Value::MakeUndefined();
}

JSC::Class &WebGLRenderingContext::GetClassRef()
{
    if (!_class)
    {

#define JSC_GL_FUNC(_FUNC_NAME_) { #_FUNC_NAME_, WebGLRenderingContext::_FUNC_NAME_, kJSPropertyAttributeDontDelete}

        static JSStaticFunction staticFunctions[] = {
            JSC_GL_FUNC(attachShader),
            JSC_GL_FUNC(bindBuffer),
            JSC_GL_FUNC(blendEquation),
            JSC_GL_FUNC(blendFunc),
            JSC_GL_FUNC(bufferData),
            JSC_GL_FUNC(clearColor),
            JSC_GL_FUNC(compileShader),
            JSC_GL_FUNC(createBuffer),
            JSC_GL_FUNC(createProgram),
            JSC_GL_FUNC(createShader),
            JSC_GL_FUNC(disable),
            JSC_GL_FUNC(enable),
            JSC_GL_FUNC(getProgramParameter),
            JSC_GL_FUNC(getShaderParameter),
            JSC_GL_FUNC(getSupportedExtensions),
            JSC_GL_FUNC(linkProgram),
            JSC_GL_FUNC(shaderSource),
            JSC_GL_FUNC(viewport),
            { 0, 0, 0 }
        };

#define JSC_GL_CONSTANT(_CONSTANT_NAME_) { #_CONSTANT_NAME_, JSC_CONSTANT(GL_##_CONSTANT_NAME_) }

        static JSStaticValue staticValues[] = {
            JSC_GL_CONSTANT(ARRAY_BUFFER),
            JSC_GL_CONSTANT(BLEND),
            JSC_GL_CONSTANT(COMPILE_STATUS),
            JSC_GL_CONSTANT(CONSTANT_ALPHA),
            JSC_GL_CONSTANT(CONSTANT_COLOR),
            JSC_GL_CONSTANT(CULL_FACE),
            JSC_GL_CONSTANT(DEPTH_TEST),
            JSC_GL_CONSTANT(DST_ALPHA),
            JSC_GL_CONSTANT(DST_COLOR),
            JSC_GL_CONSTANT(DYNAMIC_DRAW),
            JSC_GL_CONSTANT(ELEMENT_ARRAY_BUFFER),
            JSC_GL_CONSTANT(FRAGMENT_SHADER),
            JSC_GL_CONSTANT(FUNC_ADD),
            JSC_GL_CONSTANT(FUNC_REVERSE_SUBTRACT),
            JSC_GL_CONSTANT(FUNC_SUBTRACT),
            JSC_GL_CONSTANT(LINK_STATUS),
            JSC_GL_CONSTANT(ONE),
            JSC_GL_CONSTANT(ONE_MINUS_CONSTANT_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_CONSTANT_COLOR),
            JSC_GL_CONSTANT(ONE_MINUS_DST_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_DST_COLOR),
            JSC_GL_CONSTANT(ONE_MINUS_SRC_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_SRC_COLOR),
            JSC_GL_CONSTANT(SCISSOR_TEST),
            JSC_GL_CONSTANT(SRC_ALPHA),
            JSC_GL_CONSTANT(SRC_ALPHA_SATURATE),
            JSC_GL_CONSTANT(SRC_COLOR),
            JSC_GL_CONSTANT(STATIC_DRAW),
            JSC_GL_CONSTANT(STREAM_DRAW),
            JSC_GL_CONSTANT(VERTEX_SHADER),
            JSC_GL_CONSTANT(ZERO),
            { 0, 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "WebGLRenderingContext";
        classDefinition.attributes = kJSClassAttributeNone;
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.initialize = WebGLRenderingContext::Initializer;
        classDefinition.finalize = WebGLRenderingContext::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
