#ifndef I_DRAWABLE_H
#define I_DRAWABLE_H

class IDrawable
{
public:
    virtual void Draw(const unsigned int screen_width, const unsigned int screen_heigh) = 0;
};

#endif // I_DRAWABLE_H
