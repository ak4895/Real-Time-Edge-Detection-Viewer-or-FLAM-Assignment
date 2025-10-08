# Android Studio Installation and Build Guide

## Prerequisites Checklist

- [ ] Android Studio Arctic Fox or newer installed
- [ ] JDK 17 installed
- [ ] Android SDK 33 or newer
- [ ] Android NDK r21 or newer
- [ ] CMake 3.22.1 or newer
- [ ] OpenCV 4.5.0+ for Android downloaded

## Step-by-Step Setup

### 1. Install Android Studio Components

1. Open Android Studio
2. Go to **Tools → SDK Manager**
3. In **SDK Platforms** tab, install:
   - Android 13.0 (Tiramisu) API Level 33
4. In **SDK Tools** tab, install:
   - Android SDK Build-Tools
   - NDK (Side by side) - version 23.1.7779620 or newer
   - CMake - version 3.22.1
   - Android SDK Platform-Tools
   - Android Emulator

### 2. Download and Setup OpenCV

1. Download OpenCV Android SDK from: https://opencv.org/releases/
   - Download version 4.5.0 or newer
   - File name: `opencv-X.X.X-android-sdk.zip`

2. Extract the ZIP file to a location WITHOUT SPACES in the path:
   - Good: `C:\opencv-android\`
   - Good: `C:\Users\YourName\Downloads\opencv-android-sdk\`
   - Bad: `C:\Program Files\opencv\` (has space)

3. Note the full path to the `jni` folder inside the extracted SDK
   - Example: `C:/Users/kumat/Downloads/opencv-4.12.0-android-sdk/OpenCV-android-sdk/sdk/native/jni`

### 3. Configure the Project

1. **Update CMakeLists.txt**:
   - Open `app/src/main/cpp/CMakeLists.txt`
   - Find the line with `set(OpenCV_DIR ...)`
   - Update it to YOUR OpenCV path (use forward slashes):
   ```cmake
   set(OpenCV_DIR "C:/YOUR/PATH/TO/opencv-android-sdk/sdk/native/jni")
   ```

2. **Verify gradle-wrapper.properties**:
   - Open `gradle/wrapper/gradle-wrapper.properties`
   - Ensure it has:
   ```properties
   distributionUrl=https\://services.gradle.org/distributions/gradle-8.0-bin.zip
   ```

### 4. Open Project in Android Studio

1. Launch Android Studio
2. Select **File → Open**
3. Navigate to the `FLAM ASSIGNMENT` folder
4. Click **OK**
5. Wait for Gradle sync to complete (this may take several minutes)

### 5. Resolve Common Sync Issues

**If Gradle sync fails:**

1. **Check OpenCV Path**: Verify the path in CMakeLists.txt is correct
2. **Update Gradle**: File → Project Structure → Project → Gradle version: 8.0
3. **Invalidate Caches**: File → Invalidate Caches / Restart
4. **Check NDK**: File → Project Structure → SDK Location → verify NDK location

**If CMake fails:**

1. Open Android Studio Terminal and run:
   ```
   gradlew clean
   ```
2. Then rebuild the project

### 6. Build the Project

1. Click **Build → Clean Project**
2. Wait for cleaning to complete
3. Click **Build → Rebuild Project**
4. Wait for build to complete (first build takes longer)

### 7. Run on Device

1. **Enable Developer Options on Android device**:
   - Go to Settings → About Phone
   - Tap "Build Number" 7 times
   - Go back → System → Developer Options
   - Enable "USB Debugging"

2. **Connect device via USB**

3. **In Android Studio**:
   - Select your device from device dropdown
   - Click Run button (green triangle)
   - Allow camera permissions when prompted

### 8. Troubleshooting

**Error: "OpenCV not found"**
- Solution: Double-check the OpenCV_DIR path in CMakeLists.txt
- Make sure the path uses forward slashes (/)
- Verify the path points to the `jni` folder

**Error: "NDK not configured"**
- Solution: Go to File → Project Structure → SDK Location
- Set Android NDK location to your NDK path

**Error: "Unable to find CMake"**
- Solution: Install CMake via SDK Manager (Tools → SDK Manager → SDK Tools → CMake)

**App crashes on launch**
- Check Logcat in Android Studio for error messages
- Verify camera permissions are granted
- Ensure device supports camera

**Poor Performance**
- Use a physical device (emulators are slower)
- Build in Release mode for better performance
- Check if edge detection parameters are too intensive

### 9. Verify Build Success

After successful build, you should see:
- No red errors in the build output
- APK generated in `app/build/outputs/apk/`
- App installs and runs on device
- Camera view displays with edge detection
- FPS counter shows 10-15+ FPS

### 10. Building for Release

For production builds:

1. Generate signing key:
   ```
   keytool -genkey -v -keystore my-release-key.keystore -alias alias_name -keyalg RSA -keysize 2048 -validity 10000
   ```

2. Update app/build.gradle with signing config

3. Build release APK:
   - Build → Generate Signed Bundle / APK
   - Select APK
   - Choose your keystore
   - Build

## Additional Resources

- Android Studio: https://developer.android.com/studio
- OpenCV: https://opencv.org/
- CameraX: https://developer.android.com/training/camerax
- NDK: https://developer.android.com/ndk
