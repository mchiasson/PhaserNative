cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolchains/Desktop-cxx14-c11.cmake" CACHE FILEPATH "Path to the default toolchain to use that forces C++14 and c11 standards")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.20.36.tar.gz"
    SHA1 "9cbf5d9c7ee37c61d903b7f8751dc79d67eb26b4"
)

project(PhaserNative)

hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ColorUtil.cpp
    src/ColorUtil.h
    src/PhaserGL.c
    src/PhaserGL.h
    src/PhaserNativeApp.cpp
    src/PhaserNativeApp.h
    src/PhaserNativeDebugRenderer.h
    src/PhaserNativeDebugRenderer.cpp
    src/PhaserNativeEvent.cpp
    src/PhaserNativeEvent.h
    src/PhaserNativeGraphics.cpp
    src/PhaserNativeGraphics.h
    src/PhaserNativePerfGraph.cpp
    src/PhaserNativePerfGraph.h

    ### Duktape
    3rdparty/duktape/duk_config.h
    3rdparty/duktape/duk_console.c
    3rdparty/duktape/duk_console.h
    3rdparty/duktape/duktape.c
    3rdparty/duktape/duktape.h

    ### Duktape-cpp (binding helper)
    3rdparty/duktape-cpp/Box.h
    3rdparty/duktape-cpp/Constructor.h
    3rdparty/duktape-cpp/Constructor.inl
    3rdparty/duktape-cpp/Context.h
    3rdparty/duktape-cpp/Context.inl
    3rdparty/duktape-cpp/DuktapeCpp.cpp
    3rdparty/duktape-cpp/DuktapeCpp.h
    3rdparty/duktape-cpp/EmptyInspector.h
    3rdparty/duktape-cpp/Exceptions.h
    3rdparty/duktape-cpp/Method.h
    3rdparty/duktape-cpp/PushConstructorInspector.h
    3rdparty/duktape-cpp/PushConstructorInspector.inl
    3rdparty/duktape-cpp/PushObjectInspector.h
    3rdparty/duktape-cpp/PushObjectInspector.inl
    3rdparty/duktape-cpp/Type.h
    3rdparty/duktape-cpp/Type.inl
    3rdparty/duktape-cpp/Types/All.h
    3rdparty/duktape-cpp/Types/Function.h
    3rdparty/duktape-cpp/Types/Primitive.h
    3rdparty/duktape-cpp/Types/SharedPtr.h
    3rdparty/duktape-cpp/Types/STL.h
    3rdparty/duktape-cpp/Types/Tuples.h
    3rdparty/duktape-cpp/Types/UniquePtr.h
    3rdparty/duktape-cpp/Utils/ClassInfo.h
    3rdparty/duktape-cpp/Utils/Helpers.h
    3rdparty/duktape-cpp/Utils/Inspect.h

    ### Javascript Bindings
#    src/bindings/Body.cpp
#    src/bindings/Body.h
#    src/bindings/CanvasRenderingContext2D.cpp
#    src/bindings/CanvasRenderingContext2D.h
#    src/bindings/Console.cpp
#    src/bindings/Console.h
#    src/bindings/Document.cpp
#    src/bindings/Document.h
#    src/bindings/DocumentElement.cpp
#    src/bindings/DocumentElement.h
#    src/bindings/FileReader.cpp
#    src/bindings/FileReader.h
#    src/bindings/HTMLCanvasElement.cpp
#    src/bindings/HTMLCanvasElement.h
#    src/bindings/Image.cpp
#    src/bindings/Image.h
#    src/bindings/KeyboardEvent.cpp
#    src/bindings/KeyboardEvent.h
#    src/bindings/Navigator.cpp
#    src/bindings/Navigator.h
#    src/bindings/Performance.cpp
#    src/bindings/Performance.h
#    src/bindings/URL.cpp
#    src/bindings/URL.h
#    src/bindings/WebGLRenderingContext.cpp
#    src/bindings/WebGLRenderingContext.h
#    src/bindings/Window.cpp
#    src/bindings/Window.h
#    src/bindings/XMLHttpRequest.cpp
#    src/bindings/XMLHttpRequest.h

    ### Khronos
    src/KHR/khrplatform.h

    ### stb
    src/stb/stb_image.h
    src/stb/stb_truetype.h

    ### NanoVG
    src/nanovg/fontstash.h
    src/nanovg/nanovg.c
    src/nanovg/nanovg_gl.h
    src/nanovg/nanovg_gl_utils.h
    src/nanovg/nanovg.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    3rdparty/
    3rdparty/duktape
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
)

target_compile_definitions(${PROJECT_NAME} PRIVATE

)

configure_file(${PROJECT_SOURCE_DIR}/phaser.js ${PROJECT_BINARY_DIR}/phaser.js COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/entypo.ttf ${PROJECT_BINARY_DIR}/entypo.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/NotoEmoji-Regular.ttf ${PROJECT_BINARY_DIR}/NotoEmoji-Regular.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Bold.ttf ${PROJECT_BINARY_DIR}/Roboto-Bold.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Light.ttf ${PROJECT_BINARY_DIR}/Roboto-Light.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Regular.ttf ${PROJECT_BINARY_DIR}/Roboto-Regular.ttf COPYONLY)
