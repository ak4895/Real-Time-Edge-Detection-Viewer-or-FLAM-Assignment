package com.example.edgedetection

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * GLRenderer that manages the OpenGL surface for rendering the processed camera frames
 */
class GLRenderer : GLSurfaceView.Renderer {
    // OpenGL texture ID for the processed frame
    private var textureId: Int = 0
    private val nativeWrapper = NativeWrapper()

    /**
     * Called when the surface is created or recreated
     */
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        // Initialize OpenGL ES
        nativeWrapper.initGL()

        // Create a texture for the processed frame
        textureId = nativeWrapper.createTexture()
    }

    /**
     * Called when the surface changes size
     */
    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        // Set the viewport
        gl?.glViewport(0, 0, width, height)
    }

    /**
     * Called to draw the current frame
     */
    override fun onDrawFrame(gl: GL10?) {
        // Draw the frame using the current texture
        nativeWrapper.drawFrame(textureId)
    }

    /**
     * Update the texture ID used for rendering
     */
    fun updateTextureId(id: Int) {
        if (id > 0) {
            textureId = id
        }
    }

    /**
     * Clean up resources when the renderer is no longer needed
     */
    fun release() {
        nativeWrapper.cleanupGL()
    }
}
