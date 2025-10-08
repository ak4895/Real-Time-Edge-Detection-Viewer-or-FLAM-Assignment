# Project Build Checklist

Use this checklist to ensure your project is properly configured before building.

## ✅ Pre-Build Checklist

### Environment Setup
- [ ] Android Studio Arctic Fox (2020.3.1) or newer installed
- [ ] JDK 17 installed and configured
- [ ] Android SDK 33 or newer installed
- [ ] Android NDK r21+ installed via SDK Manager
- [ ] CMake 3.22.1+ installed via SDK Manager
- [ ] OpenCV 4.5.0+ for Android downloaded and extracted

### Project Configuration Files

#### Essential Files Present
- [ ] `build.gradle` (project level)
- [ ] `settings.gradle`
- [ ] `gradle.properties`
- [ ] `gradle/wrapper/gradle-wrapper.properties`
- [ ] `app/build.gradle`
- [ ] `app/proguard-rules.pro`
- [ ] `app/src/main/AndroidManifest.xml`
- [ ] `app/src/main/cpp/CMakeLists.txt`

#### Kotlin Source Files
- [ ] `app/src/main/java/com/example/edgedetection/MainActivity.kt`
- [ ] `app/src/main/java/com/example/edgedetection/GLRenderer.kt`
- [ ] `app/src/main/java/com/example/edgedetection/NativeWrapper.kt`

#### C++ Source Files
- [ ] `app/src/main/cpp/edge_detector.cpp`
- [ ] `app/src/main/cpp/gl_renderer.cpp`

#### Resource Files
- [ ] `app/src/main/res/layout/activity_main.xml`
- [ ] `app/src/main/res/values/strings.xml`
- [ ] `app/src/main/res/values/themes.xml`
- [ ] `app/src/main/res/values/colors.xml`
- [ ] Launcher icons in mipmap folders

### Critical Configurations

#### CMakeLists.txt
- [ ] OpenCV_DIR path is set correctly
- [ ] Path uses forward slashes (/)
- [ ] Path has no spaces
- [ ] Path points to `.../opencv-android-sdk/sdk/native/jni`
- [ ] Both .cpp files are listed in add_library()

Example:
```cmake
set(OpenCV_DIR "C:/Users/YourName/opencv-android-sdk/sdk/native/jni")
```

#### build.gradle (app level)
- [ ] namespace is set to 'com.example.edgedetection'
- [ ] compileSdk is 33 or higher
- [ ] minSdk is 21 or higher
- [ ] viewBinding is enabled
- [ ] CameraX dependencies are included
- [ ] NDK abiFilters are set

#### settings.gradle
- [ ] Includes ':app'
- [ ] Repositories configured (google(), mavenCentral())

#### gradle.properties
- [ ] android.useAndroidX=true
- [ ] android.enableJetifier=true

#### AndroidManifest.xml
- [ ] Camera permission declared
- [ ] Camera hardware feature declared
- [ ] MainActivity is exported and has MAIN/LAUNCHER intent filter

### OpenCV Integration

- [ ] OpenCV SDK is downloaded
- [ ] OpenCV SDK is extracted to location WITHOUT spaces
- [ ] Full path to jni folder is noted
- [ ] Path is updated in CMakeLists.txt
- [ ] Path format is correct (forward slashes)

### Gradle Configuration

- [ ] gradle-wrapper.properties uses Gradle 8.0
- [ ] Internet connection available for dependency download
- [ ] No proxy issues blocking downloads

## 🔧 Build Process Checklist

### Initial Build Steps
1. [ ] Open project in Android Studio
2. [ ] Wait for Gradle sync to complete
3. [ ] Check "Build" tab for any errors
4. [ ] If errors, consult ERROR_FIXES.md

### Clean Build
1. [ ] Build → Clean Project
2. [ ] Wait for clean to complete
3. [ ] Build → Rebuild Project
4. [ ] Check for successful build message

### Verify Native Build
1. [ ] Check Build Output for CMake messages
2. [ ] Verify "Build edge_detection" appears
3. [ ] Check for "OpenCV version: X.X.X" message
4. [ ] No errors in CMake configuration

### APK Generation
1. [ ] APK file created in `app/build/outputs/apk/debug/`
2. [ ] APK size is reasonable (10-50 MB depending on OpenCV)
3. [ ] .so libraries included in APK

## 📱 Device Setup Checklist

### Device Preparation
- [ ] Developer options enabled
- [ ] USB debugging enabled
- [ ] Device connected via USB
- [ ] Device recognized in Android Studio (appears in device dropdown)
- [ ] Device running Android 5.0 (API 21) or higher

### Installation
1. [ ] Click Run button (green triangle)
2. [ ] Select target device
3. [ ] Wait for installation
4. [ ] App launches automatically

### Permissions
- [ ] Camera permission prompt appears
- [ ] Camera permission granted
- [ ] App shows camera feed

## ✔️ Functionality Verification

### App Launch
- [ ] App launches without crashing
- [ ] No black screen
- [ ] GLSurfaceView is visible

### Camera Feed
- [ ] Camera preview appears
- [ ] Preview is not blank
- [ ] Preview is not frozen

### Edge Detection
- [ ] Edges are visible in the camera feed
- [ ] Edge detection updates in real-time
- [ ] Processing is smooth

### Performance
- [ ] FPS counter is visible
- [ ] FPS is 10 or higher
- [ ] Resolution is displayed correctly
- [ ] No significant lag

### No Crashes
- [ ] App doesn't crash on startup
- [ ] App doesn't crash during use
- [ ] App doesn't crash on rotation (if supported)
- [ ] App doesn't crash when paused/resumed

## 🐛 Common Issues Quick Check

If build fails, check:
- [ ] OpenCV path in CMakeLists.txt
- [ ] NDK is installed
- [ ] CMake is installed
- [ ] Internet connection for Gradle dependencies
- [ ] No spaces in project path
- [ ] JDK 17 is being used

If app crashes, check:
- [ ] Camera permission granted
- [ ] Native libraries built successfully
- [ ] Logcat for error messages
- [ ] Device supports OpenGL ES 2.0+

If performance is poor:
- [ ] Testing on physical device (not emulator)
- [ ] Build type is Debug (Release is faster)
- [ ] Edge detection parameters are reasonable

## 📋 Final Verification

Before considering the project complete:
- [ ] Project builds without errors
- [ ] Project builds without warnings (or only expected warnings)
- [ ] App installs on device
- [ ] App runs smoothly
- [ ] All features work as expected
- [ ] FPS is acceptable (10-15+)
- [ ] No crashes during normal use
- [ ] Code is clean and well-commented

## 🎯 Success Criteria

Your project is ready when:
✅ Clean build completes successfully  
✅ App installs without errors  
✅ Camera feed shows edge detection in real-time  
✅ FPS counter shows 10+ FPS  
✅ No crashes during 5 minutes of use  
✅ App responds to pause/resume correctly  

## Next Steps

Once all checks pass:
1. Test on multiple devices
2. Test different lighting conditions
3. Test edge detection on various objects
4. Optimize performance if needed
5. Build release APK for distribution

## Need Help?

If any checklist item fails:
1. See ERROR_FIXES.md for specific solutions
2. See INSTALLATION_GUIDE.md for setup details
3. Check Logcat for detailed error messages
4. Verify all paths and configurations
