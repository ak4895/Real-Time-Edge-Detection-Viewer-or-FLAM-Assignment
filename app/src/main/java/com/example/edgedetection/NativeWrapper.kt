package com.example.edgedetection

/**
 * Wrapper class for JNI native methods
 */
class NativeWrapper {
    // Load the native library
    companion object {
        init {
            System.loadLibrary("edgedetection")
        }
    }

    /**
     * Initialize native resources
     */
    external fun initNative()

    /**
     * Process a camera frame using OpenCV edge detection
     *
     * @param data The YUV image data from camera
     * @param width The width of the image
     * @param height The height of the image
     * @param rotation The rotation of the image
     * @return The texture ID for rendering the processed frame
     */
    external fun processFrame(data: ByteArray, width: Int, height: Int, rotation: Int): Int

    /**
     * Create an OpenGL texture for rendering processed frames
     *
     * @return The generated texture ID
     */
    external fun createTexture(): Int

    /**
     * Update the edge detection parameters
     *
     * @param lowThreshold The low threshold for Canny edge detection
     * @param ratio The ratio of high threshold to low threshold
     * @param kernelSize The kernel size for Canny edge detection
     */
    external fun updateParameters(lowThreshold: Int, ratio: Int, kernelSize: Int)

    /**
     * Clean up native resources
     */
    external fun cleanupNative()

    /**
     * Initialize OpenGL resources
     */
    external fun initGL()

    /**
     * Render a frame
     *
     * @param textureId The texture to render
     */
    external fun drawFrame(textureId: Int)

    /**
     * Clean up OpenGL resources
     */
    external fun cleanupGL()
}
