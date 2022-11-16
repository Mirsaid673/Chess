#include "framebuffer.h"

void Framebuffer::create(int w, int h)
{
    size.x = w;
    size.y = h;

    glGenFramebuffers(1, &id);
    bind();

    Image image;
    image.width = w;
    image.height = h;
    image.data = nullptr;

    texture.create(image);
    texture.filter(Texture::Filter::LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    unbind();
}

void Framebuffer::use() const
{
    texture.use();
}

void Framebuffer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::destroy() const
{
    texture.destroy();
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &id);
}

void Framebuffer::resize(int w, int h)
{
    destroy();
    create(w, h);
}