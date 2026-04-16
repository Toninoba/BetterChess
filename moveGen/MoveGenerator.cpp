//
// Created by Tobi on 14.12.2025.
//

#include "MoveGenerator.h"



MoveGenerator::bitboard MoveGenerator::pseudoKnightBitboard(BitboardData& bitboards) {

    bitboard attacks = 0;

    attacks |= (bitboards.whiteKnights & ~BitboardData::AFILE) << 17;
    attacks |= (bitboards.whiteKnights & ~BitboardData::HFILE) << 15;
    attacks |= (bitboards.whiteKnights & ~BitboardData::ABFILE) << 10;
    attacks |= (bitboards.whiteKnights & ~BitboardData::GHFILE) << 6;

    attacks |= (bitboards.whiteKnights & ~BitboardData::AFILE) >> 15;
    attacks |= (bitboards.whiteKnights & ~BitboardData::HFILE) >> 17;
    attacks |= (bitboards.whiteKnights & ~BitboardData::ABFILE) >> 6;
    attacks |= (bitboards.whiteKnights & ~BitboardData::GHFILE) >> 10;

    return attacks;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(Board &board) {

    std::vector<Move> pseudoLegalMoves(20);

    // Start generating pseudo legal moves

    return pseudoLegalMoves;
}
