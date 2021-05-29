#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#include <glad/glad.h>

class RenderBuffer
{
public:
    RenderBuffer(const GLsizei width, const GLsizei height, const GLenum internal_format)
    {
        glGenRenderbuffers(1, &render_buffer_object_);
        glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_);
        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
    }

    GLuint getId() const {return render_buffer_object_;}

private:
    GLuint render_buffer_object_;
};

#endif // RENDERBUFFER_H
