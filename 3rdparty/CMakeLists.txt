include(ExternalProject)

ExternalProject_Add(ExternalJavaScriptCore

    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/webkit

    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DPORT=JSCOnly
        -DENABLE_TOOLS=OFF
        -DENABLE_API_TESTS=OFF
        -DENABLE_STATIC_JSC=ON
        -DENABLE_WEBCORE=OFF
        -DENABLE_WEBKIT=OFF

    INSTALL_COMMAND ""

    BUILD_BYPRODUCTS
        <BINARY_DIR>/lib/libbmalloc.a
        <BINARY_DIR>/lib/libJavaScriptCore.a
        <BINARY_DIR>/lib/libWTF.a

    LOG_DOWNLOAD 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
)

ExternalProject_Get_Property(ExternalJavaScriptCore BINARY_DIR)

add_library(JavaScriptCore INTERFACE)
add_dependencies(JavaScriptCore ExternalJavaScriptCore)

target_link_libraries(JavaScriptCore
    INTERFACE
        ${BINARY_DIR}/lib/libJavaScriptCore.a
        ${BINARY_DIR}/lib/libWTF.a
        ${BINARY_DIR}/lib/libbmalloc.a
        icui18n
        icudata
        icuuc
)

target_include_directories(JavaScriptCore
    INTERFACE
        ${BINARY_DIR}/DerivedSources
        ${BINARY_DIR}/DerivedSources/ForwardingHeaders
)
