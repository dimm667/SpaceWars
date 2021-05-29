#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <initializer_list>
#include <glm/gtc/type_ptr.hpp>

class ShaderSubProgram
{
public:
    ShaderSubProgram(const std::string & path_to_file, const GLenum shader_type);
    GLuint getId() const;

    ~ShaderSubProgram();

private:
    std::string source_code_;
    GLuint shader_id_;
};


class ShaderProgram
{
public:
    ShaderProgram(const std::initializer_list<ShaderSubProgram> & shaders_subprogramms);

    void use();
    GLuint getId() const    {return program_id_;}

    void SetUniform(const std::string & name, float value);
    void SetUniform(const std::string & name, int value);
    void SetUniform(const std::string & name, unsigned int value);
    void SetUniform(const std::string & name, bool value);
    void SetUniform(const std::string & name, const glm::mat4 & value, GLboolean transpose = GL_FALSE);
    void SetUniform(const std::string & name, const glm::vec3 & value);

private:
    void Attach(const ShaderSubProgram & subprogramm);
    void Link();
    GLuint program_id_;
};



#endif // SHADER_PROGRAM_H
