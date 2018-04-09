cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolchains/Desktop-cxx14-c11.cmake" CACHE FILEPATH "Path to the default toolchain to use that forces C++14 and c11 standards")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.20.14.tar.gz"
    SHA1 "18cdbf5c47abdda437c73bf5437b7c3c65afe47c"
)

project(PhaserNative)

add_subdirectory(3rdparty)

hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ColorUtil.cpp
    src/ColorUtil.h
    src/PhaserGL.c
    src/PhaserGL.h
    src/PhaserNativeApp.cpp
    src/PhaserNativeApp.h
    src/PhaserNativeEvent.cpp
    src/PhaserNativeEvent.h
    src/PhaserNativeGraphics.cpp
    src/PhaserNativeGraphics.h
    src/PhaserNativePerfGraph.cpp
    src/PhaserNativePerfGraph.h
    src/PhaserNativeWindow.h
    src/PhaserNativeWindow.cpp

    ### Javascript Bindings
    src/bindings/Body.cpp
    src/bindings/Body.h
    src/bindings/CanvasRenderingContext2D.cpp
    src/bindings/CanvasRenderingContext2D.h
    src/bindings/Console.cpp
    src/bindings/Console.h
    src/bindings/Document.cpp
    src/bindings/Document.h
    src/bindings/DocumentElement.cpp
    src/bindings/DocumentElement.h
    src/bindings/FileReader.cpp
    src/bindings/FileReader.h
    src/bindings/HTMLCanvasElement.cpp
    src/bindings/HTMLCanvasElement.h
    src/bindings/Image.cpp
    src/bindings/Image.h
    src/bindings/Navigator.cpp
    src/bindings/Navigator.h
    src/bindings/Performance.cpp
    src/bindings/Performance.h
    src/bindings/URL.cpp
    src/bindings/URL.h
    src/bindings/WebGLRenderingContext.cpp
    src/bindings/WebGLRenderingContext.h
    src/bindings/Window.cpp
    src/bindings/Window.h
    src/bindings/XMLHttpRequest.cpp
    src/bindings/XMLHttpRequest.h

    ### Khronos
    src/KHR/khrplatform.h

    ### stb
    src/stb/stb_image.h
    src/stb/stb_truetype.h

    ### JavaScriptCore Helpers
    src/JSC/JSCClass.h
    src/JSC/JSCContext.cpp
    src/JSC/JSCContext.h
    src/JSC/JSCException.cpp
    src/JSC/JSCException.h
    src/JSC/JSCHelpers.cpp
    src/JSC/JSCHelpers.h
    src/JSC/JSCObject.cpp
    src/JSC/JSCObject.h
    src/JSC/JSCString.h
    src/JSC/JSCValue.cpp
    src/JSC/JSCValue.h

    ### NanoVG
    src/nanovg/fontstash.h
    src/nanovg/nanovg.c
    src/nanovg/nanovg_gl.h
    src/nanovg/nanovg_gl_utils.h
    src/nanovg/nanovg.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    JavaScriptCore
    SDL2::SDL2
    Boost::boost
)

target_compile_definitions(${PROJECT_NAME} PRIVATE

)

configure_file(${PROJECT_SOURCE_DIR}/phaser.js ${PROJECT_BINARY_DIR}/phaser.js COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/entypo.ttf ${PROJECT_BINARY_DIR}/entypo.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/NotoEmoji-Regular.ttf ${PROJECT_BINARY_DIR}/NotoEmoji-Regular.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Bold.ttf ${PROJECT_BINARY_DIR}/Roboto-Bold.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Light.ttf ${PROJECT_BINARY_DIR}/Roboto-Light.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Regular.ttf ${PROJECT_BINARY_DIR}/Roboto-Regular.ttf COPYONLY)
