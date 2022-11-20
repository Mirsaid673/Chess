#include "logic.h"
#include "desk.h"
#include <algorithm>
#include "ui.h"

Desk *Logic::desk;
Piece *Logic::last_moved_piece = nullptr;
Piece *Logic::turning_pawn;
int8_t Logic::moving_color = 1;
int8_t Logic::pawn_to_turn = Piece::Type::QUEEN_W;
bool Logic::pause = false;

void Logic::onPawnTurn(Piece &piece)
{
    UI::setButtonsColor((Piece::Color)moving_color);
    UI::showButtons();
    turning_pawn = &piece;
    pause = true;
    // piece.setTypeNoColor((Piece::Type)pawn_to_turn);
}

void Logic::onButtonPress()
{
    turning_pawn->setTypeNoColor((Piece::Type)pawn_to_turn);
    UI::hideButtons();
    pause = false;
}

void onPawnMove(Piece &piece, int new_y)
{
    int side = (piece.getColor() + 1) / 2 * 7; // translating from [-1, 1] to [0, 7]
    if (new_y == side)
        Logic::onPawnTurn(piece);
}

bool canPawnMove(Piece &piece, const glm::ivec2 &new_pos)
{
    const auto it = std::find(piece.available_cells.coords.begin(), piece.available_cells.coords.end(), new_pos);
    if (it == piece.available_cells.coords.end())
        return false;

    piece.first_move_large = false;

    glm::ivec2 offset = new_pos - piece.getLastPos();
    if (offset.x != 0 && Logic::desk->getPieceColor(new_pos) == Piece::Color::EMPTY)
    {
        glm::ivec2 en_passnat_pos = new_pos - glm::ivec2(0, piece.getColor());
        Logic::desk->removePiece(en_passnat_pos);
    }
    else if (glm::abs(offset.y) == 2)
        piece.first_move_large = true;

    onPawnMove(piece, new_pos.y);
    return true;
}

bool canKingMove(Piece &piece, const glm::ivec2 &new_pos)
{
    const auto it = std::find(piece.available_cells.coords.begin(), piece.available_cells.coords.end(), new_pos);
    if (it == piece.available_cells.coords.end())
        return false;
    int x_offset = new_pos.x - piece.getLastPos().x;
    if (x_offset == 2)
    {
        glm::ivec2 rook_pos = glm::ivec2(7, new_pos.y);
        glm::ivec2 rook_new_pos = new_pos - glm::ivec2(1, 0);
        Logic::desk->move(rook_pos, rook_new_pos);
    }
    else if (x_offset == -2)
    {
        glm::ivec2 rook_pos = glm::ivec2(0, new_pos.y);
        glm::ivec2 rook_new_pos = new_pos + glm::ivec2(1, 0);
        Logic::desk->move(rook_pos, rook_new_pos);
    }

    Logic::desk->setKingPos(new_pos, piece.getColor());
    return true;
}

bool Logic::canMove(Piece &piece, const glm::ivec2 &new_pos)
{
    Piece::Type t = piece.getTypeNoColor();
    glm::ivec2 offset = new_pos - piece.getLastPos();

    if (offset == glm::ivec2(0) || new_pos.x < 0 || new_pos.x > 7 || new_pos.y < 0 || new_pos.y > 7)
        return false;
    if (Logic::desk->getPieceColor(new_pos) == piece.getColor())
        return false;

    if (t == Piece::Type::PAWN_W)
    {
        return canPawnMove(piece, new_pos);
    }
    else if (t == Piece::Type::KING_W)
    {
        return canKingMove(piece, new_pos);
    }
    else
    {
        const auto it = std::find(piece.available_cells.coords.begin(), piece.available_cells.coords.end(), new_pos);
        return it != piece.available_cells.coords.end();
    }
}

bool isInside(const glm::ivec2 &pos)
{
    return pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7;
}

bool isCellAttaced(const glm::ivec2 &coord, Piece::Color attacing_color)
{
    const static glm::ivec2 diagonals[4] = {glm::ivec2(1, 1), glm::ivec2(-1, -1), glm::ivec2(-1, 1), glm::ivec2(1, -1)};
    for (const auto &dir : diagonals)
    {
        glm::ivec2 pos = coord + dir;
        while (isInside(pos) && Logic::desk->getPieceColor(pos) != -attacing_color)
        {
            Piece::Type piece_type = Logic::desk->getPiece(pos).getTypeNoColor();
            Piece::Color piece_color = Logic::desk->getPiece(pos).getColor();
            if (piece_type == Piece::Type::BISHOP_W || piece_type == Piece::Type::QUEEN_W)
                return true;
            else if (piece_color == attacing_color)
                break;
            pos += dir;
        }
    }

    const static glm::ivec2 lines[4] = {glm::ivec2(0, 1), glm::ivec2(0, -1), glm::ivec2(1, 0), glm::ivec2(-1, 0)};
    for (const auto &dir : lines)
    {
        glm::ivec2 pos = coord + dir;
        while (isInside(pos) && Logic::desk->getPieceColor(pos) != -attacing_color)
        {
            Piece::Type piece_type = Logic::desk->getPiece(pos).getTypeNoColor();
            Piece::Color piece_color = Logic::desk->getPiece(pos).getColor();
            if (piece_type == Piece::Type::ROOK_W || piece_type == Piece::Type::QUEEN_W)
                return true;
            else if (piece_color == attacing_color)
                break;
            pos += dir;
        }
    }

    const static glm::ivec2 knight_moves[8] = {glm::ivec2(2, 1), glm::ivec2(1, 2), glm::ivec2(-2, 1), glm::ivec2(-1, 2), glm::ivec2(2, -1), glm::ivec2(1, -2), glm::ivec2(-2, -1), glm::ivec2(-1, -2)};
    for (const auto &dir : knight_moves)
    {
        glm::ivec2 pos = coord + dir;
        if (isInside(pos))
            if (Logic::desk->getPiece(pos).getType() == Piece::Type::KNIGHT_W * attacing_color)
                return true;
    }

    const static glm::ivec2 king_moves[8] = {glm::ivec2(0, 1), glm::ivec2(0, -1), glm::ivec2(1, 0), glm::ivec2(-1, 0), glm::ivec2(1, 1), glm::ivec2(-1, -1), glm::ivec2(-1, 1), glm::ivec2(1, -1)};
    for (const auto &dir : king_moves)
    {
        glm::ivec2 pos = coord + dir;
        if (isInside(pos))
            if (Logic::desk->getPiece(pos).getType() == Piece::Type::KING_W * attacing_color)
                return true;
    }

    const glm::ivec2 pawn_moves[2] = {glm::ivec2(1, -attacing_color), glm::ivec2(-1, -attacing_color)};
    for (const auto &dir : pawn_moves)
    {
        glm::ivec2 pos = coord + dir;
        if (isInside(pos))
            if (Logic::desk->getPiece(pos).getType() == Piece::Type::PAWN_W * attacing_color)
                return true;
    }

    return false;
}
bool isCheck(Piece::Color color = (Piece::Color)Logic::moving_color)
{
    return isCellAttaced(Logic::desk->getKingPos(color), (Piece::Color)-color);
}
bool isCheckAfertMove(const glm::ivec2 &p1, const glm::ivec2 &p2)
{
    Logic::desk->falseMove(p1, p2);

    bool is_check = isCheck();

    Logic::desk->returnFalseMove(p1, p2);

    return is_check;
}

bool isCheckAfertMoveKingMove(const glm::ivec2 p1, const glm::ivec2 p2)
{
    Logic::desk->falseMove(p1, p2);

    glm::ivec2 king_tmp = Logic::desk->getKingPos((Piece::Color)Logic::moving_color);
    Logic::desk->setKingPos(p2, (Piece::Color)Logic::moving_color);

    bool is_check = isCheck();

    Logic::desk->setKingPos(king_tmp, (Piece::Color)Logic::moving_color);
    Logic::desk->returnFalseMove(p1, p2);

    return is_check;
}

void calculateMovesForPawn(Piece &piece)
{
    int this_color = piece.getColor();
    glm::ivec2 last_pos = piece.getLastPos();

    glm::ivec2 new_pos1 = last_pos + glm::ivec2(0, this_color);
    glm::ivec2 new_pos2 = last_pos + glm::ivec2(0, this_color * 2);

    glm::ivec2 new_pos3 = last_pos + glm::ivec2(1, this_color);
    glm::ivec2 new_pos4 = last_pos + glm::ivec2(-1, this_color);

    if (Logic::desk->getPieceColor(new_pos1) == Piece::Color::EMPTY)
    {
        if (not isCheckAfertMove(last_pos, new_pos1))
            piece.available_cells.push_back(new_pos1);

        if (!piece.isMoved())
            if (Logic::desk->getPieceColor(new_pos2) == Piece::Color::EMPTY && not isCheckAfertMove(last_pos, new_pos2))
                piece.available_cells.push_back(new_pos2);
    }
    if (isInside(new_pos3))
    {
        if (Logic::desk->getPieceColor(new_pos3) == -this_color && not isCheckAfertMove(last_pos, new_pos3))
            piece.available_cells.push_back(new_pos3);
        else
        {
            Piece &another = Logic::desk->getPiece(new_pos3 - glm::ivec2(0, piece.getColor()));
            if (Logic::desk->getPieceColor(new_pos3) == Piece::Color::EMPTY && another.first_move_large &&
                Logic::last_moved_piece == &another && not isCheckAfertMove(last_pos, new_pos3))
            {
                piece.available_cells.push_back(new_pos3);
            }
        }
    }
    if (isInside(new_pos4))
    {
        if (Logic::desk->getPieceColor(new_pos4) == -this_color && not isCheckAfertMove(last_pos, new_pos4))
            piece.available_cells.push_back(new_pos4);
        else
        {
            Piece &another = Logic::desk->getPiece(new_pos4 - glm::ivec2(0, piece.getColor()));
            if (Logic::desk->getPieceColor(new_pos4) == Piece::Color::EMPTY && another.first_move_large &&
                Logic::last_moved_piece == &another && not isCheckAfertMove(last_pos, new_pos4))
            {
                piece.available_cells.push_back(new_pos4);
            }
        }
    }
}

void calculateMovesForBisop(Piece &piece)
{
    const static glm::ivec2 directions[4] =
        {
            glm::ivec2(1, 1),
            glm::ivec2(-1, -1),
            glm::ivec2(-1, 1),
            glm::ivec2(1, -1),
        };
    glm::ivec2 last_pos = piece.getLastPos();
    for (const auto &dir : directions)
    {
        glm::ivec2 pos = last_pos + dir;
        while (isInside(pos) && Logic::desk->getPieceColor(pos) != piece.getColor())
        {
            if (Logic::desk->getPieceColor(pos) != Piece::Color::EMPTY)
            {
                if (not isCheckAfertMove(last_pos, pos))
                    piece.available_cells.push_back(pos);
                break;
            }
            if (not isCheckAfertMove(last_pos, pos))
                piece.available_cells.push_back(pos);
            pos += dir;
        }
    }
}

void calculateMovesForKnight(Piece &piece)
{
    const static glm::ivec2 directions[8] =
        {
            glm::ivec2(2, 1),
            glm::ivec2(1, 2),

            glm::ivec2(-2, 1),
            glm::ivec2(-1, 2),

            glm::ivec2(2, -1),
            glm::ivec2(1, -2),

            glm::ivec2(-2, -1),
            glm::ivec2(-1, -2),
        };
    glm::ivec2 last_pos = piece.getLastPos();
    for (const auto &dir : directions)
    {
        glm::ivec2 pos = last_pos + dir;
        if (isInside(pos))
            if (Logic::desk->getPieceColor(pos) != piece.getColor() && not isCheckAfertMove(last_pos, pos))
                piece.available_cells.push_back(pos);
    }
}

void calculateMovesForRook(Piece &piece)
{
    const static glm::ivec2 directions[4] =
        {
            glm::ivec2(0, 1),
            glm::ivec2(0, -1),
            glm::ivec2(1, 0),
            glm::ivec2(-1, 0),
        };
    glm::ivec2 last_pos = piece.getLastPos();
    for (const auto &dir : directions)
    {
        glm::ivec2 pos = last_pos + dir;
        while (isInside(pos) && Logic::desk->getPieceColor(pos) != piece.getColor())
        {
            if (Logic::desk->getPieceColor(pos) != Piece::Color::EMPTY)
            {
                if (not isCheckAfertMove(last_pos, pos))
                    piece.available_cells.push_back(pos);
                break;
            }
            if (not isCheckAfertMove(last_pos, pos))
                piece.available_cells.push_back(pos);
            pos += dir;
        }
    }
}

void calculateMovesForQueen(Piece &piece)
{
    const static glm::ivec2 directions[8] =
        {
            glm::ivec2(0, 1),
            glm::ivec2(0, -1),
            glm::ivec2(1, 0),
            glm::ivec2(-1, 0),
            glm::ivec2(1, 1),
            glm::ivec2(-1, -1),
            glm::ivec2(-1, 1),
            glm::ivec2(1, -1),
        };
    glm::ivec2 last_pos = piece.getLastPos();
    for (const auto &dir : directions)
    {
        glm::ivec2 pos = last_pos + dir;
        while (isInside(pos) && Logic::desk->getPieceColor(pos) != piece.getColor())
        {
            if (Logic::desk->getPieceColor(pos) != Piece::Color::EMPTY)
            {
                if (not isCheckAfertMove(last_pos, pos))
                    piece.available_cells.push_back(pos);
                break;
            }
            if (not isCheckAfertMove(last_pos, pos))
                piece.available_cells.push_back(pos);
            pos += dir;
        }
    }
}

void calculateMovesForKing(Piece &piece)
{
    const static glm::ivec2 directions[8] =
        {
            glm::ivec2(0, 1),
            glm::ivec2(0, -1),
            glm::ivec2(1, 0),
            glm::ivec2(-1, 0),
            glm::ivec2(1, 1),
            glm::ivec2(-1, -1),
            glm::ivec2(-1, 1),
            glm::ivec2(1, -1),
        };

    glm::ivec2 last_pos = piece.getLastPos();
    for (const auto &dir : directions)
    {
        glm::ivec2 pos = last_pos + dir;
        if (isInside(pos))
            if (Logic::desk->getPieceColor(pos) != piece.getColor() && not isCheckAfertMoveKingMove(last_pos, pos))
                piece.available_cells.push_back(pos);
    }
    // castling
    const static glm::ivec2 castle_dirs[2] =
        {
            glm::ivec2(2, 0),
            glm::ivec2(-2, 0),
        };
    if (not piece.isMoved() && not isCheck())
    {
        int king_y = last_pos.y;
        for (const auto &dir : castle_dirs)
        {
            int rook_x = (dir.x + 2) / 4 * 7; // translating from [-2, 2] to [0, 7]
            Piece &rook = Logic::desk->getPiece(glm::ivec2(rook_x, king_y));
            if (rook.isMoved() || rook.getTypeNoColor() != Piece::Type::ROOK_W)
            {
                continue;
            }
            // checking if there is nothing between king and rook
            bool braked = false;
            int sign = dir.x / 2;
            for (int x = last_pos.x + sign; x != rook_x; x += sign)
            {
                if (Logic::desk->getPieceColor(glm::ivec2(x, king_y)) != Piece::Color::EMPTY)
                {
                    braked = true;
                    break;
                }
            }
            // checking if cells are not attacked
            glm::ivec2 king_new_pos = last_pos + dir;
            for (int x = last_pos.x + sign; x != king_new_pos.x; x += sign)
            {
                if (isCellAttaced(glm::ivec2(x, king_y), (Piece::Color)-piece.getColor()))
                {
                    braked = true;
                    break;
                }
            }
            if (braked)
                continue;

            piece.available_cells.push_back(king_new_pos);
        }
    }
}

void (*calculateFuncs[8])(Piece &) = {
    nullptr,
    calculateMovesForKing,
    calculateMovesForQueen,
    calculateMovesForRook,
    calculateMovesForKnight,
    calculateMovesForBisop,
    calculateMovesForPawn,
};

void Logic::calculateAvailableMoves(Piece &piece)
{
    Piece::Type t = piece.getTypeNoColor();
    piece.available_cells.clear();
    calculateFuncs[t](piece);
}

void Logic::calculateAvailableMoves(int8_t color)
{
    int available_moves_count = 0;
    for (auto &piece : desk->pieces_manager.pieces)
    {
        if (piece->getColor() == color)
        {
            calculateAvailableMoves(*piece);
            available_moves_count += piece->available_cells.coords.size();
        }
    }
    if (available_moves_count == 0)
    {
        if (isCheck())
            spdlog::info("game over win color: {}", -Logic::moving_color);
        else
            spdlog::info("draw");
    }
}