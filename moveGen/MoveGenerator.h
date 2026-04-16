//
// Created by Tobi on 14.12.2025.
//

#ifndef BETTERCHESS_MOVEGENERATOR_H
#define BETTERCHESS_MOVEGENERATOR_H
#include "../bitboards/BitboardData.h"
#include "Move.cpp"


class MoveGenerator {
public:
    using bitboard = unsigned long long;

    static bitboard pseudoKnightBitboard(BitboardData& bitboards);

    /**
     * Generates all pseudo legal moves for the current players turn
     * @param board The board representation, for which all pseudo legal moves should be generated
     * @return a vector of all pseudo legal moves for player, who's turn it is
     */
    static std::vector<Move> generatePseudoLegalMoves(Board& board);

};


#endif //BETTERCHESS_MOVEGENERATOR_H