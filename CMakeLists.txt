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

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/PhaserNativeApp.cpp
    src/PhaserNativeApp.h
    src/PhaserNativeScript.cpp
    src/PhaserNativeScript.h
    src/PhaserNativeWindow.h
    src/PhaserNativeWindow.cpp
)

message("JavaScriptCore_INCLUDE_DIRS=${JavaScriptCore_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    src/SDL2
    ${JavaScriptCore_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${JavaScriptCore_LIBRARIES}
    SDL2::SDL2
)

target_compile_definitions(${PROJECT_NAME} PRIVATE

)
