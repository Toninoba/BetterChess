#include <iostream>

#include "Board.h"
#include "FenParser.h"
#include "moveGen/MoveLogic.h"


int main() {
    Board board;

    std::string fen = "8/8/8/8/8/8/8/R3K2R w KQkq - 0 1";

    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;
    std::cout << board.getTurnToMove() << std::endl;

    Move move(A1, A2, board.getPieceFromList(A1), nullptr, false, Piece::QUEEN, false, false);

    MoveLogic::performMove(board, move);

    std::cout << board << std::endl;
    std::cout << board.getTurnToMove() << std::endl;


    MoveLogic::undoLastMove(board);

    std::cout << board << std::endl;
    std::cout << board.getTurnToMove() << std::endl;







    return 0;
}
