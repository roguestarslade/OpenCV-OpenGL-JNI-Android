# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("controller")
set(pathPROJECT D:/Ubuntu/Files/Project_5/controller)
set(pathOPENCV ${pathPROJECT}/sdk)
set(pathLIBOPENCV_JAVA ${pathOPENCV}/native/libs/${ANDROID_ABI}/libopencv_java4.so)

# configure import TF libs
set(tf_DIR ${CMAKE_SOURCE_DIR}/../../../../tensorflow)

# This states that there exists a shared library called libtensorflowLite
# which will be imported (means it is not built with the rest of the project!)
add_library(libtensorflowLite SHARED IMPORTED)
# This indicates where the libtensorflowLite.so for each architecture is found relative to our distribution directory
set_target_properties(libtensorflowLite PROPERTIES IMPORTED_LOCATION
		${tf_DIR}/lib/${ANDROID_ABI}/libtensorflowlite_c.so)

#gpu_delegate
add_library(libtensorflowLite_gpu SHARED IMPORTED)
set_target_properties(libtensorflowLite_gpu PROPERTIES IMPORTED_LOCATION
		${tf_DIR}/lib/${ANDROID_ABI}/libtensorflowlite_gpu_delegate.so)

#libnnapi_util
add_library(libnnapi_util SHARED IMPORTED)
set_target_properties(libnnapi_util PROPERTIES IMPORTED_LOCATION
		${tf_DIR}/lib/${ANDROID_ABI}/libnnapi_util.a)

#libnnapi_implementation
add_library(libnnapi_implementation SHARED IMPORTED)
set_target_properties(libnnapi_implementation PROPERTIES IMPORTED_LOCATION
		${tf_DIR}/lib/${ANDROID_ABI}/libnnapi_implementation.a)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(${pathOPENCV}/native/jni/include)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(
		# Sets the name of the library.
		native-lib
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        native-lib.cpp
        utils.h
        detect_obj.cpp
        detect_obj.h
        point_tracker.h
        point_tracker.cpp
        point_obj.cpp
        point_obj.h
	)

# This indicates where the header files are found relative to our distribution dir
target_include_directories(native-lib PRIVATE
		${tf_DIR}/include)

add_library( lib_opencv SHARED IMPORTED )

set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathLIBOPENCV_JAVA})


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        lib_opencv
		libtensorflowLite
		libtensorflowLite_gpu
		libnnapi_util
		libnnapi_implementation
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
