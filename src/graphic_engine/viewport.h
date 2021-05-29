#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <glad/glad.h>
#include <stdexcept>

class Viewport
{
public:
    Viewport(const GLint vp_x, const GLint vp_y, const GLsizei vp_width, const GLsizei vp_height) ;

    void Update(const GLsizei vp_width, const GLsizei vp_height);
    void Activate();

private:
    GLint x_;
    GLint y_;
    GLsizei width_;
    GLsizei height_;
};

#endif // VIEWPORT_H
