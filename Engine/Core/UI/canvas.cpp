#include "canvas.h"

#include "Camera/camera2d.h"
#include "Renderer/renderer.h"
#include "global.h"

RID Canvas::button_quad;

void Canvas::initialize()
{
    button_quad = GPU::gpu.loadMesh(ResourceManager::createQuadMesh(glm::vec2(0, 1), glm::vec2(1, 0)));
}

bool isPointInside(const glm::ivec2 &p, const Button &b)
{
    return (p.x >= b.origin.x && p.x <= (b.size.x + b.origin.x)) &&
           (p.y >= b.origin.y && p.y <= (b.size.y + b.origin.y));
}

void Canvas::update()
{
    glm::ivec2 cursor = input.getCursorPos();
    for (const auto button : buttons)
    {
        if (not button->enabled)
            continue;

        if (button->pressed)
            button->onMouseDown();
        else
            button->onMouseUp();

        if (isPointInside(input.getCursorPos(), *button))
        {
            if (input.getMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
            {
                button->pressed = true;
                button->onMousePress();
            }
            else if (input.getMouseButtonRelease(GLFW_MOUSE_BUTTON_LEFT) && button->pressed)
            {
                button->pressed = false;
                button->onMouseRelease();
            }
        }
        else
        {
            if (button->pressed)
            {
                button->pressed = false;
                button->onMouseRelease();
            }
        }
    }
}

void Canvas::draw()
{
    for (const auto button : buttons)
    {
        if (not button->enabled)
            continue;
        Renderer::setMaterial(button->material);
        button->material.program->use();
        button->material.program->setPVM(projection * button->getMatrix());
        Renderer::drawVAO(button_quad);
    }
}

void Canvas::resize(const glm::ivec2 &new_size)
{
    size = new_size;
    Camera2d c;
    c.ortho(0, size.x, 0, size.y);

    projection = c.getProjection();
}