#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <filesystem>
#include <string>
#define STB_INCLUDE_LINE_GLSL
#define STB_INCLUDE_IMPLEMENTATION
#include <stb/stb_include.h>

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string& filename, GLenum type) const
{
    auto file_path = std::filesystem::path(filename);
    auto file_path_string = file_path.string();
    auto parent_path_string = file_path.parent_path().string();
    auto path_to_includes = &(parent_path_string[0]);
    char error[256];

    // Read the file as a string and resolve any "#include"s recursively
    auto source = stb_include_file(&(file_path_string[0]), nullptr, path_to_includes, error);

    // Check if any loading errors happened
    if (source == nullptr) {
        std::cerr << "ERROR: " << error << std::endl;
        return false;
    }


    // TODO: Complete this function
    // Note: The function "checkForShaderCompilationErrors" checks if there is
    //  an error in the given shader. You should use it to check if there is a
    //  compilation error and print it so that you can know what is wrong with
    //  the shader. The returned string will be empty if there is no errors.
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    free(source);

    std::string errorLog = checkForShaderCompilationErrors(shader);
    if (!errorLog.empty())
    {
        std::cerr << "ERROR: Shader compilation failed (" << filename << "): " << errorLog << std::endl;
        glDeleteShader(shader);
        return false;
    }

    glAttachShader(program, shader);
    glDeleteShader(shader);

    // We return true if the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const
{
    // TODO: Complete this function
    // Note: The function "checkForLinkingErrors" checks if there is
    //  an error in the given program. You should use it to check if there is a
    //  linking error and print it so that you can know what is wrong with the
    //  program. The returned string will be empty if there is no errors.
    glLinkProgram(program);

    std::string errorLog = checkForLinkingErrors(program);
    if (!errorLog.empty())
    {
        std::cerr << "ERROR: program linking failed: " << errorLog << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader)
{
    // Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program)
{
    // Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}