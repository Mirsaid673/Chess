#pragma once

#include "Texture/texture.h"
#include <glm/glm.hpp>

class Framebuffer
{
private:
    Texture texture;
    glm::ivec2 size;
    GLuint id = -1;
    GLuint rbo = -1;

public:
    Framebuffer() = default;

    void create(int w, int h);
    void use() const;
    void clear() const;

    void bind() const;
    void unbind() const;
    void destroy() const;

    void resize(int w, int h);
    const glm::ivec2 &getSize() const { return size; }

    static void bindMain() { glBindFramebuffer(GL_FRAMEBUFFER, 0);}
};