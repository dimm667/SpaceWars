#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <array>
#include <list>
#include <limits>
#include <sstream>
#include <ft2build.h>
#include "shader_program.h"
#include "i_drawable.h"
#include FT_FREETYPE_H

struct Character {
    GLuint      textureID;
    glm::ivec2  size;
    glm::ivec2  bearing;
    GLint      advance;
};

class TextRender;

class TextStream : public std::stringstream
{
public:
    TextStream(const TextStream& other);
    TextStream(GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    ~TextStream();

    void UpdateCoordinates(GLfloat x, GLfloat y);
    void Clear();
    const std::string GetString() const;

friend TextRender;
private:
    GLfloat x_;
    GLfloat y_;
    GLfloat scale_;
    glm::vec3 color_;
};

class TextRender : public IDrawable
{
public:
    TextRender();

    TextStream& GetRenderTextStream(const GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3& color);
    void Draw(const unsigned int screen_width, const unsigned int screen_heigh) override;

private:
    void PrintText(const std::string& text, const GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3& color);
    ShaderProgram shader_program_{{{"src/graphic_engine/shaders/vertex_glyph.glsl", GL_VERTEX_SHADER},
                                    {"src/graphic_engine/shaders/fragment_glyph.glsl", GL_FRAGMENT_SHADER}}};

    std::list<TextStream> streams_{};
    std::array<Character, 128> characters_{};
    glm::mat4 projectionGlyph_{};
    GLuint VAO_;
    GLuint VBO_;
};


#endif // TEXT_RENDER_H
