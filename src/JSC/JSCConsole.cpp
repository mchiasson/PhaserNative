#include <SDL2/SDL_log.h>

#include "JSCHelpers.h"

JSC_BINDINGS(JSCConsole)
{
    static JSStaticFunction staticFunctions[] = {
        { "log", [](JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef /*thiz*/, size_t argc, const JSValueRef argv[], JSValueRef* exception) -> JSValueRef {
              try {
                  std::string output;
                  for(size_t i = 0; i < argc; i++) {
                      if (i > 0){ output += " "; }
                      output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
                  }
                  SDL_Log("%s\n", output.c_str());
              } catch(const std::exception & e) {
                  JSC::Value message = JSC::Value::MakeString(ctx, e.what());
                  JSValueRef args[] { message, nullptr };
                  *exception = JSObjectMakeError(ctx, 1, args, nullptr);
              }
              return JSC::Value::MakeUndefined(ctx);
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "info", [](JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef /*thiz*/, size_t argc, const JSValueRef argv[], JSValueRef* exception) -> JSValueRef {
              try {
                  std::string output;
                  for(size_t i = 0; i < argc; i++) {
                      if (i > 0){ output += " "; }
                      output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
                  }
                  SDL_LogInfo(0, "%s\n", output.c_str());
              } catch(const std::exception & e) {
                  JSC::Value message = JSC::Value::MakeString(ctx, e.what());
                  JSValueRef args[] { message, nullptr };
                  *exception = JSObjectMakeError(ctx, 1, args, nullptr);
              }
              return JSC::Value::MakeUndefined(ctx);
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "warn", [](JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef /*thiz*/, size_t argc, const JSValueRef argv[], JSValueRef* exception) -> JSValueRef {
              try {
                  std::string output;
                  for(size_t i = 0; i < argc; i++) {
                      if (i > 0){ output += " "; }
                      output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
                  }
                  SDL_LogWarn(0, "%s\n", output.c_str());
              } catch(const std::exception & e) {
                  JSC::Value message = JSC::Value::MakeString(ctx, e.what());
                  JSValueRef args[] { message, nullptr };
                  *exception = JSObjectMakeError(ctx, 1, args, nullptr);
              }
              return JSC::Value::MakeUndefined(ctx);
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { "error", [](JSContextRef ctx, JSObjectRef /*function*/, JSObjectRef /*thiz*/, size_t argc, const JSValueRef argv[], JSValueRef* exception) -> JSValueRef {
              try {
                  std::string output;
                  for(size_t i = 0; i < argc; i++) {
                      if (i > 0){ output += " "; }
                      output += JSC::Value(ctx, argv[i]).toString().getUTF8String();
                  }
                  SDL_LogError(0, "%s\n", output.c_str());
              } catch(const std::exception & e) {
                  JSC::Value message = JSC::Value::MakeString(ctx, e.what());
                  JSValueRef args[] { message, nullptr };
                  *exception = JSObjectMakeError(ctx, 1, args, nullptr);
              }
              return JSC::Value::MakeUndefined(ctx);
          }, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { 0, 0, 0 }
    };

    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;
    classDefinition.className = "console";
    classDefinition.attributes = kJSClassAttributeNoAutomaticPrototype;
    classDefinition.staticFunctions = staticFunctions;
    JSClassRef console_class = JSClassCreate(&classDefinition);

    auto ctx = JSC::globalContext();
    JSC::String propertyName = JSC::String(ctx, "console");
    JSObjectSetProperty(ctx,
                        JSContextGetGlobalObject(ctx),
                        propertyName,
                        JSC::Object::Make(ctx, console_class),
                        kJSPropertyAttributeNone, nullptr);

}
