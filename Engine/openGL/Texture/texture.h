#pragma once

#include "Resources/image.h"

class Texture
{
private:
    GLuint id = -1;
    uint32_t m_unit = 0;

public:
    enum WrapMode
    {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP = GL_CLAMP,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    };

    enum Filter
    {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,

        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    };

    Texture() = default;
    Texture(const Image &image, uint32_t mipmap_levels = 0, bool gamma_correcred = false);
    Texture(const char *file, uint32_t mipmap_levels = 0, bool gamma_correcred = false);

    void create(const char *file, uint32_t mipmap_levels = 0, bool gamma_correcred = false);
    void create(const Image &image, uint32_t mipmap_levels = 0, bool gamma_correcred = false);
    void destroy() const;

    void bind() const;
    void unbind() const;
    void use() const;
    void setUnit(uint32_t unit) { m_unit = unit; };

    void wrapMode(WrapMode wp) const;
    void filter(Filter f) const;
    void mipmaps() const;

    GLuint getID() const { return id; }

    friend class Renderer;
};