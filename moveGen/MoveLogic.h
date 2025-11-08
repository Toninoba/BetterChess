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
    bool isCapture = false;
    bool isPromotion = false;
    Piece::TYPE promotionType;
    bool isCastle = false;
    bool isEnPassant = false;
};

class MoveLogic {
public:
    static void performMove(Board& board, const Move& move) {

    }

    static void undoMove(Board& board, const Move& move) {

    }


private:
    static std::vector<Move> _moveHistory;
};


#endif //BETTERCHESS_MOVELOGIC_H