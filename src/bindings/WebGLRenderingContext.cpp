#include "WebGLRenderingContext.h"
#include "Image.h"
#include "HTMLCanvasElement.h"

#include <cstring>

#include "PhaserNativeGraphics.h"

JSC_CONSTRUCTOR(WebGLRenderingContext::Constructor) {

    WebGLRenderingContext &gl = CreateNativeInstance();

    gl.canvasIndex = JSC::Value(argv[0]);

#ifdef NANOVG_GL3_IMPLEMENTATION
    // hack for core profile to make things work without using VAOs: create a
    // blank VAO and bind it.
    glGenVertexArrays(1, &gl.vao);
    glBindVertexArray(gl.vao);
#endif

    return gl.object;
}

JSC_FINALIZER(WebGLRenderingContext::Finalizer) {
    WebGLRenderingContext &gl = GetNativeInstance(object);

#ifdef NANOVG_GL3_IMPLEMENTATION
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &gl.vao);
    gl.vao = 0;
#endif

    SDL_GL_MakeCurrent(nullptr, nullptr);
    SDL_GL_DeleteContext(gl.context);

    gl.context = nullptr;
    gl.canvasIndex = 0;

    FreeNativeInstance(object);
}

JSC_FUNCTION(WebGLRenderingContext::attachShader) {
    WebGLRenderingContext &gl = GetNativeInstance(object);

    JSC::Value program = argv[0];
    JSC::Value shader = argv[1];

    phaserGLCheckErrorDebug( glAttachShader(program, shader) );

    return JSC::Value::MakeUndefined();
}


JSC_FUNCTION(WebGLRenderingContext::bindBuffer) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value target = argv[0];
    JSC::Value buffer = argv[1];

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glBindBuffer(target, buffer) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::bindTexture) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value target = argv[0];
    JSC::Value texture = argv[1];

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glBindTexture(target, texture) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::blendEquation) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value mode = argv[0];

#ifndef NDEBUG
    if (mode.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported mode! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glBlendEquation(mode) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::blendFunc) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


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

    phaserGLCheckErrorDebug( glBlendFunc(sfactor, dfactor) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::bufferData) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


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
        phaserGLCheckErrorDebug( glBufferData(target, dataOrSize, nullptr, usage) );
    }
    else if (dataOrSize.isObject())
    {
        JSC::Object arrayBuffer = dataOrSize.toObject();
        void* data = arrayBuffer.getArrayBufferBytesPtr();
        GLsizeiptr size = arrayBuffer.getArrayBufferByteLength();
        phaserGLCheckErrorDebug( glBufferData(target, size,  data, usage) );
    }
    else
    {
        *exception = JSC::Object::MakeError("Unknown type! Please contact a developer");
    }

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::bufferSubData) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value target  = argv[0];
    JSC::Value offset  = argv[1];
    JSC::Object srcData = JSC::Value(argv[2]).toObject();

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    void* data = srcData.getTypedArrayBytesPtr();
    GLsizeiptr size = srcData.getTypedArrayByteLength();
    phaserGLCheckErrorDebug( glBufferSubData(target, offset, size, data) );

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::clear) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value mask = argv[0];
    glClear(mask);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::clearColor) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value r = argv[0];
    JSC::Value g = argv[0];
    JSC::Value b = argv[0];
    JSC::Value a = argv[0];

    glClearColor(r, g, b, a);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::compileShader) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value shader = argv[0];
    phaserGLCheckErrorDebug( glCompileShader(shader) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::createBuffer) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    GLuint buffer;
    phaserGLCheckErrorDebug( glGenBuffers(1, &buffer) );
    return JSC::Value(buffer);
}

JSC_FUNCTION(WebGLRenderingContext::createProgram) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    auto program = phaserGLCheckErrorDebug( glCreateProgram() );
    return JSC::Value(program);
}

JSC_FUNCTION(WebGLRenderingContext::createShader) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value type = argv[0];

#ifndef NDEBUG
    if (type.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported type! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    auto shader = phaserGLCheckErrorDebug( glCreateShader(type) );

    return JSC::Value(shader);
}

JSC_FUNCTION(WebGLRenderingContext::createTexture) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    GLuint texture;
    phaserGLCheckErrorDebug( glGenTextures(1, &texture) );
    return JSC::Value(texture);
}

JSC_FUNCTION(WebGLRenderingContext::disable) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value cap = argv[0];

#ifndef NDEBUG
    if (cap.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported capability! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glDisable(cap) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::drawArrays) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value mode = argv[0];
    JSC::Value first = argv[1];
    JSC::Value count = argv[2];

    phaserGLCheckErrorDebug( glDrawArrays(mode, first, count) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::enable) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value cap = argv[0];

#ifndef NDEBUG
    if (cap.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported capability! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glEnable(cap) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::enableVertexAttribArray) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value index = argv[0];
    phaserGLCheckErrorDebug( glEnableVertexAttribArray(index) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::getAttribLocation) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value program = argv[0];
    JSC::Value name = argv[1];

    auto location = phaserGLCheckErrorDebug( glGetAttribLocation(program, name.toString().getUTF8String().c_str()) );
    return JSC::Value(location);
}

JSC_FUNCTION(WebGLRenderingContext::getProgramParameter) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value shader = argv[0];
    JSC::Value pname = argv[1];

#ifndef NDEBUG
    if (pname.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported pname! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    GLint params;
    phaserGLCheckErrorDebug( glGetProgramiv(shader, pname, &params) );

    return JSC::Value(params);
}

JSC_FUNCTION(WebGLRenderingContext::getShaderParameter) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value shader = argv[0];
    JSC::Value pname = argv[1];

#ifndef NDEBUG
    if (pname.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported pname! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    GLint params;
    phaserGLCheckErrorDebug( glGetShaderiv(shader, pname, &params) );

    return JSC::Value(params);
}

JSC_FUNCTION(WebGLRenderingContext::getUniformLocation) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value program = argv[0];
    JSC::Value name = argv[1];
    auto location = phaserGLCheckErrorDebug( glGetUniformLocation(program, name.toString().getUTF8String().c_str()) );
    return JSC::Value(location);
}

JSC_FUNCTION(WebGLRenderingContext::getSupportedExtensions) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    std::vector<JSValueRef> extensions;

    if (phaserGLSupport(GL_VERSION_3_0) || phaserGLSupport(GL_ES_VERSION_3_0)) {
        GLint i = 0;
        GLint nExtensionsCount = 0;
        phaserGLCheckErrorDebug( glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensionsCount) );
        while(i < nExtensionsCount) {
            JSC::Value extension = (const char*)glGetStringi(GL_EXTENSIONS, i++);
            extensions.push_back(extension);
        }
    } else {
        const char* extensionsStr = (const char*) phaserGLCheckErrorDebug( glGetString(GL_EXTENSIONS) );
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
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value program = argv[0];
    phaserGLCheckErrorDebug( glLinkProgram(program) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::scissor) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value x = argv[0];
    JSC::Value y = argv[1];
    JSC::Value w = argv[2];
    JSC::Value h = argv[3];
    glScissor(x, y, w, h);
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::shaderSource) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value shader = argv[0];
    std::string source = JSC::Value(argv[1]).toString().getUTF8String();

    const char *string = source.c_str();
    GLint length = source.size();

    phaserGLCheckErrorDebug( glShaderSource(shader, 1, &string, &length) );

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::pixelStorei) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value pnameVal = argv[0];
    JSC::Value param = argv[1];

#ifndef NDEBUG
    if (pnameVal.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported pname! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (param.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported param! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    int pname = pnameVal.toInteger();
    if (!(pname == GL_UNPACK_COLORSPACE_CONVERSION_WEBGL ||
          pname == GL_UNPACK_FLIP_Y_WEBGL ||
          pname == GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL))
    {
        phaserGLCheckErrorDebug( glPixelStorei(pname, param) );
    }
    return JSC::Value::MakeUndefined();

}

JSC_FUNCTION(WebGLRenderingContext::texImage2D) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value target = argv[0];
    JSC::Value level = argv[1];
    JSC::Value internalformat = argv[2];
    JSC::Value format = argv[3];
    JSC::Value type = argv[4];
    JSC::Value imageVal = argv[5];

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (internalformat.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported internalformat! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (format.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported format! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (type.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported type! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

#endif

    Image &image = Image::GetNativeInstance(imageVal.toObject());

    try
    {
        if (image.object.hasProperty("width") && image.object.hasProperty("height"))
        {
//            phaserGLCheckErrorDebug( glTexImage2D(target,
//                                                  level,
//                                                  internalformat,
//                                                  image.object.getProperty("width"),
//                                                  image.object.getProperty("height"),
//                                                  0,
//                                                  format,
//                                                  type,
//                                                  image.m_pixels) );
        }

    }
    catch (JSC::Exception &e)
    {
        *exception = JSC::Object::MakeError(e.what(), e.getStack().c_str());
    }

    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::texParameteri) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value target = argv[0];
    JSC::Value pname = argv[1];
    JSC::Value param = argv[2];

#ifndef NDEBUG
    if (target.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (pname.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported pname! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }

    if (param.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported param! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    phaserGLCheckErrorDebug( glTexParameteri(target, pname, param) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::useProgram) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value program = argv[0];
    phaserGLCheckErrorDebug( glUseProgram(program) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::uniformMatrix4fv) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value location = argv[0];
    JSC::Value transpose = argv[1];
    JSC::Value value = argv[2];

    phaserGLCheckErrorDebug( glUniformMatrix4fv(location, 1, transpose.toBoolean(), (const GLfloat*)value.toObject().getTypedArrayBytesPtr()) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::vertexAttribPointer) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value indexVal = argv[0];
    JSC::Value sizeVal = argv[1];
    JSC::Value typeVal = argv[2];
    JSC::Value normalizedVal = argv[3];
    JSC::Value strideVal = argv[4];
    JSC::Value offsetVal = argv[5];

#ifndef NDEBUG
    if (typeVal.isUndefined())
    {
        *exception = JSC::Object::MakeError("unsupported target! Please contact a developer");
        return JSC::Value::MakeUndefined();
    }
#endif

    GLuint index = indexVal.toUnsignedInteger();
    GLint size = sizeVal.toInteger();
    GLenum type = typeVal.toUnsignedInteger();
    GLboolean normalized = normalizedVal.toBoolean() ? GL_TRUE : GL_FALSE;
    GLsizei stride = strideVal.toInteger();
    void* offset = (void*)offsetVal.toLongInteger();

    phaserGLCheckErrorDebug( glVertexAttribPointer(index, size, type, normalized, stride, offset) );
    return JSC::Value::MakeUndefined();
}

JSC_FUNCTION(WebGLRenderingContext::viewport) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);


    JSC::Value x      = argv[0];
    JSC::Value y      = argv[1];
    JSC::Value width  = argv[2];
    JSC::Value height = argv[3];
    phaserGLCheckErrorDebug( glViewport(x, y, width, height) );
    return JSC::Value::MakeUndefined();
}

JSC_PROPERTY_GET(WebGLRenderingContext::getDrawingBufferWidth) {
    int w, h;
    SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &w, &h);
    return JSC::Value(w);
}


JSC_PROPERTY_GET(WebGLRenderingContext::getDrawingBufferHeight) {
    int w, h;
    SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &w, &h);
    return JSC::Value(h);
}

JSC_PROPERTY_GET(WebGLRenderingContext::getCanvas) {
    WebGLRenderingContext &gl = GetNativeInstance(object);
    HTMLCanvasElement &canvas = HTMLCanvasElement::GetNativeInstance(gl.canvasIndex);
    return canvas.object;
}


JSC::Class &WebGLRenderingContext::GetClassRef()
{
    if (!_class)
    {

#define JSC_GL_FUNC(_FUNC_NAME_) { #_FUNC_NAME_, WebGLRenderingContext::_FUNC_NAME_, kJSPropertyAttributeDontDelete}

        static JSStaticFunction staticFunctions[] = {
            JSC_GL_FUNC(attachShader),
            JSC_GL_FUNC(bindBuffer),
            JSC_GL_FUNC(bindTexture),
            JSC_GL_FUNC(blendEquation),
            JSC_GL_FUNC(blendFunc),
            JSC_GL_FUNC(bufferData),
            JSC_GL_FUNC(bufferSubData),
            JSC_GL_FUNC(clear),
            JSC_GL_FUNC(clearColor),
            JSC_GL_FUNC(compileShader),
            JSC_GL_FUNC(createBuffer),
            JSC_GL_FUNC(createProgram),
            JSC_GL_FUNC(createShader),
            JSC_GL_FUNC(createTexture),
            JSC_GL_FUNC(disable),
            JSC_GL_FUNC(drawArrays),
            JSC_GL_FUNC(enable),
            JSC_GL_FUNC(enableVertexAttribArray),
            JSC_GL_FUNC(getAttribLocation),
            JSC_GL_FUNC(getProgramParameter),
            JSC_GL_FUNC(getShaderParameter),
            JSC_GL_FUNC(getUniformLocation),
            JSC_GL_FUNC(getSupportedExtensions),
            JSC_GL_FUNC(linkProgram),
            JSC_GL_FUNC(pixelStorei),
            JSC_GL_FUNC(scissor),
            JSC_GL_FUNC(shaderSource),
            JSC_GL_FUNC(texImage2D),
            JSC_GL_FUNC(texParameteri),
            JSC_GL_FUNC(uniformMatrix4fv),
            JSC_GL_FUNC(useProgram),
            JSC_GL_FUNC(uniformMatrix4fv),
            JSC_GL_FUNC(vertexAttribPointer),
            JSC_GL_FUNC(viewport),
            { 0, 0, 0 }
        };

#define JSC_GL_CONSTANT(_CONSTANT_NAME_) { #_CONSTANT_NAME_, JSC_CONSTANT(GL_##_CONSTANT_NAME_) }

        static JSStaticValue staticValues[] = {
            JSC_GL_CONSTANT(ALPHA),
            JSC_GL_CONSTANT(ARRAY_BUFFER),
            JSC_GL_CONSTANT(BLEND),
            JSC_GL_CONSTANT(BYTE),
            JSC_GL_CONSTANT(CLAMP_TO_EDGE),
            JSC_GL_CONSTANT(COLOR_BUFFER_BIT),
            JSC_GL_CONSTANT(COMPILE_STATUS),
            JSC_GL_CONSTANT(CONSTANT_ALPHA),
            JSC_GL_CONSTANT(CONSTANT_COLOR),
            JSC_GL_CONSTANT(CULL_FACE),
            JSC_GL_CONSTANT(DEPTH_BUFFER_BIT),
            JSC_GL_CONSTANT(DEPTH_TEST),
            JSC_GL_CONSTANT(DST_ALPHA),
            JSC_GL_CONSTANT(DST_COLOR),
            JSC_GL_CONSTANT(DYNAMIC_DRAW),
            JSC_GL_CONSTANT(ELEMENT_ARRAY_BUFFER),
            JSC_GL_CONSTANT(FLOAT),
            JSC_GL_CONSTANT(FRAGMENT_SHADER),
            JSC_GL_CONSTANT(FUNC_ADD),
            JSC_GL_CONSTANT(FUNC_REVERSE_SUBTRACT),
            JSC_GL_CONSTANT(FUNC_SUBTRACT),
            JSC_GL_CONSTANT(LINEAR),
            JSC_GL_CONSTANT(LINEAR_MIPMAP_LINEAR),
            JSC_GL_CONSTANT(LINEAR_MIPMAP_NEAREST),
            JSC_GL_CONSTANT(LINES),
            JSC_GL_CONSTANT(LINE_LOOP),
            JSC_GL_CONSTANT(LINE_STRIP),
            JSC_GL_CONSTANT(LINK_STATUS),
            JSC_GL_CONSTANT(LUMINANCE),
            JSC_GL_CONSTANT(LUMINANCE_ALPHA),
            JSC_GL_CONSTANT(MIRRORED_REPEAT),
            JSC_GL_CONSTANT(NEAREST),
            JSC_GL_CONSTANT(NEAREST_MIPMAP_LINEAR),
            JSC_GL_CONSTANT(NEAREST_MIPMAP_NEAREST),
            JSC_GL_CONSTANT(ONE),
            JSC_GL_CONSTANT(ONE_MINUS_CONSTANT_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_CONSTANT_COLOR),
            JSC_GL_CONSTANT(ONE_MINUS_DST_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_DST_COLOR),
            JSC_GL_CONSTANT(ONE_MINUS_SRC_ALPHA),
            JSC_GL_CONSTANT(ONE_MINUS_SRC_COLOR),
            JSC_GL_CONSTANT(PACK_ALIGNMENT),
            JSC_GL_CONSTANT(POINTS),
            JSC_GL_CONSTANT(REPEAT),
            JSC_GL_CONSTANT(RGB),
            JSC_GL_CONSTANT(RGBA),
            JSC_GL_CONSTANT(SCISSOR_TEST),
            JSC_GL_CONSTANT(SHORT),
            JSC_GL_CONSTANT(SRC_ALPHA),
            JSC_GL_CONSTANT(SRC_ALPHA_SATURATE),
            JSC_GL_CONSTANT(SRC_COLOR),
            JSC_GL_CONSTANT(STATIC_DRAW),
            JSC_GL_CONSTANT(STENCIL_BUFFER_BIT),
            JSC_GL_CONSTANT(STREAM_DRAW),
            JSC_GL_CONSTANT(TEXTURE_2D),
            JSC_GL_CONSTANT(TEXTURE_MAG_FILTER),
            JSC_GL_CONSTANT(TEXTURE_MIN_FILTER),
            JSC_GL_CONSTANT(TEXTURE_WRAP_S),
            JSC_GL_CONSTANT(TEXTURE_WRAP_T),
            JSC_GL_CONSTANT(TRIANGLES),
            JSC_GL_CONSTANT(TRIANGLE_FAN),
            JSC_GL_CONSTANT(TRIANGLE_STRIP),
            JSC_GL_CONSTANT(UNPACK_ALIGNMENT),
            JSC_GL_CONSTANT(UNPACK_COLORSPACE_CONVERSION_WEBGL),
            JSC_GL_CONSTANT(UNPACK_FLIP_Y_WEBGL),
            JSC_GL_CONSTANT(UNPACK_PREMULTIPLY_ALPHA_WEBGL),
            JSC_GL_CONSTANT(UNSIGNED_BYTE),
            JSC_GL_CONSTANT(UNSIGNED_BYTE),
            JSC_GL_CONSTANT(UNSIGNED_SHORT),
            JSC_GL_CONSTANT(UNSIGNED_SHORT_4_4_4_4),
            JSC_GL_CONSTANT(UNSIGNED_SHORT_5_5_5_1),
            JSC_GL_CONSTANT(UNSIGNED_SHORT_5_6_5),
            JSC_GL_CONSTANT(VERTEX_SHADER),
            JSC_GL_CONSTANT(ZERO),

            { "drawingBufferWidth", WebGLRenderingContext::getDrawingBufferWidth, nullptr, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "drawingBufferHeight", WebGLRenderingContext::getDrawingBufferHeight, nullptr, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "canvas", WebGLRenderingContext::getCanvas, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },

            { 0, 0, 0, 0 }
        };

        JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
        classDefinition.className = "WebGLRenderingContext";
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;
        classDefinition.callAsConstructor = WebGLRenderingContext::Constructor;
        classDefinition.finalize = WebGLRenderingContext::Finalizer;
        _class = JSC::Class(&classDefinition);
    }

    return _class;
}
