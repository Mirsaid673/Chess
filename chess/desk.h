#pragma once

#include "pieces_manager.h"

class Origin : public Node
{
public:
    Transform2d transform;

    glm::mat3 getTransform2d() override { return transform.getMatrix(); }
};

class Desk : public Node
{
public:
    PiecesManager pieces_manager;
    Origin *origin;

    Transform2d transform;
    RID quad;
    Material m;

    glm::ivec2 white_king_pos;
    glm::ivec2 black_king_pos;

    void init() override
    {
        quad = GPU::gpu.getDeafultQuad();
        m.texture = GPU::gpu.createTexture("../chess/resources/desk.jpg");
        GPU::gpu.textureFilter(m.texture, Texture::Filter::NEAREST);
        m.program = &Program::basic2d;

        transform.scale({4, 4});
    }

    void initBoard(Origin &org, int8_t board[8][8])
    {
        origin = &org;
        for (int i = 0; i < 4; i++)
        {
            std::swap(board[i], board[7 - i]);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Piece::Type t = (Piece::Type)board[i][j];
                if (t == Piece::Type::NONE)
                    continue;

                auto new_piece = pieces_manager.newPiece(t, glm::vec2(j, i));
                org.addChild(*new_piece);
                new_piece->transform.origin = glm::vec2(j, i);
                new_piece->last_pos = glm::ivec2(j, i);

                if (t == Piece::KING_W)
                    white_king_pos = glm::ivec2(j, i);
                else if (t == Piece::KING_B)
                    black_king_pos = glm::ivec2(j, i);
            }
        }
    }

    void update() override
    {
    }

    void draw() override
    {
        Renderer::setMaterial(m);
        Renderer::setTransform2d(transform.getMatrix());
        Renderer::drawVAO(quad);
    }

    Piece &getPiece(const glm::ivec2 &coord)
    {
        return pieces_manager.getPiece(coord);
    }

    Piece::Color getPieceColor(const glm::ivec2 &coord)
    {
        return pieces_manager.getPiece(coord).getColor();
    }

    Piece::Type false_move_type;
    Piece::Color false_move_color;

    void falseMove(const glm::ivec2 &p1, const glm::ivec2 &p2)
    {
        false_move_type = pieces_manager.getPiece(p2).getType();
        false_move_color = pieces_manager.getPiece(p2).getColor();

        pieces_manager.getPiece(p1).last_pos = p2;

        pieces_manager.getPiece(p2).type = Piece::Type::NONE;
        pieces_manager.getPiece(p2).color = Piece::Color::EMPTY;

        std::swap(pieces_manager.pieces[p1.y * 8 + p1.x], pieces_manager.pieces[p2.y * 8 + p2.x]);
    }

    void returnFalseMove(const glm::ivec2 &p1, const glm::ivec2 &p2)
    {
        std::swap(pieces_manager.pieces[p1.y * 8 + p1.x], pieces_manager.pieces[p2.y * 8 + p2.x]);

        pieces_manager.getPiece(p2).type = false_move_type;
        pieces_manager.getPiece(p2).color = false_move_color;

        pieces_manager.getPiece(p1).last_pos = p1;
    }

    void move(const glm::ivec2 &p1, const glm::ivec2 &p2)
    {
        if (getPieceColor(p2) != Piece::Color::EMPTY)
        {
            removePiece(p2);
        }
        pieces_manager.getPiece(p1).transform.origin = p2;
        pieces_manager.getPiece(p1).last_pos = p2;

        std::swap(pieces_manager.pieces[p1.y * 8 + p1.x], pieces_manager.pieces[p2.y * 8 + p2.x]);
    }

    void removePiece(const glm::ivec2 &p)
    {
        pieces_manager.removePiece(p);
    }

    void setKingPos(const glm::ivec2 &pos, Piece::Color color)
    {
        if (color == Piece::Color::WHITE)
            white_king_pos = pos;
        else
            black_king_pos = pos;
    }

    const glm::ivec2 &getKingPos(Piece::Color color)
    {
        if (color == Piece::Color::WHITE)
            return white_king_pos;
        return black_king_pos;
    }
};