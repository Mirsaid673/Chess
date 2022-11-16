#pragma once

#include "../Engine/Core/application.h"
#include "logic.h"
#include "available_cells.h"
#include <list>

class Piece : public Node
{
public:
    enum Color : int8_t
    {
        BLACK = -1,
        EMPTY = 0,
        WHITE = 1,
    };

    enum Type : int8_t
    {
        PAWN_B = -6,
        BISHOP_B,
        KNIGHT_B,
        ROOK_B,
        QUEEN_B,
        KING_B,
        NONE,
        KING_W,
        QUEEN_W,
        ROOK_W,
        KNIGHT_W,
        BISHOP_W,
        PAWN_W,

    };

    friend Type operator-(Type a, Type b) { return (Type)((uint8_t)a - (uint8_t)b); }
    friend Type operator-(Type a, int b) { return (Type)((uint8_t)a - b); }
    friend Type operator*(Type a, int b) { return (Type)((uint8_t)a * b); }

private:
    Material m;
    RID quad;
    bool moved = false;

public:
    Type type = Type::NONE;
    Color color = Color::EMPTY;
    
    glm::ivec2 last_pos;
    AvailableCells available_cells;
    Transform2d transform;
    bool first_move_large = false; // only for pawns

    void onMouseDown(const glm::vec2 &mouse_world);
    void onMouseHold(const glm::vec2 &mouse_world);
    void onMouseUp(const glm::vec2 &mouse_world);

    void draw() override
    {
        Renderer::setMaterial(m);
        Renderer::setTransform2d(parent->getTransform2d() * transform.getMatrix());
        Renderer::drawVAO(quad);
    }

    void remove()
    {
        color = Color::EMPTY;
        type = Type::NONE;
        removeParent();
    }

    void showAvailableCells();
    void hideAvailableCells();

    void setType(Type t);
    void setTypeNoColor(Type t);

    Color getColor() const { return color; }
    glm::mat3 getTransform2d() override { return transform.getMatrix(); }
    const glm::ivec2 &getLastPos() const { return last_pos; }
    Type getType() const { return type; }
    Type getTypeNoColor() const { return (Type)glm::abs((int8_t)type); }
    bool isMoved() const { return moved; }
    bool operator==(const Piece &other) const { return transform.origin == other.transform.origin; }

    Piece() : m(Material(pieces_program)) {}
    Piece(Type t) : m(Material(pieces_program)) { setType(t); }

public:
    static Program pieces_program;

    static RID white_texture_atlas;
    static RID black_texture_atlas;

    static RID PawnQuad;
    static RID BishopQuad;
    static RID KnightQuad;
    static RID RookQuad;
    static RID QueenQuad;
    static RID KingQuad;

    static std::vector<Vertex2d> getTexturePart(int index);
    static void initialize(const char *white_texture, const char *black_texture);
    static void determinate();
};