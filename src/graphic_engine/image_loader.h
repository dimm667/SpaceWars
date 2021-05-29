#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include <memory>


class ImageLoader
{
public:
    using TDataPointer = void *;
    enum class PixelDataType
    {
        R, G, B, A
    };

    enum class ImageFormat: int
    {
        UNDEF = 0,
        RED =   1,
        RGB =   3,
        RGBA =  4,
    };

    ImageLoader(const std::string & pathToFile, bool flip = true);
    ~ImageLoader();
    float getPixel(PixelDataType type, unsigned int x, unsigned int y);
    float getWidth() const {return width;}
    float getHeight() const {return height;}
    ImageFormat getImageFormat() { return format;}
    const TDataPointer getData() {return data;}

private:
    TDataPointer data = nullptr;
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    ImageFormat format = ImageFormat::UNDEF;
};

#endif // IMAGELOADER_H
