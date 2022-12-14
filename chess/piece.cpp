#include "piece.h"
#include "desk.h"
#include "logic.h"

void Piece::onMouseDown(const glm::vec2 &mouse_world)
{
    showAvailableCells();
}
void Piece::onMouseHold(const glm::vec2 &mouse_world)
{
    if (input.getCursorOffset() != glm::ivec2(0))
        transform.origin = mouse_world;
}
void Piece::onMouseUp(const glm::vec2 &mouse_world)
{
    glm::ivec2 new_pos = glm::round(mouse_world);
    if (Logic::canMove(*this, new_pos))
    {
        moved = true;

        Logic::desk->move(glm::ivec2(last_pos), new_pos); // anonymus object because we are telling the reference to method

        hideAvailableCells();

        Logic::last_moved_piece = this;
        Logic::changeMovingColor();
        Logic::calculateAvailableMoves(Logic::moving_color);
    }
    else
    {
        transform.origin = last_pos;
    }
}

void Piece::showAvailableCells()
{
    Logic::desk->origin->addChild(available_cells);
}

void Piece::hideAvailableCells()
{
    Logic::desk->origin->removeChild(available_cells);
}

void Piece::setType(Type t)
{
    type = t;
    m.texture = white_texture_atlas;
    color = Color::WHITE;
    if (t < Piece::Type::NONE)
    {
        m.texture = black_texture_atlas;
        color = Color::BLACK;
        t = t * -1;
    }
    switch (t)
    {
    case Piece::Type::PAWN_W:
        quad = PawnQuad;
        return;
    case Piece::Type::BISHOP_W:
        quad = BishopQuad;
        return;
    case Piece::Type::KNIGHT_W:
        quad = KnightQuad;
        return;
    case Piece::Type::ROOK_W:
        quad = RookQuad;
        return;
    case Piece::Type::QUEEN_W:
        quad = QueenQuad;
        return;
    case Piece::Type::KING_W:
        quad = KingQuad;
        return;
    }
}
void Piece::setTypeNoColor(Type t)
{
    if (color == Piece::Color::WHITE)
        setType(t);
    else
        setType(t * -1);
}

Program Piece::pieces_program;

RID Piece::white_texture_atlas;
RID Piece::black_texture_atlas;

RID Piece::PawnQuad;
RID Piece::BishopQuad;
RID Piece::KnightQuad;
RID Piece::RookQuad;
RID Piece::QueenQuad;
RID Piece::KingQuad;

void Piece::getTexturePart(std::vector<Vertex2d> &vertices, int index)
{
    // 3 4 5
    // 0 1 2

    glm::vec2 verts[4] = {
        glm::vec2(0, 0),              // left bottom
        glm::vec2(0, 0.5f),           // left top
        glm::vec2(1.0f / 3.0f, 0),    // rigth bottom
        glm::vec2(1.0f / 3.0f, 0.5f), // rigth toop
    };

    glm::vec2 offset(1.0f / 3.0f, 0);
    if (index > 2)
        offset.y += 0.5f;

    for (auto &vert : verts)
        vert += glm::vec2(offset.x * (index % 3), offset.y);

    vertices[0].texture_st = verts[0];
    vertices[1].texture_st = verts[1];
    vertices[2].texture_st = verts[2];
    vertices[3].texture_st = verts[3];
}

void Piece::initialize(const char *white_texture, const char *black_texture)
{
    Mesh2d mesh;
    std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};
    mesh.setIndices(indices);
    std::vector<Vertex2d> vertices = {Vertex2d(glm::vec2(-.5, -.5f)),
                                      Vertex2d(glm::vec2(-.5f, .5f)),
                                      Vertex2d(glm::vec2(.5f, -.5f)),
                                      Vertex2d(glm::vec2(.5f, .5f))};
    getTexturePart(vertices, 0);
    mesh.setVertices(vertices);
    RookQuad = GPU::gpu.loadMesh(mesh);

    getTexturePart(vertices, 1);
    mesh.setVertices(vertices);
    QueenQuad = GPU::gpu.loadMesh(mesh);

    getTexturePart(vertices, 2);
    mesh.setVertices(vertices);
    KingQuad = GPU::gpu.loadMesh(mesh);

    getTexturePart(vertices, 3);
    mesh.setVertices(vertices);
    PawnQuad = GPU::gpu.loadMesh(mesh);

    getTexturePart(vertices, 4);
    mesh.setVertices(vertices);
    BishopQuad = GPU::gpu.loadMesh(mesh);

    getTexturePart(vertices, 5);
    mesh.setVertices(vertices);
    KnightQuad = GPU::gpu.loadMesh(mesh);

    white_texture_atlas = GPU::gpu.createTexture(white_texture);
    black_texture_atlas = GPU::gpu.createTexture(black_texture);

    pieces_program.create("../Engine/openGL/Program/Shaders/2d/basic.vert", "../chess/resources/piece.frag");

    mesh.setVertices({
        Vertex2d(glm::vec2(-0.5f, -0.5f), glm::vec2(0, 0)), // left bottom
        Vertex2d(glm::vec2(-.5f, .5f), glm::vec2(0, 1)),    // left top
        Vertex2d(glm::vec2(.5f, -.5f), glm::vec2(1, 0)),    // right bottom
        Vertex2d(glm::vec2(.5f, .5f), glm::vec2(1, 1)),     // right top
    });
    AvailableCells::quad = GPU::gpu.loadMesh(mesh);
    AvailableCells::material.program = &pieces_program;
    AvailableCells::material.texture = GPU::gpu.createTexture("../chess/resources/available.png");
}

void Piece::determinate()
{
    pieces_program.destroy();
}