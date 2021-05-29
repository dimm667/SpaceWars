#include "framebuffer.h"

FrameBuffer::FrameBuffer(const GLenum fbbind_target) :
    bind_target_(fbbind_target)
{
    if(fbbind_target != GL_DRAW_FRAMEBUFFER &&
       fbbind_target != GL_READ_FRAMEBUFFER &&
       fbbind_target != GL_FRAMEBUFFER)
    {
        throw std::runtime_error(std::string("ERROR::FRAMEBUFFER::CREATING\n") + std::string("Invalid frame buffer targed.\n"));
    }

    glGenFramebuffers(1, &frame_buffer_object_);
    if(glGetError() == GL_INVALID_VALUE)
    {
        throw std::runtime_error(std::string("ERROR::FRAMEBUFFER::CREATING\n") + std::string("Can't create framebuffer.\n"));
    }
}

void FrameBuffer::AttachTextures2D(const FrameBuffer::Attachments & attachments)
{
    Bind();
    std::vector<GLuint> att;
    for(auto && attachment_config : attachments)
    {
        glFramebufferTexture2D(bind_target_, attachment_config.attach_to, GL_TEXTURE_2D, attachment_config.texture_id, 0);
        att.push_back(attachment_config.attach_to);
    }

    glDrawBuffers(att.size(), &att.front());
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("ERROR::TEXTUREBUFFER::FRAMEBUFFER\n");
    }
    Unbind();
}

void FrameBuffer::AttachRenderBuffer(const RenderBuffer & rb, GLenum attach_to)
{
    Bind();
    glFramebufferRenderbuffer(bind_target_, attach_to, GL_RENDERBUFFER, rb.getId());
    Unbind();
}

void FrameBuffer::AttachTextures(const FrameBuffer::Attachments & attachments)
{
    Bind();
    std::vector<GLuint> att;
    for(auto && attachment_config : attachments)
    {
        glFramebufferTexture(bind_target_, attachment_config.attach_to, attachment_config.texture_id, 0);
        att.push_back(attachment_config.attach_to);
    }
    glDrawBuffers(att.size(), &att.front());
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("ERROR::TEXTUREBUFFER::FRAMEBUFFER\n");
    }
    Unbind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &frame_buffer_object_);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(bind_target_, frame_buffer_object_);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(bind_target_, 0);
}

