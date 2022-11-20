#include "piece_choose_button.h"
#include "ui.h"

RID PieceChooseButton::QueenQuad;
RID PieceChooseButton::RookQuad;
RID PieceChooseButton::BishopQuad;
RID PieceChooseButton::KnightQuad;
Program PieceChooseButton::button_program;

void PieceChooseButton::onMousePress()
{
    Logic::pawn_to_turn = piece_type;
    Logic::onButtonPress();
}