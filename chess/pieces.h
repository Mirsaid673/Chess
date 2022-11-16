/* #pragma once

#include "piece.h"

class Pawn : public Piece
{
public:
    Pawn() = default;
    Pawn(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::PawnQuad;
    }
};

class Bishop : public Piece
{
public:
    Bishop() = default;
    Bishop(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::BishopQuad;
    }
};

class Knight : public Piece
{
public:
    Knight() = default;
    Knight(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::KnightQuad;
    }
};

class Rook : public Piece
{
public:
    Rook() = default;
    Rook(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::RookQuad;
    }
};

class Queen : public Piece
{
public:
    Queen() = default;
    Queen(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::QueenQuad;
    }
};

class King : public Piece
{
public:
    King() = default;
    King(Piece::Color color) { setColor(color); }

    void init() override
    {
        quad = Piece::KingQuad;
    }
};
 */