#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/**
 * Texture - Helper class for creating and managing OpenGL textures
 */
class Texture {
private:
    GLuint mTextureId;
    int mWidth;
    int mHeight;
    
public:
    /**
     * Constructor
     */
    Texture() : mTextureId(0), mWidth(0), mHeight(0) {}
    
    /**
     * Destructor
     */
    ~Texture() {
        if (mTextureId != 0) {
            glDeleteTextures(1, &mTextureId);
        }
    }
    
    /**
     * Create a new texture with the specified dimensions
     * 
     * @param width The width of the texture
     * @param height The height of the texture
     * @return true if creation is successful, false otherwise
     */
    bool create(int width, int height) {
        // Delete existing texture if any
        if (mTextureId != 0) {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
        }
        
        // Generate a new texture
        glGenTextures(1, &mTextureId);
        if (mTextureId == 0) {
            return false;
        }
        
        mWidth = width;
        mHeight = height;
        
        // Configure texture parameters
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // Allocate empty texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        
        return true;
    }
    
    /**
     * Update texture data from buffer
     * 
     * @param data Pointer to the texture data (RGBA)
     * @param width Width of the data
     * @param height Height of the data
     */
    void update(const void* data, int width, int height) {
        if (mTextureId == 0 || data == nullptr) {
            return;
        }
        
        // If dimensions have changed, recreate the texture
        if (width != mWidth || height != mHeight) {
            create(width, height);
        }
        
        // Update the texture data
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    
    /**
     * Get the texture ID
     * 
     * @return The OpenGL texture ID
     */
    GLuint getId() const {
        return mTextureId;
    }
    
    /**
     * Get the texture width
     * 
     * @return The texture width
     */
    int getWidth() const {
        return mWidth;
    }
    
    /**
     * Get the texture height
     * 
     * @return The texture height
     */
    int getHeight() const {
        return mHeight;
    }
    
    /**
     * Bind the texture to the current texture unit
     */
    void bind() const {
        glBindTexture(GL_TEXTURE_2D, mTextureId);
    }
};
