
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("androidluaapp")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             androidluaapp

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             androidluaapp.cpp )
add_subdirectory("lua-lua-5.3")
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

include_directories("lua-lua-5.3/src" "${CMAKE_CURRENT_BINARY_DIR}"  "${CMAKE_CURRENT_BINARY_DIR}/lua-lua-5.3")
message(WARNING "INCLUDE ...     ${CMAKE_CURRENT_BINARY_DIR} ")
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       androidluaapp
             GLESv2
        android
                        liblua
                       ${log-lib} )
