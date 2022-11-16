#pragma once

#include "../Engine/Core/application.h"

class UI : public Node
{
    Material m;
    Transform2d transform;
    glm::vec2 canvas_size;
    glm::vec2 size;

    void init() override
    {
        canvas_size = window.getSize();
        ui_camera.ortho(0, canvas_size.x, 0, canvas_size.y);

        m.program = &Program::basic2d;
        m.texture = GPU::gpu.createTexture("../chess/resources/desk.jpg");
        GPU::gpu.textureFilter(m.texture, Texture::Filter::NEAREST);
        GPU::gpu.textureWrapMode(m.texture, Texture::WrapMode::MIRRORED_REPEAT);

        size = {80, 60};
        transform.scale(size);
    }

    void update() override
    {
        if (window.resized())
        {
            canvas_size = window.getSize();
            ui_camera.ortho(0, canvas_size.x, 0, canvas_size.y);
        }

        transform.origin = {(canvas_size.x - size.x) / 2.0f, (canvas_size.y - size.y) / 2.0f};
    }

    void draw() override
    {
        Renderer::setMaterial(m);
        m.program->setPVM(ui_camera.getProjection() * transform.getMatrix());
        Renderer::drawVAO(quad);
    }

    void destroy() override
    {
    }

    static Camera2d ui_camera;
    static RID quad;

public:
    static void initialize()
    {
        quad = GPU::gpu.loadMesh(ResourceManager::createQuadMesh(glm::vec2(0, 1), glm::vec2(1, 0)));
    }
};

Camera2d UI::ui_camera;
RID UI::quad;