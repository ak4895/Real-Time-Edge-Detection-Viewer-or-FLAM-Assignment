#include <jni.h>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdlib>
#include <cstring>

#define LOG_TAG "EdgeDetector"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/**
 * Edge Detector class that applies Canny edge detection to camera frames using OpenCV
 */
class EdgeDetector {
private:
    // OpenCV edge detection parameters
    int lowThreshold = 50;
    int ratio = 3;
    int kernelSize = 3;
    cv::Mat grayMat;
    cv::Mat edgeMat;

public:
    EdgeDetector() {
        LOGI("EdgeDetector created");
    }

    ~EdgeDetector() {
        LOGI("EdgeDetector destroyed");
    }

    // Processes the input frame with Canny edge detection
    cv::Mat processFrame(const cv::Mat& inputFrame) {
        // Convert input frame to grayscale
        cv::cvtColor(inputFrame, grayMat, cv::COLOR_RGBA2GRAY);
        
        // Apply Gaussian blur to reduce noise
        cv::GaussianBlur(grayMat, grayMat, cv::Size(5, 5), 1.5, 1.5);
        
        // Apply Canny edge detection
        cv::Canny(grayMat, edgeMat, lowThreshold, lowThreshold * ratio, kernelSize);
        
        // Convert back to RGBA format for OpenGL rendering
        cv::cvtColor(edgeMat, edgeMat, cv::COLOR_GRAY2RGBA);
        
        return edgeMat;
    }
    
    // Update the edge detection parameters
    void updateParameters(int low, int highRatio, int kernel) {
        lowThreshold = low;
        ratio = highRatio;
        kernelSize = kernel;
    }
};

// Global pointer to our edge detector
EdgeDetector* gEdgeDetector = nullptr;

// Texture ID for OpenGL
GLuint gTextureId = 0;

extern "C" {

// Initialize native resources
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_initNative(JNIEnv* env, jobject thiz) {
    if (!gEdgeDetector) {
        gEdgeDetector = new EdgeDetector();
        LOGI("Native resources initialized");
    }
}

// Process a frame from the camera
JNIEXPORT jint JNICALL
Java_com_example_edgedetection_NativeWrapper_processFrame(JNIEnv* env, jobject thiz,
                                                      jbyteArray input, jint width, jint height,
                                                      jint rotation) {
    if (!gEdgeDetector) {
        LOGE("Edge detector not initialized");
        return -1;
    }
    
    // Access the input byte array
    jbyte* inputBuffer = env->GetByteArrayElements(input, NULL);
    
    if (!inputBuffer) {
        LOGE("Failed to get byte array elements");
        return -1;
    }
    
    // Create an OpenCV Mat from the input byte array
    cv::Mat inputMat(height + height/2, width, CV_8UC1, inputBuffer);
    
    // Convert YUV to RGBA
    cv::Mat rgbaMat;
    cv::cvtColor(inputMat, rgbaMat, cv::COLOR_YUV2RGBA_NV21);
    
    // Process the frame using our edge detector
    cv::Mat processedFrame = gEdgeDetector->processFrame(rgbaMat);
    
    // Release the byte array
    env->ReleaseByteArrayElements(input, inputBuffer, 0);
    
    // Update the OpenGL texture with the processed frame
    glBindTexture(GL_TEXTURE_2D, gTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, processedFrame.cols, processedFrame.rows, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, processedFrame.data);
    
    return gTextureId;
}

// Create an OpenGL texture to hold our processed frame
JNIEXPORT jint JNICALL
Java_com_example_edgedetection_NativeWrapper_createTexture(JNIEnv* env, jobject thiz) {
    // Generate a new texture ID
    glGenTextures(1, &gTextureId);
    
    // Configure texture parameters
    glBindTexture(GL_TEXTURE_2D, gTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    LOGI("Created texture with ID: %d", gTextureId);
    
    return gTextureId;
}

// Update edge detector parameters
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_updateParameters(JNIEnv* env, jobject thiz,
                                                         jint lowThreshold, jint ratio, 
                                                         jint kernelSize) {
    if (gEdgeDetector) {
        gEdgeDetector->updateParameters(lowThreshold, ratio, kernelSize);
    }
}

// Clean up native resources
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_cleanupNative(JNIEnv* env, jobject thiz) {
    if (gEdgeDetector) {
        delete gEdgeDetector;
        gEdgeDetector = nullptr;
    }
    
    if (gTextureId != 0) {
        glDeleteTextures(1, &gTextureId);
        gTextureId = 0;
    }
    
    LOGI("Native resources cleaned up");
}

}
