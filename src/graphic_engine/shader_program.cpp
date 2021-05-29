#include "shader_program.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

ShaderSubProgram::ShaderSubProgram(const std::string &pathToFile, const GLenum shaderType) :
    shader_id_(0),
    source_code_("")
{
    std::ifstream fileStream;
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fileStream.open(pathToFile);

        std::stringstream strStream;
        strStream << fileStream.rdbuf();
        fileStream.close();
        source_code_ = strStream.str();
    }
    catch(std::ifstream::failure e)
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    shader_id_ = glCreateShader(shaderType);
    const char * sourceCodeCharString = source_code_.c_str();
    glShaderSource(shader_id_, 1, &sourceCodeCharString, NULL);
    glCompileShader(shader_id_);

    // check compile errors
    GLint success;
    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader_id_, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED: " + pathToFile + "\n" + std::string(infoLog));
    }
}

GLuint ShaderSubProgram::getId() const
{
    return shader_id_;
}

ShaderSubProgram::~ShaderSubProgram()
{
    glDeleteShader(shader_id_);
}


ShaderProgram::ShaderProgram(const std::initializer_list<ShaderSubProgram> & shaders_subprogramms) :
    program_id_(0)
{
    program_id_ = glCreateProgram();

    for(auto && subprogram : shaders_subprogramms)
    {
        Attach(subprogram);
    }
    Link();
}

void ShaderProgram::Attach(const ShaderSubProgram & subprogramm)
{
     glAttachShader(program_id_, subprogramm.getId());
}

void ShaderProgram::Link()
{
    glLinkProgram(program_id_);

    GLint success;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program_id_, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
}

void ShaderProgram::use()
{
    glUseProgram(program_id_);
}

void ShaderProgram::SetUniform(const std::string & name, float value)
{
    glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string & name, int value)
{
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string & name, unsigned int value)
{
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 & value, GLboolean transpose)
{
    glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, transpose, &value[0][0]);
}

void ShaderProgram::SetUniform(const std::string & name, const glm::vec3 & value)
{
     glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
