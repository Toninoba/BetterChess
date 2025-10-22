//
// Created by tobi on 10/22/25.
//

#ifndef BETTERCHESS_BOARD_H
#define BETTERCHESS_BOARD_H
#include <array>
#include <vector>

#include "../pieces/Piece.h"


class Board {
public:

    Board() {

    }



private:

    int _board[120];
    std::vector<Piece> _whitePieces;
    std::vector<Piece> _blackPieces;

};


#endif //BETTERCHESS_BOARD_H