cmake_minimum_required(VERSION 3.10.2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set OpenCV path - User will need to update this path
# Use forward slashes or escaped backslashes in CMake paths
set(OpenCV_DIR "C:/Users/kumat/Downloads/opencv-4.12.0-android-sdk/OpenCV-android-sdk/sdk/native/jni")

# Check if OpenCV directory exists
if(NOT EXISTS ${OpenCV_DIR})
    message(WARNING "OpenCV directory not found at: ${OpenCV_DIR}")
    message(WARNING "Please update the OpenCV_DIR variable in CMakeLists.txt")
endif()

find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add verbose logging
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Create our native library
add_library(edgedetection SHARED
            edge_detector.cpp
            gl_renderer.cpp)

add_library(image_processing_util_jni SHARED jni_utils.cpp)

# Link against required libraries
target_link_libraries(edgedetection
                      ${OpenCV_LIBS}
                      android
                      log
                      EGL
                      GLESv2)

target_link_libraries(image_processing_util_jni
                      android
                      log)

# Add compile options
target_compile_options(edgedetection PRIVATE
                       -Wall
                       -Wextra
                       -fexceptions
                       -frtti)

# Set linker flag for 16KB alignment
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=16384")
