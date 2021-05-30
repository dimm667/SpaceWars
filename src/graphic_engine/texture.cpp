#include "texture.h"
#include "image_loader.h"


Texture::Texture(GLenum type) :
    textureType(type)
{
    glGenTextures(1, &texID);
    if(texID == GL_INVALID_VALUE )
    {
        throw std::runtime_error(std::string("ERROR::TEXTURE::LOAD\n") + std::string("Max textures number reached.\n"));
    }
}


void Texture::configure(const TextureParameterList & params)
{
    glBindTexture(textureType, texID);
    for(auto && param : params)
    {
        glTexParameteri(textureType, param.name, param.value);
    }
}

GLenum Texture::getBindUnitID() const
{
    return bindedToUnit - GL_TEXTURE0;
}

void Texture::activate(GLenum textureUint)
{
    bindedToUnit = textureUint;
    glActiveTexture(textureUint);
    glBindTexture(textureType, texID);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texID);
}

TextureBuffer::TextureBuffer(int buffWidth, int buffHeight, TexImageFormat format) :
    Texture(GL_TEXTURE_2D), width(buffWidth), height(buffHeight)
{
    glBindTexture(textureType, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, format.internalFormat,
                 width, height, 0, format.pixelFormat, format.pixelType, NULL);

    Texture::configure({  {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                          {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                          {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                          {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
                          {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}});
}

TextureCubeMapBuffer::TextureCubeMapBuffer(int buffWidth, int buffHeight, FrameBuffer & fb, TexImageFormat format, GLenum attachment) :
    Texture(GL_TEXTURE_CUBE_MAP), frameBuffer(fb), width(buffWidth), height(buffHeight)
{
    glBindTexture(textureType, texID);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format.internalFormat, width, height, 0, format.pixelFormat, format.pixelType, NULL);
    }

    configure({{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
              {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
              {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
              {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
              {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}});

    frameBuffer.AttachTextures({{attachment, texID}});
}

TextureImage::TextureImage(const std::string & pathToFile) :
    Texture(GL_TEXTURE_2D)
{

    ImageLoader image(pathToFile);

    GLenum format{};
    switch (image.getImageFormat())
    {
        case ImageLoader::ImageFormat::RED: format = GL_RED; break;
        case ImageLoader::ImageFormat::RGB: format = GL_RGB; break;
        case ImageLoader::ImageFormat::RGBA: format = GL_RGBA; break;
        default: break;
    }

    glBindTexture(textureType, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    configure({{GL_TEXTURE_WRAP_S, GL_REPEAT},
               {GL_TEXTURE_WRAP_T, GL_REPEAT},
               {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
               {GL_TEXTURE_MAG_FILTER, GL_LINEAR}});
}

TextureCubeMap::TextureCubeMap(const std::vector<std::string> & pathsToFiles) :
    Texture(GL_TEXTURE_CUBE_MAP)
{
    glBindTexture(textureType, texID);
    int texIdx = 0;
    for(auto && pathToFile : pathsToFiles)
    {
        ImageLoader image(pathToFile, false);
        GLenum format{GL_RGBA};
        switch (image.getImageFormat())
        {
            case ImageLoader::ImageFormat::RED: format = GL_RED; break;
            case ImageLoader::ImageFormat::RGB: format = GL_RGB; break;
            case ImageLoader::ImageFormat::RGBA: format = GL_RGBA; break;
            default: break;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (texIdx++),
                     0, GL_RGB, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
    }

    configure({{GL_TEXTURE_MIN_FILTER, GL_LINEAR},
              {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
              {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
              {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
              {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}});
}

