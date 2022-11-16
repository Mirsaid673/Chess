#pragma once

#include "piece.h"
#include <array>

class PiecesManager
{
public:
    std::array<Piece *, 64> pieces;
    PiecesManager()
    {
        for (auto &piece : pieces)
        {
            piece = new Piece();
        }
    }
    ~PiecesManager()
    {
        for (auto &piece : pieces)
        {
            delete piece;
        }
    }

    Piece *newPiece(Piece::Type t, const glm::ivec2 &pos)
    {
        pieces[pos.y * 8 + pos.x]->setType(t);
        return pieces[pos.y * 8 + pos.x];
    }

    Piece &getPiece(const glm::ivec2 &pos)
    {
        return *pieces[pos.y * 8 + pos.x];
    }

    void removePiece(const glm::ivec2 &pos)
    {
        pieces[pos.y * 8 + pos.x]->remove();
    }
};