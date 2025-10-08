# QUICK START GUIDE - 5 Minutes to Running App

## ⚠️ FIRST TIME USERS - DO THIS FIRST!

If you see **"Unsupported class file major version 65"** error:
1. Close Android Studio
2. Delete: `C:\Users\kumat\.gradle\caches` folder
3. Reopen Android Studio
4. Continue with steps below

**See [DO_THIS_NOW.md](DO_THIS_NOW.md) for detailed fix.**

---

## Prerequisites (Do Once)
1. ✅ Android Studio installed
2. ✅ OpenCV for Android downloaded
3. ✅ Android device with USB debugging enabled

## 3-Step Build Process

### STEP 1: Configure OpenCV (30 seconds)
1. Open `app/src/main/cpp/CMakeLists.txt`
2. Find line 10:
   ```cmake
   set(OpenCV_DIR "C:/Users/kumat/Downloads/opencv-4.12.0-android-sdk/OpenCV-android-sdk/sdk/native/jni")
   ```
3. Change the path to YOUR OpenCV location (use forward slashes `/`)
4. Save the file

### STEP 2: Build Project (2-3 minutes)
1. Open Android Studio
2. File → Open → Select `FLAM ASSIGNMENT` folder
3. Wait for Gradle sync (status bar at bottom)
4. Click: **Build → Rebuild Project**
5. Wait for "BUILD SUCCESSFUL" message

### STEP 3: Run on Device (1 minute)
1. Connect Android phone via USB
2. If prompted, allow USB debugging on phone
3. Select your device from dropdown (top toolbar)
4. Click green **Run** button (▶)
5. When app launches, allow camera permission
6. Done! Edge detection should be working

## Expected Result
- Camera feed with edge detection overlay
- FPS counter in top-left corner
- Smooth real-time processing

## If Build Fails
See the line with red text in Build Output (bottom panel), then:

**"OpenCV not found"** → Check CMakeLists.txt path (Step 1)  
**"NDK not configured"** → Install NDK via Tools → SDK Manager → SDK Tools  
**"CMake not found"** → Install CMake via Tools → SDK Manager → SDK Tools  
**Other errors** → See `ERROR_FIXES.md` for detailed solutions  

## If App Crashes
1. Check Logcat (bottom panel) for error
2. Verify camera permission was granted
3. Try uninstall + reinstall
4. See `ERROR_FIXES.md` for specific errors

## Verification
✅ App launches  
✅ Camera feed shows  
✅ Edges are detected  
✅ FPS shows 10+  
✅ No crashes  

## That's It!
For detailed instructions, see `INSTALLATION_GUIDE.md`  
For troubleshooting, see `ERROR_FIXES.md`  
For verification, see `PROJECT_CHECKLIST.md`
