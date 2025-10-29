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
    Piece::TYPE type;
};

class MoveLogic {
public:
    void performMove(Board& board, const Move& move) const {

    }

    void undoMove(Board& board, const Move& move) const {

    }


private:
    std::vector<Move> _moveStack;
};


#endif //BETTERCHESS_MOVELOGIC_H