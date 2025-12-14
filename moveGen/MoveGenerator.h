//
// Created by Tobi on 14.12.2025.
//

#ifndef BETTERCHESS_MOVEGENERATOR_H
#define BETTERCHESS_MOVEGENERATOR_H
#include "../bitboards/BitboardData.h"


class MoveGenerator {
public:
    using bitboard = unsigned long long;

    bitboard pseudoKnightBitboard(BitboardData& bitboards);

};


#endif //BETTERCHESS_MOVEGENERATOR_H