//
// Created by tobi on 10/29/25.
//

#ifndef BETTERCHESS_MOVELOGIC_H
#define BETTERCHESS_MOVELOGIC_H

#include "../board/Board.h"
#include "../pieces/Piece.h"

struct Move {
    int from;
    int to;
    Piece* movedPiece;
    Piece* capturedPiece;
    bool isPromotion = false;
    Piece::TYPE promotionType;
    bool isCastle = false;
    bool isEnPassant = false;

    char prevCastlingRights = 0;
};

class MoveLogic {
public:

    /**
    * Performs a single move on a board object, handles any type of move but does not check
    * the legality of the move
    * @param board
    * @param move
    */
    static void performMove(Board& board, Move& move);

    static void undoLastMove(Board& board);

private:
    static std::vector<Move> _moveHistory;

    static void performPromotion(Board& board, const Move& move);
    static void performCastling(Board& board, const Move& move);
    static void performEnPassant(Board& board, const Move& move);

    static void undoPromotion(Board& board, const Move& move);
    static void undoCastling(Board& board, const Move& move);
    static void undoEnPassant(Board& board, const Move& move);

    static void updateCastlingRights(Board& board, const Move& move);
};


#endif //BETTERCHESS_MOVELOGIC_H