cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Toolchains/Desktop-cxx11-c99.cmake" CACHE FILEPATH "Path to the default toolchain to use that forces C++11 and c99 standards")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.20.14.tar.gz"
    SHA1 "18cdbf5c47abdda437c73bf5437b7c3c65afe47c"
)

project(PhaserNative)

find_package(JavaScriptCore REQUIRED)

hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/PhaserGL.c
    src/PhaserGL.h
    src/PhaserNativeApp.cpp
    src/PhaserNativeApp.h
    src/PhaserNativeEvent.cpp
    src/PhaserNativeEvent.h
    src/PhaserNativeScript.cpp
    src/PhaserNativeScript.h
    src/PhaserNativeWindow.h
    src/PhaserNativeWindow.cpp

    ### Khronos
    src/KHR/khrplatform.h

    ### JavaScriptCore Helpers
    src/JSC/JSCConsole.cpp
    src/JSC/JSCException.cpp
    src/JSC/JSCException.h
    src/JSC/JSCHelpers.cpp
    src/JSC/JSCHelpers.h
    src/JSC/JSCObject.cpp
    src/JSC/JSCObject.h
    src/JSC/JSCString.h
    src/JSC/JSCTimer.cpp
    src/JSC/JSCValue.cpp
    src/JSC/JSCValue.h

    ### NanoVG
    src/nanovg/fontstash.h
    src/nanovg/nanovg.c
    src/nanovg/nanovg_gl.h
    src/nanovg/nanovg_gl_utils.h
    src/nanovg/nanovg.h
    src/nanovg/perf.c
    src/nanovg/perf.h
    src/nanovg/stb_image.h
    src/nanovg/stb_truetype.h

)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    ${JavaScriptCore_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${JavaScriptCore_LIBRARIES}
    SDL2::SDL2
    Boost::boost
)

target_compile_definitions(${PROJECT_NAME} PRIVATE

)

configure_file(${PROJECT_SOURCE_DIR}/entypo.ttf ${PROJECT_BINARY_DIR}/entypo.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/NotoEmoji-Regular.ttf ${PROJECT_BINARY_DIR}/NotoEmoji-Regular.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Bold.ttf ${PROJECT_BINARY_DIR}/Roboto-Bold.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Light.ttf ${PROJECT_BINARY_DIR}/Roboto-Light.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Roboto-Regular.ttf ${PROJECT_BINARY_DIR}/Roboto-Regular.ttf COPYONLY)
