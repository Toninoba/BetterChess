#include "FenParser.h"
#include "gameloop.h"
#include "bitboards/BitboardData.h"

int main() {
    //start2PlayerGame();

    Board board;
    std::string fen = "rnbqk1nr/pppp1ppp/8/3p4/1b1P2P1/5N2/PPP1PP1P/RNBQKB1R b KQkq - 0 1";
    FenParser::parseFen(board, fen);

    BitboardData bitboards(board);

    return 0;
}
