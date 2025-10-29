#include <iostream>

#include "Board.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    Board board;

    board.addPiece({Piece::PAWN, Piece::BLACK, A3});
    board.addPiece({Piece::BISHOP, Piece::BLACK, A3});
    board.addPiece({Piece::BISHOP, Piece::BLACK, A5});

    board.removePiece({Piece::BISHOP, Piece::BLACK, A5});


    return 0;
}
