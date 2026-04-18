#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"
#include "moveGen/MoveGenerator.h"
#include "moveGen/MoveLogic.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "8/8/8/2b5/8/8/8/2R4B w - - 0 1";
    FenParser::parseFen(board, fen);

    std::cout << board << std::endl;

    MoveGenerator::generatePseudoLegalMoves(board);

    return 0;
}
