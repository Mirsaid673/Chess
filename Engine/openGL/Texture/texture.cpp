#include "texture.h"

Texture::Texture(const Image &image, uint32_t mipmap_levels, bool gamma_correcred)
{
    create(image, mipmap_levels, gamma_correcred);
}

Texture::Texture(const char *file, uint32_t mipmap_levels, bool gamma_correcred)
{
    create(file, mipmap_levels, gamma_correcred);
}

void Texture::create(const char *file, uint32_t mipmap_levels, bool gamma_correcred)
{
    Image im(file);

    create(im, mipmap_levels, gamma_correcred);
    im.free();
}

void Texture::create(const Image &i, uint32_t mipmap_levels, bool gamma_correcred)
{
    ;

    glGenTextures(1, &id);
    bind();

    uint32_t width = i.getWidth();
    uint32_t height = i.getHeight();

    GLenum internal_format = GL_RGB8;
    if (i.getChanbelNum() == 4)
        internal_format = GL_RGBA8;
    if (gamma_correcred)
    {
        if (internal_format == GL_RGB8)
            internal_format = GL_SRGB8;
        else if (internal_format == GL_RGBA8)
            internal_format = GL_SRGB8_ALPHA8;
    }

    glTexStorage2D(GL_TEXTURE_2D, 1 + mipmap_levels, internal_format, width, height);
    if (i.getData())
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, i.getFormat(), i.getType(), i.getData());

    unbind();
}

void Texture::destroy() const
{
    glDeleteTextures(1, &id);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use() const
{
    glActiveTexture(GL_TEXTURE0 + m_unit);
    bind();
}

void Texture::wrapMode(WrapMode wp) const
{
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);

    unbind();
}

void Texture::filter(Filter f) const
{
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

    unbind();
}

void Texture::mipmaps() const
{
    bind();

    glGenerateMipmap(GL_TEXTURE_2D);

    unbind();
}