# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.

# Keep native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep edge detection classes
-keep class com.example.edgedetection.** { *; }

# OpenCV
-keep class org.opencv.** { *; }
