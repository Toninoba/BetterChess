//
// Created by Tobi on 25.06.2026.
//

#ifndef BETTERCHESS_FIRSTBOT_H
#define BETTERCHESS_FIRSTBOT_H
#include "Board.h"
#include "Move.h"


class ChessBot {
public:

    ChessBot(Board& board, int minSearchDepth) : _board(board), _minSearchDepth(minSearchDepth) {}

    Move getBestMove() const;

private:

    Board& _board;
    const int _minSearchDepth;



    [[nodiscard]] int search(int depth) const;

    [[nodiscard]] int evaluate() const;

    static int getValueOfPiece(int type) ;

};


#endif //BETTERCHESS_FIRSTBOT_H
