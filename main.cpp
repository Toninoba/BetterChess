#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"
#include "moveGen/MoveGenerator.h"
#include "moveGen/MoveLogic.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "8/8/5n1B/8/2r5/2R1b3/3P2p1/8 b - - 0 1";
    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;

    MoveGenerator::generatePseudoLegalMoves(board);

    return 0;
}
