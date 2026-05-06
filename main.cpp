#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"
#include "moveGen/MoveGenerator.h"
#include "moveGen/MoveLogic.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "8/2p2P2/8/8/8/3p4/2PP1p2/8 b - - 0 1";
    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;

    MoveGenerator::generatePseudoLegalMoves(board);

    return 0;
}
