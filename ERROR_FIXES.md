# Common Errors and Fixes

This document lists all common errors you might encounter and their solutions.

## Build Errors

### 1. "Unsupported class file major version 65" - Java Version Mismatch

**Error Message:**
```
BUG! exception in phase 'semantic analysis' in source unit '_BuildScript_' 
Unsupported class file major version 65
```

**Cause:**
You're using Java 21 (class file version 65), but your Gradle version doesn't support it.

**Solution:**
1. **Option A - Update Gradle (Recommended):**
   - Open `gradle/wrapper/gradle-wrapper.properties`
   - Change to Gradle 8.5 or newer:
     ```properties
     distributionUrl=https\://services.gradle.org/distributions/gradle-8.5-bin.zip
     ```
   - Click **File → Invalidate Caches / Restart**
   - Sync project

2. **Option B - Downgrade Java to 17:**
   - Go to **File → Settings → Build, Execution, Deployment → Build Tools → Gradle**
   - Set **Gradle JDK** to Java 17
   - If not available, download from: https://adoptium.net/temurin/releases/?version=17
   - Click Apply and sync project

3. **Quick Fix - Clear Gradle Cache:**
   - Close Android Studio
   - Delete folder: `C:\Users\kumat\.gradle\caches`
   - Reopen Android Studio
   - Let Gradle sync again

### 2. "OpenCV not found" or "Could not find OpenCV"

**Error Message:**
```
CMake Error at CMakeLists.txt:X (find_package):
  Could not find a package configuration file provided by "OpenCV"
```

**Solution:**
1. Verify OpenCV SDK is downloaded and extracted
2. Open `app/src/main/cpp/CMakeLists.txt`
3. Update the `OpenCV_DIR` path to match your OpenCV location:
   ```cmake
   set(OpenCV_DIR "C:/YOUR/ACTUAL/PATH/opencv-android-sdk/sdk/native/jni")
   ```
4. Use forward slashes (/) not backslashes (\)
5. Ensure path has no spaces
6. Click "Sync Project with Gradle Files"

### 2. "NDK not configured" or "NDK not found"

**Error Message:**
```
NDK is not configured
```

**Solution:**
1. Go to **File → Project Structure → SDK Location**
2. Check "Android NDK location" is set
3. If not set, browse to your NDK folder:
   - Usually: `C:\Users\YourName\AppData\Local\Android\Sdk\ndk\23.1.7779620`
4. Or install NDK via **Tools → SDK Manager → SDK Tools → NDK**

### 3. "CMake not found" or "CMake executable not found"

**Error Message:**
```
Gradle sync failed: CMake was not found
```

**Solution:**
1. Open **Tools → SDK Manager**
2. Go to **SDK Tools** tab
3. Check **CMake** and click Apply
4. Wait for installation
5. Restart Android Studio
6. Sync Gradle again

### 4. Gradle sync failed - "Unsupported class file major version"

**Error Message:**
```
Unsupported class file major version 61
```

**Solution:**
1. Go to **File → Settings → Build, Execution, Deployment → Build Tools → Gradle**
2. Set **Gradle JDK** to Java 17
3. If Java 17 not available, download from: https://adoptium.net/
4. Click Apply and OK
5. Sync project again

### 5. "Package 'com.example.edgedetection' not found"

**Error Message:**
```
Unresolved reference: databinding
```

**Solution:**
1. Check that `app/build.gradle` has:
   ```gradle
   android {
       namespace 'com.example.edgedetection'
       buildFeatures {
           viewBinding true
       }
   }
   ```
2. Click **Build → Clean Project**
3. Click **Build → Rebuild Project**

### 6. "Unable to resolve dependency for ':app@debug/compileClasspath'"

**Error Message:**
```
Could not resolve androidx.camera:camera-core:1.2.3
```

**Solution:**
1. Check internet connection
2. Open `build.gradle` (project level)
3. Ensure repositories are correctly defined:
   ```gradle
   repositories {
       google()
       mavenCentral()
   }
   ```
4. Click **File → Invalidate Caches / Restart**
5. Sync Gradle again

### 7. "More than one file was found with OS independent path 'lib/x86/libopencv_java4.so'"

**Error Message:**
```
More than one file was found with OS independent path
```

**Solution:**
Add to `app/build.gradle` inside `android` block:
```gradle
android {
    packagingOptions {
        pickFirst 'lib/x86/libopencv_java4.so'
        pickFirst 'lib/x86_64/libopencv_java4.so'
        pickFirst 'lib/armeabi-v7a/libopencv_java4.so'
        pickFirst 'lib/arm64-v8a/libopencv_java4.so'
    }
}
```

## Runtime Errors

### 8. App crashes immediately on launch

**Error in Logcat:**
```
java.lang.UnsatisfiedLinkError: dlopen failed: library "libedge_detection.so" not found
```

**Solution:**
1. Ensure native libraries are built successfully
2. Check **Build → Rebuild Project** completes without errors
3. Verify CMakeLists.txt has no errors
4. Clean and rebuild:
   ```
   Build → Clean Project
   Build → Rebuild Project
   ```

### 9. Camera permission denied

**Error:**
```
Camera permission denied
```

**Solution:**
1. Uninstall the app from device
2. Reinstall the app
3. When prompted, tap "Allow" for camera permission
4. Or go to Settings → Apps → Edge Detection Viewer → Permissions → Enable Camera

### 10. App shows black screen

**Possible Causes:**
- Camera not accessible
- OpenGL initialization failed
- JNI bridge issue

**Solution:**
1. Check Logcat for errors
2. Ensure camera permission granted
3. Try on a different device
4. Verify device supports OpenGL ES 2.0+

### 11. Low FPS (< 5 FPS)

**Solution:**
1. Test on a physical device (not emulator)
2. Build in Release mode instead of Debug
3. Reduce camera resolution in MainActivity.kt
4. Adjust edge detection parameters for faster processing

### 12. "Native method not found"

**Error in Logcat:**
```
java.lang.UnsatisfiedLinkError: No implementation found for int com.example.edgedetection.NativeWrapper.processFrame
```

**Solution:**
1. Verify JNI function signatures match exactly
2. Check native library is being loaded:
   ```kotlin
   companion object {
       init {
           System.loadLibrary("edge_detection")
       }
   }
   ```
3. Rebuild native code
4. Clean and rebuild entire project

## Gradle Errors

### 13. "Gradle version 7.x is required. Current version is 6.x"

**Solution:**
1. Open `gradle/wrapper/gradle-wrapper.properties`
2. Update to:
   ```properties
   distributionUrl=https\://services.gradle.org/distributions/gradle-8.0-bin.zip
   ```
3. Sync project

### 14. "Namespace not specified"

**Error Message:**
```
Namespace not specified. Specify a namespace in the module's build file
```

**Solution:**
Add to `app/build.gradle`:
```gradle
android {
    namespace 'com.example.edgedetection'
    // ... rest of config
}
```

### 15. "Build was configured to prefer settings repositories over project repositories"

**Solution:**
This is a warning, not an error. To fix:
1. Move all `repositories` blocks from `build.gradle` to `settings.gradle`
2. Or add to `settings.gradle`:
   ```gradle
   dependencyResolutionManagement {
       repositoriesMode.set(RepositoriesMode.PREFER_SETTINGS)
   }
   ```

## C++ / Native Errors

### 16. "use of undeclared identifier 'malloc'" or 'free'

**Error Message:**
```
use of undeclared identifier 'malloc' [x86_64] :86
use of undeclared identifier 'free' [x86_64] :89
```

**Cause:**
Missing C++ standard library headers in your C++ files.

**Solution:**
Add these includes to your .cpp files:
```cpp
#include <cstdlib>  // For malloc, free, calloc
#include <cstring>  // For string functions
```

**Files to update:**
- `app/src/main/cpp/gl_renderer.cpp`
- `app/src/main/cpp/edge_detector.cpp`

**Already Fixed:** These headers have been added to your files.

### 17. "opencv2/opencv.hpp: No such file or directory"

**Solution:**
1. Verify OpenCV_DIR path in CMakeLists.txt
2. Check that `${OpenCV_INCLUDE_DIRS}` is being included
3. Ensure OpenCV was found during CMake configuration
4. Check CMake output for OpenCV version message

### 17. "undefined reference to cv::Canny"

**Solution:**
1. Verify OpenCV libraries are linked:
   ```cmake
   target_link_libraries(edge_detection
                         ${OpenCV_LIBS}
                         ...)
   ```
2. Check OpenCV was found correctly during CMake
3. Rebuild project

## Web Viewer Errors

### 18. TypeScript compilation errors

**Solution:**
1. Navigate to `web` directory
2. Remove node_modules and package-lock.json
3. Run:
   ```
   npm install
   npm run build
   ```

### 19. "Cannot find module 'typescript'"

**Solution:**
```
cd web
npm install typescript --save-dev
npm run build
```

## General Tips

1. **Always check Logcat** for detailed error messages
2. **Clean and Rebuild** often solves many issues
3. **Invalidate Caches** can fix weird IDE issues
4. **Update Android Studio** to latest stable version
5. **Check file paths** for spaces or special characters
6. **Use forward slashes** in CMake paths, even on Windows
7. **Test on physical device** for better results

## Still Having Issues?

1. Check the full error message in:
   - Build Output (bottom panel)
   - Logcat (for runtime errors)
   - Event Log (bottom right corner)

2. Common investigation steps:
   - Clean project
   - Invalidate caches and restart
   - Check all paths are correct
   - Verify all prerequisites are installed
   - Try on a different device

3. Verify your setup:
   - Android Studio version
   - JDK version (should be 17)
   - NDK version (21+)
   - CMake version (3.22.1+)
   - OpenCV version (4.5.0+)
