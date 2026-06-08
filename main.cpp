#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"
#include "moveGen/MoveGenerator.h"
#include "moveGen/MoveLogic.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "1k5r/4r3/8/8/8/3B1n2/8/R3K2R w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;

    MoveGenerator::generateLegalMoves(board);

    return 0;
}
