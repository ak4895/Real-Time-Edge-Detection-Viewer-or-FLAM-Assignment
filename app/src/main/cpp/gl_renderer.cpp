#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <cstdlib>
#include <cstring>

#define LOG_TAG "GLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Shader source code
static const char gVertexShader[] = 
    "attribute vec4 aPosition;\n"
    "attribute vec2 aTexCoord;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "  gl_Position = aPosition;\n"
    "  vTexCoord = aTexCoord;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec2 vTexCoord;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(uTexture, vTexCoord);\n"
    "}\n";

// Program and shader handles
static GLuint gProgram = 0;
static GLuint gVShader = 0;
static GLuint gFShader = 0;

// Attribute handles
static GLint gPositionHandle = -1;
static GLint gTexCoordHandle = -1;

// Uniform handles
static GLint gTextureUniform = -1;

// VBO handles
static GLuint gPositionVBO = 0;
static GLuint gTexCoordVBO = 0;

// Full screen quad vertex data
static const GLfloat gVertices[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

// Texture coordinate data - flipped vertically for OpenGL
static const GLfloat gTexCoords[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

/**
 * Compiles a shader of the given type from source
 */
static GLuint loadShader(GLenum type, const char* shaderSource) {
    // Create the shader object
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        LOGE("Error creating shader");
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSource, NULL);
    
    // Compile the shader
    glCompileShader(shader);
    
    // Check compilation status
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        
        if (infoLen) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGE("Error compiling shader:\n%s", infoLog);
            free(infoLog);
        }
        
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

extern "C" {

// Initialize the OpenGL renderer
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_initGL(JNIEnv* env, jobject thiz) {
    // Load the vertex and fragment shaders
    gVShader = loadShader(GL_VERTEX_SHADER, gVertexShader);
    gFShader = loadShader(GL_FRAGMENT_SHADER, gFragmentShader);
    
    // Create the program and attach the shaders
    gProgram = glCreateProgram();
    glAttachShader(gProgram, gVShader);
    glAttachShader(gProgram, gFShader);
    
    // Link the program
    glLinkProgram(gProgram);
    
    // Check link status
    GLint linked = 0;
    glGetProgramiv(gProgram, GL_LINK_STATUS, &linked);
    
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(gProgram, GL_INFO_LOG_LENGTH, &infoLen);
        
        if (infoLen) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(gProgram, infoLen, NULL, infoLog);
            LOGE("Error linking program:\n%s", infoLog);
            free(infoLog);
        }
        
        glDeleteProgram(gProgram);
        return;
    }
    
    // Get handle to vertex shader attributes
    gPositionHandle = glGetAttribLocation(gProgram, "aPosition");
    gTexCoordHandle = glGetAttribLocation(gProgram, "aTexCoord");
    
    // Get handle to fragment shader uniforms
    gTextureUniform = glGetUniformLocation(gProgram, "uTexture");
    
    // Generate VBOs
    glGenBuffers(1, &gPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), gVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &gTexCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTexCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gTexCoords), gTexCoords, GL_STATIC_DRAW);
    
    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Disable depth test - we're rendering a 2D texture
    glDisable(GL_DEPTH_TEST);
    
    LOGI("GL initialization complete");
}

// Render the processed frame texture
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_drawFrame(JNIEnv* env, jobject thiz, jint textureId) {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use the program
    glUseProgram(gProgram);
    
    // Set the active texture unit to 0
    glActiveTexture(GL_TEXTURE0);
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set the texture sampler to texture unit 0
    glUniform1i(gTextureUniform, 0);
    
    // Bind and enable position VBO
    glBindBuffer(GL_ARRAY_BUFFER, gPositionVBO);
    glVertexAttribPointer(gPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(gPositionHandle);
    
    // Bind and enable texcoord VBO
    glBindBuffer(GL_ARRAY_BUFFER, gTexCoordVBO);
    glVertexAttribPointer(gTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(gTexCoordHandle);
    
    // Draw the quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Disable vertex arrays
    glDisableVertexAttribArray(gPositionHandle);
    glDisableVertexAttribArray(gTexCoordHandle);
}

// Clean up the GL resources
JNIEXPORT void JNICALL
Java_com_example_edgedetection_NativeWrapper_cleanupGL(JNIEnv* env, jobject thiz) {
    // Delete program and shaders
    if (gProgram) {
        glDeleteProgram(gProgram);
        gProgram = 0;
    }
    
    if (gVShader) {
        glDeleteShader(gVShader);
        gVShader = 0;
    }
    
    if (gFShader) {
        glDeleteShader(gFShader);
        gFShader = 0;
    }
    
    // Delete VBOs
    if (gPositionVBO) {
        glDeleteBuffers(1, &gPositionVBO);
        gPositionVBO = 0;
    }
    
    if (gTexCoordVBO) {
        glDeleteBuffers(1, &gTexCoordVBO);
        gTexCoordVBO = 0;
    }
    
    LOGI("GL resources cleaned up");
}

}
