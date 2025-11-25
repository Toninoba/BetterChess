#include <iostream>

#include "Board.h"
#include "FenParser.h"
#include "moveGen/MoveLogic.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    Board board;

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;
    std::cout << board.getTurnToMove() << std::endl;
    std::cout << board.getPieceFromList(E2)->getPosition() << std::endl;

    Move move(E2, E3, board.getPieceFromList(E2), nullptr,
        false, false, Piece::PAWN, false, false);

    MoveLogic::performMove(board, move);

    std::cout << board << std::endl;
    std::cout << board.getTurnToMove() << std::endl;
    std::cout << board.getPieceFromList(E3)->getPosition() << std::endl;



    return 0;
}
