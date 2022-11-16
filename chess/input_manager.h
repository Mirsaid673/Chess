#pragma once

#include "../Engine/Core/application.h"
#include "desk.h"
#include "Math/AABB.h"

class InputMananger : public Node
{
    Piece *last_piece = nullptr;
    Piece *current_piece = nullptr;

    void init() override
    {
        Logic::calculateAvailableMoves(Logic::moving_color);
    }

    void update() override
    {
        glm::vec2 cursor_desk_pos = getCursorWorldPosition(input.getCursorPos()) + glm::vec2(3.5f);
        if (input.getMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
        {
            bool clicked_on_piece = false;
            for (auto &piece : Logic::desk->pieces_manager.pieces)
            {
                if (piece->getColor() != Logic::moving_color)
                    continue;

                glm::vec2 pos = piece->transform.origin;
                AABB piece_quad(pos - glm::vec2(0.5f), pos + glm::vec2(0.5f));

                if (piece_quad.isPointInside(cursor_desk_pos))
                {
                    if (last_piece != piece && last_piece) // if we click on another piece
                    {
                        last_piece->hideAvailableCells();
                    }
                    last_piece = piece;
                    current_piece = piece;
                    piece->onMouseDown(cursor_desk_pos);

                    clicked_on_piece = true;
                    break;
                }
            }
            if ((not clicked_on_piece) && last_piece)
            {
                if (last_piece->getColor() == Logic::moving_color)
                    last_piece->onMouseUp(cursor_desk_pos);
            }
        }
        else if (input.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
        {
            if (current_piece)
                current_piece->onMouseHold(cursor_desk_pos);
        }
        else if (input.getMouseButtonRelease(GLFW_MOUSE_BUTTON_LEFT))
        {
            if (current_piece)
            {
                current_piece->onMouseUp(cursor_desk_pos);
                current_piece = nullptr;
            }
        }
    }

    glm::vec2 getCursorWorldPosition(glm::ivec2 c_pos)
    {
        float w = (float)window.getWidth();
        float h = (float)window.getHeight();

        float x = ((float)c_pos.x / w - 0.5f) * camera2d.getWidth();
        float y = -((float)c_pos.y / h - 0.5f) * camera2d.getHeight();

        return glm::vec2(x, y);
    }
};