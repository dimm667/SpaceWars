#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <glad/glad.h>
#include <stdexcept>
#include <vector>
#include "render_buffer.h"

class FrameBuffer
{
public:
    FrameBuffer(const GLenum fbbind_target_ = GL_FRAMEBUFFER);

    struct AttachmentConfig
    {
        GLenum attach_to;
        GLuint texture_id;
    };
    using Attachments = std::vector<AttachmentConfig>;

    void AttachTextures2D(const Attachments & attachments);
    void AttachRenderBuffer(const RenderBuffer & rb, GLenum attach_to);
    void AttachTextures(const Attachments & attachments);

    ~FrameBuffer();

private:
    void Bind();
    void Unbind();

    GLuint frame_buffer_object_;
    GLenum bind_target_;
};

#endif // FRAMEBUFFER_H
