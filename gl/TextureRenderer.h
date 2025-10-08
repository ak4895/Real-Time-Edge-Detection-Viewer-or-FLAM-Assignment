#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "ShaderUtil.h"

/**
 * TextureRenderer - Class for rendering textures as quads
 */
class TextureRenderer {
private:
    // Shader program and handles
    GLuint mProgram;
    GLint mPositionHandle;
    GLint mTexCoordHandle;
    GLint mTextureUniform;
    
    // VBO handles
    GLuint mPositionVBO;
    GLuint mTexCoordVBO;
    
    // Shader source
    const char* mVertexShader = 
        "attribute vec4 aPosition;\n"
        "attribute vec2 aTexCoord;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "  gl_Position = aPosition;\n"
        "  vTexCoord = aTexCoord;\n"
        "}\n";
    
    const char* mFragmentShader = 
        "precision mediump float;\n"
        "varying vec2 vTexCoord;\n"
        "uniform sampler2D uTexture;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(uTexture, vTexCoord);\n"
        "}\n";
    
    // Full screen quad vertex data
    const GLfloat mVertices[8] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };
    
    // Texture coordinate data - flipped vertically for OpenGL
    const GLfloat mTexCoords[8] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };
    
public:
    /**
     * Constructor
     */
    TextureRenderer() : mProgram(0), mPositionHandle(-1), mTexCoordHandle(-1), 
                       mTextureUniform(-1), mPositionVBO(0), mTexCoordVBO(0) {}
    
    /**
     * Destructor
     */
    ~TextureRenderer() {
        cleanup();
    }
    
    /**
     * Initialize the renderer
     * 
     * @return true if initialization is successful, false otherwise
     */
    bool init() {
        // Create the shader program
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        if (mProgram == 0) {
            return false;
        }
        
        // Get handles to shader attributes and uniforms
        mPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        mTexCoordHandle = glGetAttribLocation(mProgram, "aTexCoord");
        mTextureUniform = glGetUniformLocation(mProgram, "uTexture");
        
        // Generate VBOs
        glGenBuffers(1, &mPositionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_STATIC_DRAW);
        
        glGenBuffers(1, &mTexCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoords), mTexCoords, GL_STATIC_DRAW);
        
        return true;
    }
    
    /**
     * Render a texture to the current framebuffer
     * 
     * @param textureId The OpenGL texture ID to render
     */
    void renderTexture(GLuint textureId) {
        // Use the shader program
        glUseProgram(mProgram);
        
        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(mTextureUniform, 0);
        
        // Bind position VBO and enable attribute
        glBindBuffer(GL_ARRAY_BUFFER, mPositionVBO);
        glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(mPositionHandle);
        
        // Bind texcoord VBO and enable attribute
        glBindBuffer(GL_ARRAY_BUFFER, mTexCoordVBO);
        glVertexAttribPointer(mTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(mTexCoordHandle);
        
        // Draw the quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        // Disable vertex attributes
        glDisableVertexAttribArray(mPositionHandle);
        glDisableVertexAttribArray(mTexCoordHandle);
    }
    
    /**
     * Clean up OpenGL resources
     */
    void cleanup() {
        if (mProgram != 0) {
            glDeleteProgram(mProgram);
            mProgram = 0;
        }
        
        if (mPositionVBO != 0) {
            glDeleteBuffers(1, &mPositionVBO);
            mPositionVBO = 0;
        }
        
        if (mTexCoordVBO != 0) {
            glDeleteBuffers(1, &mTexCoordVBO);
            mTexCoordVBO = 0;
        }
    }
};
