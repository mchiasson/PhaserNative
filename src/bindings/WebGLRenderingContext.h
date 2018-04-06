#pragma once

#include <JSC/JSCHelpers.h>

class WebGLRenderingContext : public JSC::Binding<WebGLRenderingContext>
{
public:

    static JSC::Class &GetClassRef();

    static JSC_CONSTRUCTOR(Constructor);
    static JSC_FINALIZER(Finalizer);

    static JSC_FUNCTION(attachShader);
    static JSC_FUNCTION(bindBuffer);
    static JSC_FUNCTION(bindTexture);
    static JSC_FUNCTION(blendEquation);
    static JSC_FUNCTION(blendFunc);
    static JSC_FUNCTION(bufferData);
    static JSC_FUNCTION(bufferSubData);
    static JSC_FUNCTION(clear);
    static JSC_FUNCTION(clearColor);
    static JSC_FUNCTION(compileShader);
    static JSC_FUNCTION(createBuffer);
    static JSC_FUNCTION(createProgram);
    static JSC_FUNCTION(createShader);
    static JSC_FUNCTION(createTexture);
    static JSC_FUNCTION(disable);
    static JSC_FUNCTION(drawArrays);
    static JSC_FUNCTION(enable);
    static JSC_FUNCTION(enableVertexAttribArray);
    static JSC_FUNCTION(getAttribLocation);
    static JSC_FUNCTION(getProgramParameter);
    static JSC_FUNCTION(getShaderParameter);
    static JSC_FUNCTION(getSupportedExtensions);
    static JSC_FUNCTION(getUniformLocation);
    static JSC_FUNCTION(linkProgram);
    static JSC_FUNCTION(pixelStorei);
    static JSC_FUNCTION(shaderSource);
    static JSC_FUNCTION(texImage2D);
    static JSC_FUNCTION(texParameteri);
    static JSC_FUNCTION(uniformMatrix4fv);
    static JSC_FUNCTION(useProgram);
    static JSC_FUNCTION(vertexAttribPointer);
    static JSC_FUNCTION(viewport);

};

