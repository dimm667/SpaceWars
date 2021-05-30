#include "image_loader.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "../../deps/stb_image.h"

ImageLoader::ImageLoader(const std::string & pathToFile, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    int t_width{};
    int t_height{};
    data = stbi_load(pathToFile.c_str(), &t_width, &t_height, &nrChannels, 0);
    width = t_width;
    height = t_height;
    format = static_cast<ImageFormat>(nrChannels);
    if(data == nullptr)
    {
        stbi_image_free(data);
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Failed to load texture: ") + pathToFile + std::string("\n"));
    }
}

ImageLoader::~ImageLoader()
{
    stbi_image_free(data);
}

float ImageLoader::getPixel(PixelDataType type, unsigned int x, unsigned int y)
{
    return static_cast<float>(static_cast<unsigned char *>(data)[(x + height*y) * nrChannels + static_cast<unsigned int>(type)]) / 255.0f;
}
