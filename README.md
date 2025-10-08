# Real-Time Edge Detection Viewer

This project implements a real-time edge detection viewer using Android, C++ with OpenCV, OpenGL ES, and a TypeScript web viewer. It processes camera frames in real-time, applies Canny edge detection, and displays the results using OpenGL rendering.

## Project Structure

The project is organized into four main directories:

- `/app`: Android/Kotlin code for the main application
- `/jni`: C++ code for OpenCV processing
- `/gl`: OpenGL ES renderer classes
- `/web`: TypeScript web viewer

## Features

- Real-time camera feed capture using Android CameraX API
- JNI bridge to C++ for image processing
- Canny Edge Detection using OpenCV in C++
- Efficient rendering with OpenGL ES 2.0+
- Performance of 10-15+ FPS (device-dependent)
- Frame statistics display (FPS, resolution)
- TypeScript web viewer for displaying processed frames
- Clean architecture with separation of concerns

## Setup Instructions

### Prerequisites

- Android Studio (latest version)
- Android NDK (r21 or newer)
- CMake (3.18.1 or newer)
- OpenCV for Android SDK (4.5.0 or newer)
- Node.js and npm (for web viewer development)

### Setting up OpenCV

1. Download OpenCV for Android SDK from [OpenCV releases](https://opencv.org/releases/)
2. Extract the SDK to a location of your choice
3. Update the `OpenCV_DIR` path in `app/src/main/cpp/CMakeLists.txt` to point to your OpenCV SDK location:

```cmake
set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../opencv-sdk/sdk/native/jni)
```

### Building the Android App

1. Open the project in Android Studio
2. Sync Gradle files
3. Connect an Android device or use an emulator (physical device recommended for camera access)
4. Build and run the project

### Building the Web Viewer

1. Navigate to the `/web` directory
2. Install dependencies:
   ```
   npm install
   ```
3. Build the TypeScript files:
   ```
   npm run build
   ```
4. Serve the files using a local web server (e.g., `npx http-server`)

## Architecture

### Android Application Flow

1. The Android app captures camera frames using CameraX API
2. Each frame is passed to the native C++ code via JNI
3. The C++ code processes the frame using OpenCV's Canny edge detection
4. The processed frame is uploaded directly to an OpenGL texture
5. The OpenGL renderer displays the texture on a GLSurfaceView
6. Performance statistics are calculated and displayed

### Web Viewer

The TypeScript web viewer provides a simplified view of the edge detection results:

1. Loads a static sample image of a processed frame
2. Displays the image on an HTML canvas
3. Simulates frame stats (FPS, resolution)
4. Updates the DOM with these statistics

## Technical Details

### Native C++ Processing

The native C++ code handles the most compute-intensive part of the application:

1. Converts camera data to OpenCV Mat format
2. Applies grayscale conversion and Gaussian blur for noise reduction
3. Runs Canny edge detection with configurable parameters
4. Uploads the processed frame directly to an OpenGL texture

### OpenGL ES Rendering

For efficient rendering:

1. Uses OpenGL ES 2.0+ with custom shaders
2. Renders the processed frame as a full-screen quad
3. Optimizes texture transfer from OpenCV to OpenGL
4. Minimizes CPU usage by doing processing on the GPU when possible

## License

MIT License - See LICENSE file for details

## Acknowledgments

- OpenCV team for the computer vision library
- Android team for the CameraX API
