#pragma once

#include "piece_choose_button.h"

class UI
{
public:
    static PieceChooseButton buttons[4];
    static void hideButtons()
    {
        for (auto &btn : buttons)
            btn.enabled = false;
    }
    static void showButtons()
    {
        for (auto &btn : buttons)
            btn.enabled = true;
    }
    static void setButtonsColor(Piece::Color color)
    {
        for (auto &btn : buttons)
            btn.setColor(color);
    }
};
