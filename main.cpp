#include <iostream>

#include "Board.h"
#include "FenParser.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    Board board;

    std::string fen = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";

    FenParser::parseFen(board, fen);

    board.addPiece({Piece::PAWN, Piece::BLACK, A3});
    board.addPiece({Piece::BISHOP, Piece::BLACK, A3});
    board.addPiece({Piece::BISHOP, Piece::BLACK, A5});

    board.removePiece({Piece::BISHOP, Piece::BLACK, A5});


    return 0;
}
