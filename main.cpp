#include "FenParser.h"
#include "gameloop.h"
#include "moveGen/MoveGenerator.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "1krr4/8/8/8/8/3B2n1/8/R3K2R w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;

    MoveGenerator::generateLegalMoves(board);

    std::cout << board << std::endl;

    return 0;
}
