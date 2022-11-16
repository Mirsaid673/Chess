#pragma once

#include "../Engine/Core/application.h"
#include "input_manager.h"
#include "desk.h"
#include "UI/canvas.h"

int8_t board[8][8] =
    {
        {-3, -4, -5, -2, -1, -5, -4, -3},
        {-6, -6, -6, -6, -6, -6, -6, -6},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {6, 6, 6, 6, 6, 6, 6, 6},
        {3, 4, 5, 2, 1, 5, 4, 3},
};

class Chess : public Application
{
    Desk desk;
    Origin origin;
    InputMananger im;

    float scale = 4;
    void init() override
    {
        Image::setFlipOnLoad(true);
        Renderer::setClearColor(glm::vec3(1, 1, 0));

        Piece::initialize("../chess/resources/white pieces.png", "../chess/resources/black pieces.png");
        Canvas::initialize();

        desk.initBoard(origin, board);
        Logic::setDesk(desk);
        origin.transform.origin = glm::vec2(-3.5f, -3.5f);

        scene.addNode(desk);
        scene.addNode(origin);
        scene.addNode(im);
        
        float asp = (float)window.getWidth() / (float)window.getHeight();
        camera2d.ortho(-asp * scale, asp * scale, scale, -scale);
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }

    void determinate() override
    {
        Piece::determinate();
    }

    void on_resize() override
    {
        float w = window.getWidth();
        float h = window.getHeight();

        if (h > w)
        {
            float asp = h / w;
            camera2d.ortho(-scale, scale, asp * scale, -asp * scale);
        }
        else
        {
            float asp = w / h;
            camera2d.ortho(-asp * scale, asp * scale, scale, -scale);
        }
    }
};