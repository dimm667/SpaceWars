
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "texture.h"
#include "shader_program.h"
#include "i_drawable.h"

class Background : public IDrawable
{
public:
    explicit Background(const std::string& texture_filename);

    void Draw(const unsigned int screen_width, const unsigned int screen_heigh) override;

private:
    static constexpr float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };
    TextureImage background_image_;
    ShaderProgram shader_program_{{{"src/graphic_engine/shaders/vertex_background.glsl", GL_VERTEX_SHADER},
                                  {"src/graphic_engine/shaders/fragment_background.glsl", GL_FRAGMENT_SHADER}}};
    GLuint quadVAO;
    GLuint quadVBO;

};

#endif // BACKGROUND_H
