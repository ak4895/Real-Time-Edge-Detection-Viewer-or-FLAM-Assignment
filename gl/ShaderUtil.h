#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>

/**
 * ShaderUtil - Helper class for loading and compiling OpenGL shaders
 */
class ShaderUtil {
public:
    /**
     * Loads and compiles a shader
     * 
     * @param shaderType The shader type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
     * @param shaderSource The shader source code
     * @return The shader handle or 0 on failure
     */
    static GLuint loadShader(GLenum shaderType, const char* shaderSource) {
        // Create the shader object
        GLuint shader = glCreateShader(shaderType);
        if (shader == 0) {
            return 0;
        }

        // Load the shader source
        glShaderSource(shader, 1, &shaderSource, nullptr);
        
        // Compile the shader
        glCompileShader(shader);
        
        // Check the compile status
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            
            if (infoLen) {
                char* infoLog = new char[infoLen];
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
                std::string errorMsg = "Error compiling shader: ";
                errorMsg += infoLog;
                delete[] infoLog;
                
                // Delete the shader and return error
                glDeleteShader(shader);
                return 0;
            }
        }
        
        return shader;
    }
    
    /**
     * Creates a program with vertex and fragment shaders
     * 
     * @param vertexSource The vertex shader source code
     * @param fragmentSource The fragment shader source code
     * @return The program handle or 0 on failure
     */
    static GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
        // Load the vertex and fragment shaders
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
        
        // Create the program and attach the shaders
        GLuint program = glCreateProgram();
        if (program == 0) {
            return 0;
        }
        
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        
        // Link the program
        glLinkProgram(program);
        
        // Check the link status
        GLint linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            
            if (infoLen) {
                char* infoLog = new char[infoLen];
                glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
                std::string errorMsg = "Error linking program: ";
                errorMsg += infoLog;
                delete[] infoLog;
                
                // Delete the program and shaders
                glDeleteProgram(program);
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return 0;
            }
        }
        
        // Clean up the shaders, they're attached to the program now
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return program;
    }
};
