#pragma once

#include <glm/glm.hpp>

class Desk;
class Piece;

class Logic
{
private:
public:
    static Piece *last_moved_piece;
    static Piece *turning_pawn;
    static Desk *desk;
    static int8_t pawn_to_turn;
    static int8_t moving_color;
    static bool pause;

    static void setDesk(Desk &d) { desk = &d; }
    static void calculateAvailableMoves(Piece &piece);
    static void calculateAvailableMoves(int8_t color);
    static bool canMove(Piece &piece, const glm::ivec2 &new_pos);
    static void changeMovingColor() { moving_color *= -1; }
    static void onPawnTurn(Piece &piece);
    static void onButtonPress();
};