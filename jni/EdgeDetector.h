#pragma once

#include <opencv2/opencv.hpp>
#include <android/log.h>

#define LOG_TAG "EdgeDetector"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/**
 * EdgeDetector - A class for applying edge detection to images using OpenCV
 */
class EdgeDetector {
private:
    // Edge detection parameters
    int mLowThreshold;
    int mRatio;
    int mKernelSize;
    
    // Intermediate OpenCV matrices
    cv::Mat mGrayMat;
    cv::Mat mBlurMat;
    cv::Mat mEdgeMat;
    cv::Mat mOutputMat;

public:
    /**
     * Constructor with default parameters
     */
    EdgeDetector() : 
        mLowThreshold(50),
        mRatio(3),
        mKernelSize(3) {
        LOGI("EdgeDetector created with default parameters");
    }
    
    /**
     * Constructor with custom parameters
     * 
     * @param lowThreshold The low threshold for Canny edge detection
     * @param ratio The ratio of high threshold to low threshold
     * @param kernelSize The kernel size for Canny edge detection
     */
    EdgeDetector(int lowThreshold, int ratio, int kernelSize) : 
        mLowThreshold(lowThreshold),
        mRatio(ratio),
        mKernelSize(kernelSize) {
        LOGI("EdgeDetector created with custom parameters: low=%d, ratio=%d, kernel=%d",
             lowThreshold, ratio, kernelSize);
    }
    
    /**
     * Destructor
     */
    ~EdgeDetector() {
        LOGI("EdgeDetector destroyed");
    }
    
    /**
     * Process an image using Canny edge detection
     * 
     * @param input The input image (RGBA format)
     * @return The processed image with edges highlighted (RGBA format)
     */
    cv::Mat process(const cv::Mat& input) {
        // Convert to grayscale
        cv::cvtColor(input, mGrayMat, cv::COLOR_RGBA2GRAY);
        
        // Apply Gaussian blur to reduce noise
        cv::GaussianBlur(mGrayMat, mBlurMat, cv::Size(5, 5), 1.5, 1.5);
        
        // Apply Canny edge detection
        cv::Canny(mBlurMat, mEdgeMat, mLowThreshold, mLowThreshold * mRatio, mKernelSize);
        
        // Convert back to RGBA for OpenGL rendering
        cv::cvtColor(mEdgeMat, mOutputMat, cv::COLOR_GRAY2RGBA);
        
        return mOutputMat;
    }
    
    /**
     * Update the edge detection parameters
     * 
     * @param lowThreshold The low threshold for Canny edge detection
     * @param ratio The ratio of high threshold to low threshold
     * @param kernelSize The kernel size for Canny edge detection
     */
    void updateParameters(int lowThreshold, int ratio, int kernelSize) {
        mLowThreshold = lowThreshold;
        mRatio = ratio;
        mKernelSize = kernelSize;
        
        LOGI("Parameters updated: low=%d, ratio=%d, kernel=%d", 
             mLowThreshold, mRatio, mKernelSize);
    }
};
