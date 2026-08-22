//
// Created by Tobi on 25.06.2026.
//

#include "ChessBot.h"

#include "MoveGenerator.h"
#include "MoveLogic.h"
#include "SDL3/SDL_log.h"


Move ChessBot::getBestMove() const {

    int bestScore = std::numeric_limits<int>::min();
    SDL_Log("Bestscore %d", bestScore);
    Move bestMove;

    // Generate Moves for depth of 1 so we can extract best move here
    for (std::vector<Move> moves = MoveGenerator::generateLegalMoves(_board); Move& move : moves) {
        SDL_Log("from: %d, to: %d", move.from, move.to);
        MoveLogic::performMove(_board, move);

        int score = -search(_minSearchDepth - 1);
        SDL_Log("Score: %d   BestScore: %d", score, bestScore);
        if (score > bestScore) {
            SDL_Log("Updating Best Move");
            bestScore = score;
            bestMove = move;
        }

        MoveLogic::undoLastMove(_board);
    }

    return bestMove;
}

int ChessBot::search(int depth) const {

    if (depth <= 0) {
        return evaluate();
    }

    int bestScore = -std::numeric_limits<int>::infinity();

    for (std::vector<Move> moves = MoveGenerator::generateLegalMoves(_board); Move& move : moves) {

        MoveLogic::performMove(_board, move);

        int score = -search(depth - 1);
        SDL_Log("Score: %d", score);

        if (score > bestScore) {
            bestScore = score;

        }

        MoveLogic::undoLastMove(_board);
    }
    return bestScore;
}

int ChessBot::evaluate() const {

    SDL_Log("Starting evaluation");

    int whiteScore = 0;
    int blackScore = 0;

    auto whitePieces = _board.getPieceList(Piece::WHITE);
    auto blackPieces = _board.getPieceList(Piece::BLACK);

    for (Piece& piece: whitePieces) {
        if (piece.isAlive()) {
            whiteScore += getValueOfPiece(piece.getType());
        }
    }

    SDL_Log("White Score: %d", whiteScore);

    for (Piece& piece: blackPieces) {
        if (piece.isAlive()) {
            blackScore += getValueOfPiece(piece.getType());
        }
    }

    SDL_Log("Black Score: %d", blackScore);

    return _board.getTurnToMove() == Piece::WHITE ? whiteScore - blackScore : blackScore - whiteScore;

}

int ChessBot::getValueOfPiece(int type) {
    switch (type) {
        case Piece::PAWN:
            return 1;
        case Piece::KNIGHT:
            return 3;
        case Piece::BISHOP:
            return 3;
        case Piece::ROOK:
            return 5;
        case Piece::QUEEN:
            return 9;
        case Piece::KING:
            return 12;
        default:
            throw std::invalid_argument("Couldnt resolve piece type in evaluation");
    }
}
