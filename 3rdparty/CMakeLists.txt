
if (CMAKE_BUILD_TYPE MATCHES Debug)
    get_property(ICU_I18N_LIBRARIES   TARGET ICU::i18n PROPERTY IMPORTED_LOCATION_DEBUG)
    get_property(ICU_DATA_LIBRARIES   TARGET ICU::data PROPERTY IMPORTED_LOCATION_DEBUG)
    get_property(ICU_COMMON_LIBRARIES TARGET ICU::uc   PROPERTY IMPORTED_LOCATION_DEBUG)
else()
    get_property(ICU_I18N_LIBRARIES   TARGET ICU::i18n PROPERTY IMPORTED_LOCATION_RELEASE)
    get_property(ICU_DATA_LIBRARIES   TARGET ICU::data PROPERTY IMPORTED_LOCATION_RELEASE)
    get_property(ICU_COMMON_LIBRARIES TARGET ICU::uc   PROPERTY IMPORTED_LOCATION_RELEASE)
endif()
get_property(ICU_COMMON_INCLUDE_DIRS TARGET ICU::uc PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

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
        -DENABLE_STATIC_JSC=OFF
        -DENABLE_WEBCORE=OFF
        -DENABLE_WEBKIT=OFF
        -DICU_I18N_LIBRARY=${ICU_I18N_LIBRARIES}
        -DICU_DATA_LIBRARY=${ICU_DATA_LIBRARIES}
        -DICU_LIBRARY=${ICU_COMMON_LIBRARIES}
        -DICU_INCLUDE_DIR=${ICU_COMMON_INCLUDE_DIRS}

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
        ICU::i18n
        ICU::data
        ICU::uc
)

target_include_directories(JavaScriptCore
    INTERFACE
        ${BINARY_DIR}/DerivedSources
        ${BINARY_DIR}/DerivedSources/ForwardingHeaders
)
