#include "text_render.h"

TextStream::TextStream(const TextStream& other) :
x_{other.x_}, y_{other.y_}, scale_{other.scale_}, color_{other.color_}
{}

void TextStream::UpdateCoordinates(GLfloat x, GLfloat y)
{
    x_ = x;
    y_ = y;
}

TextStream::TextStream(GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) :
    x_{x}, y_{y}, scale_{scale}, color_{color}
{
}

TextStream::~TextStream() {
}

void TextStream::Clear()
{
    str(std::string());
}

const std::string TextStream::GetString() const
{
    return str();
}

TextRender::TextRender()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte character_id = 0; character_id < 128; character_id++)
    {
        // Load character glyph
        if (FT_Load_Char(face, static_cast<char>(character_id), FT_LOAD_RENDER))
        {
            throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)),
            glm::ivec2(static_cast<int>(face->glyph->bitmap_left), static_cast<int>(face->glyph->bitmap_top)),
            static_cast<int>(face->glyph->advance.x)
        };
        characters_[character_id] = character;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRender::PrintText(const std::string& text, const GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3& color)
{
    glDisable(GL_DEPTH_TEST);
    shader_program_.use();
    shader_program_.SetUniform("projection", projectionGlyph_);
    shader_program_.SetUniform("text", 0);
    shader_program_.SetUniform("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    // Iterate through all characters
    std::string::const_iterator character_id;
    GLfloat new_x = x;
    for (character_id = text.begin(); character_id != text.end(); character_id++)
    {
        Character character = characters_[*character_id];

        GLfloat xpos = new_x + character.bearing.x * scale;
        GLfloat ypos = y - (character.size.y - character.bearing.y) * scale;

        GLfloat w = character.size.x * scale;
        GLfloat h = character.size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, character.textureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        new_x += (character.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
}

TextStream& TextRender::GetRenderTextStream(const GLfloat x, const GLfloat y, const GLfloat scale, const glm::vec3& color)
{
    streams_.emplace_back(x, y, scale, color);
    return streams_.back();
}

void TextRender::Draw(const unsigned int screen_width, const unsigned int screen_heigh)
{
    projectionGlyph_ = glm::ortho(0.0f, static_cast<float>(screen_width), 0.0f, static_cast<float>(screen_heigh));
    for(auto& stream : streams_)
    {
        PrintText(stream.str(), stream.x_, stream.y_, stream.scale_, stream.color_);
        stream.Clear();
    }
}
