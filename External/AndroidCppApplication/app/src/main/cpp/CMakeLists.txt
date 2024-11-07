
cmake_minimum_required(VERSION 3.22.1)


project("androidcppapplication")


# mono


add_library(
        libmonosgen-2.0
        SHARED
        IMPORTED
)

add_library(
        libSystemSecurity
        SHARED
        IMPORTED
)

add_library(
        libSystem.Native
        SHARED
        IMPORTED
)


add_library(
        libSystem.IO.Compression.Native
        SHARED
        IMPORTED
)


add_library(
        libmono-component-debugger
        SHARED
        IMPORTED
)

add_library(
        libmono-component-hot_reload
        SHARED
        IMPORTED
)


add_library(
        libmono-component-diagnostics_tracing
        SHARED
        IMPORTED
)

include_directories(${CMAKE_SOURCE_DIR}/include)
set_target_properties(
        libmonosgen-2.0
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmonosgen-2.0.so
)

set_target_properties(
        libSystemSecurity
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libSystem.Security.Cryptography.Native.Android.so
)

set_target_properties(
        libSystem.Native
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libSystem.Native.so
)

set_target_properties(
        libSystem.IO.Compression.Native
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libSystem.IO.Compression.Native.so
)

set_target_properties(
        libmono-component-debugger
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmono-component-debugger.so
)

set_target_properties(
        libmono-component-hot_reload
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmono-component-hot_reload.so
)

set_target_properties(
        libmono-component-diagnostics_tracing
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libmono-component-diagnostics_tracing.so
)


##############################################################################


add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp)


target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        libmonosgen-2.0
        libSystemSecurity
        libSystem.Native
        libSystem.IO.Compression.Native
        libmono-component-debugger
        libmono-component-hot_reload
        libmono-component-diagnostics_tracing

        android
        log)
