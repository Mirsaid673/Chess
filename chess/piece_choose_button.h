#pragma once

#include "UI/button.h"
#include "logic.h"
#include "piece.h"

class PieceChooseButton : public Button
{
public:
    glm::vec2 relative_size = glm::vec2(1.0f / 7.5f);
    glm::ivec2 binding_point = glm::ivec2(0); //(1, 0) | (0, 0)
                                              //-------+-------
                                              //(1, 1) | (0, 1)
    Piece::Type piece_type;

    void onMousePress() override;

    void onCanvasResize(const glm::ivec2 &canvas_size)
    {
        glm::vec2 c_s = glm::vec2(canvas_size);
        size = glm::vec2(glm::min(c_s.x, c_s.y)) * relative_size;
        origin = c_s / 2.0f - size * glm::vec2(binding_point);
    }

    void setType(Piece::Type type)
    {
        piece_type = type;
        switch (type)
        {
        case Piece::Type::QUEEN_W:
            quad = QueenQuad;
            break;
        case Piece::Type::ROOK_W:
            quad = RookQuad;
            break;
        case Piece::Type::BISHOP_W:
            quad = BishopQuad;
            break;
        case Piece::Type::KNIGHT_W:
            quad = KnightQuad;
            break;
        default:
            quad = RID::invalid;
            break;
        }
    }

    void setColor(Piece::Color color)
    {
        if (color == Piece::Color::WHITE)
            material.texture = Piece::white_texture_atlas;
        else
            material.texture = Piece::black_texture_atlas;
    }

    PieceChooseButton() { material.program = &button_program; }

public:
    static Program button_program;
    static RID QueenQuad;
    static RID RookQuad;
    static RID BishopQuad;
    static RID KnightQuad;

    static void initialize()
    {
        Mesh2d mesh;
        std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};
        mesh.setIndices(indices);

        std::vector<Vertex2d> vertices =
            {
                Vertex2d(glm::vec2(0, 1)),
                Vertex2d(glm::vec2(0, 0)),
                Vertex2d(glm::vec2(1, 1)),
                Vertex2d(glm::vec2(1, 0)),
            };

        Piece::getTexturePart(vertices, 0);
        mesh.setVertices(vertices);
        RookQuad = GPU::gpu.loadMesh(mesh);

        Piece::getTexturePart(vertices, 1);
        mesh.setVertices(vertices);
        QueenQuad = GPU::gpu.loadMesh(mesh);

        Piece::getTexturePart(vertices, 4);
        mesh.setVertices(vertices);
        BishopQuad = GPU::gpu.loadMesh(mesh);

        Piece::getTexturePart(vertices, 5);
        mesh.setVertices(vertices);
        KnightQuad = GPU::gpu.loadMesh(mesh);

        button_program.create("../Engine/openGL/Program/Shaders/2d/basic.vert", "../chess/resources/button.frag");
    }

    static void determinate()
    {
        button_program.destroy();
    }
};