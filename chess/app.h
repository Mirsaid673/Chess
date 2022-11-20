#pragma once

#include "../Engine/Core/application.h"
#include "ui.h"
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
    Canvas canvas;

    float scale = 4;
    void init() override
    {
        Image::setFlipOnLoad(true);
        Renderer::setClearColor(glm::vec3(1, 1, 0));

        Piece::initialize("../chess/resources/white pieces.png", "../chess/resources/black pieces.png");
        PieceChooseButton::initialize();
        canvas.resize(window.getSize());

        desk.initBoard(origin, board);
        Logic::setDesk(desk);
        origin.transform.origin = glm::vec2(-3.5f, -3.5f);

        scene.addNode(desk);
        scene.addNode(origin);
        scene.addNode(im);
        scene.addNode(canvas);

        UI::buttons[0].binding_point = glm::ivec2(0, 0);
        UI::buttons[0].setColor(Piece::Color::WHITE);
        UI::buttons[0].setType(Piece::Type::ROOK_W);

        UI::buttons[1].binding_point = glm::ivec2(1, 0);
        UI::buttons[1].setColor(Piece::Color::WHITE);
        UI::buttons[1].setType(Piece::Type::QUEEN_W);

        UI::buttons[2].binding_point = glm::ivec2(0, 1);
        UI::buttons[2].setColor(Piece::Color::WHITE);
        UI::buttons[2].setType(Piece::Type::KNIGHT_W);

        UI::buttons[3].binding_point = glm::ivec2(1, 1);
        UI::buttons[3].setColor(Piece::Color::WHITE);
        UI::buttons[3].setType(Piece::Type::BISHOP_W);

        for (auto &btn : UI::buttons)
            canvas.addButton(btn);

        UI::hideButtons();

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
        canvas.resize(window.getSize());

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