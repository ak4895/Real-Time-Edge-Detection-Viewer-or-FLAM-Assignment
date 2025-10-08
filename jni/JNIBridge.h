#pragma once

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <GLES2/gl2.h>

/**
 * JNIBridge - A utility class for JNI operations
 */
class JNIBridge {
public:
    /**
     * Convert a Java byte array to an OpenCV Mat
     * 
     * @param env The JNI environment
     * @param byteArray The Java byte array
     * @param width The width of the image
     * @param height The height of the image
     * @param channels The number of channels (1 for grayscale, 3 for RGB, 4 for RGBA)
     * @return An OpenCV Mat containing the image data
     */
    static cv::Mat byteArrayToMat(JNIEnv* env, jbyteArray byteArray, int width, int height, int channels = 4) {
        jbyte* data = env->GetByteArrayElements(byteArray, 0);
        cv::Mat mat;
        
        if (channels == 1) {
            mat = cv::Mat(height, width, CV_8UC1, data);
        } else if (channels == 3) {
            mat = cv::Mat(height, width, CV_8UC3, data);
        } else {
            mat = cv::Mat(height, width, CV_8UC4, data);
        }
        
        // Create a deep copy so we can release the Java byte array
        cv::Mat result = mat.clone();
        env->ReleaseByteArrayElements(byteArray, data, 0);
        
        return result;
    }
    
    /**
     * Convert YUV_NV21 format to RGBA OpenCV Mat
     * 
     * @param env The JNI environment
     * @param yuv The Java byte array containing YUV_NV21 data
     * @param width The width of the image
     * @param height The height of the image
     * @return An OpenCV Mat in RGBA format
     */
    static cv::Mat yuv2Rgba(JNIEnv* env, jbyteArray yuv, int width, int height) {
        jbyte* yuvData = env->GetByteArrayElements(yuv, 0);
        
        // YUV_NV21 format has Y plane followed by interleaved VU plane
        cv::Mat yuvMat(height + height/2, width, CV_8UC1, yuvData);
        cv::Mat rgbaMat;
        cv::cvtColor(yuvMat, rgbaMat, cv::COLOR_YUV2RGBA_NV21);
        
        // Release Java byte array
        env->ReleaseByteArrayElements(yuv, yuvData, 0);
        
        return rgbaMat;
    }
    
    /**
     * Upload an OpenCV Mat directly to an OpenGL texture
     * 
     * @param mat The OpenCV Mat containing RGBA data
     * @param textureId The OpenGL texture ID
     * @return true if successful, false otherwise
     */
    static bool matToTexture(const cv::Mat& mat, GLuint textureId) {
        if (mat.empty() || textureId == 0) {
            return false;
        }
        
        // Ensure mat is in RGBA format
        cv::Mat rgbaMat;
        if (mat.channels() == 4) {
            rgbaMat = mat;
        } else if (mat.channels() == 3) {
            cv::cvtColor(mat, rgbaMat, cv::COLOR_RGB2RGBA);
        } else if (mat.channels() == 1) {
            cv::cvtColor(mat, rgbaMat, cv::COLOR_GRAY2RGBA);
        } else {
            return false;
        }
        
        // Bind the texture and upload the data
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaMat.cols, rgbaMat.rows, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgbaMat.data);
        
        return true;
    }
};
