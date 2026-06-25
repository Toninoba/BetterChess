//
// Created by Tobi on 25.06.2026.
//

#include "FirstBot.h"

#include "MoveGenerator.h"


Move FirstBot::getBestMove(Board &board) {
    auto moves = MoveGenerator::generateLegalMoves(board);
    if (!moves.empty()) {
        return moves.front();
    }
    return Move(-1,-1);
}
