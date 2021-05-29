#include "viewport.h"

Viewport::Viewport(const GLint vp_x, const GLint vp_y, const GLsizei vp_width, const GLsizei vp_height) :
    x_(vp_x), y_(vp_y), width_(vp_width), height_(vp_height)
{
    if(vp_width < 0 || vp_height < 0)
    {
        throw std::runtime_error(std::string("ERROR::VIEWPORT::CREATING\n") + std::string("Invalid view port parameters.\n"));
    }
    Activate();
}

void Viewport::Update(const GLsizei vp_width, const GLsizei vp_height)
{
    width_ = vp_width;
    height_ = vp_height;
}

void Viewport::Activate()
{
    glViewport(x_, y_, width_, height_);
}
