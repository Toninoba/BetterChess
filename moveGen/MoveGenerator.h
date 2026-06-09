//
// Created by Tobi on 14.12.2025.
//

#ifndef BETTERCHESS_MOVEGENERATOR_H
#define BETTERCHESS_MOVEGENERATOR_H
#include "Move.h"
#include <iostream>
#include <vector>




class Board;

class MoveGenerator {
public:
    using bitboard = unsigned long long;



    /**
     * Generates all pseudo legal moves for the current players turn
     * @param board The board representation, for which all pseudo legal moves should be generated
     * @return a vector of all pseudo legal moves for player, who's turn it is
     */
    static std::vector<Move> generatePseudoLegalMoves(Board& board);

    static std::vector<Move> generateLegalMoves(Board& board);

    static void generateMovesPiece(Board& board, Piece& piece, std::vector<Move>& pseudoMoves);

    static void generateMovesPawn(Board& board, Piece& piece, std::vector<Move>& pseudoMoves);
    static void generateMovesKnight(Board& board, Piece& piece, std::vector<Move>& pseudoMoves);
    static void generateMovesKing(Board& board, Piece& piece, std::vector<Move>& pseudoMoves);
    static void generateMovesSliding(Board& board, Piece& piece, std::vector<Move>& pseudoMoves);

    static bool isKingChecked(Board& board, int color);
    static bool isSlidingAttacker(int piece, int dir);

    static bool isLegalCastle(Board& board, const Move& move);

    static bool isSquareAttacked(Board& board, int square, int color);

};


#endif //BETTERCHESS_MOVEGENERATOR_H