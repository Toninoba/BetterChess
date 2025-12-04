//
// Created by tobi on 04.12.25.
//

#ifndef BETTERCHESS_BITBOARDGEN_H
#define BETTERCHESS_BITBOARDGEN_H
#include "Board.h"


struct Move;

class BitboardData {

    using bitboard = unsigned long long;

public:

    BitboardData(Board& board);

    void performMove(const Move& m);

    void undoMove(const Move& m);

    bitboard whitePawns = 0;
    bitboard whiteKnights = 0;
    bitboard whiteBishops = 0;
    bitboard whiteRooks = 0;
    bitboard whiteQueens = 0;
    bitboard whiteKing = 0;

    bitboard blackPawns = 0;
    bitboard blackKnights = 0;
    bitboard blackBishops = 0;
    bitboard blackRooks = 0;
    bitboard blackQueens = 0;
    bitboard blackKing = 0;

    bitboard whitePieces = 0;
    bitboard blackPieces = 0;
    bitboard occupied = 0;



private:

    static int getBitboardPosition(int mailboxPos) ;

    void parsePieceTypeBitboard(const Piece& piece, int position);

};


#endif //BETTERCHESS_BITBOARDGEN_H