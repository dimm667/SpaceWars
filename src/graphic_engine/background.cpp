#include "background.h"

Background::Background(const std::string& texture_filename) :
        background_image_{texture_filename}
{
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Background::Draw(const unsigned int screen_width, const unsigned int screen_heigh)
{
    background_image_.activate(GL_TEXTURE0);
    shader_program_.use();
    shader_program_.SetUniform("my_texture", background_image_.getBindUnitID());
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glClear(GL_DEPTH_BUFFER_BIT);
}
