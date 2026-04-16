#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"
#include "moveGen/MoveGenerator.h"
#include "moveGen/MoveLogic.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "8/8/8/8/8/2N5/8/8 b - - 0 1";
    FenParser::parseFen(board, fen);

    BitboardData bitboards(board);

    MoveGenerator::bitboard attack = MoveGenerator::pseudoKnightBitboard(bitboards);

    return 0;
}
