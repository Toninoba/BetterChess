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

    //Determine which sides moves to be generated

    std::vector<Piece>& pieces = board.getPieceList(board.getTurnToMove());

    // Start generating pseudo legal moves
    for (Piece& piece : pieces) {
        generateMovesPiece(board, piece, pseudoLegalMoves);
    }



    return pseudoLegalMoves;
}

void MoveGenerator::generateMovesPiece(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {
    switch (piece.getType()) {
        case Piece::PAWN:
            generateMovesPawn(board, piece, pseudoMoves);
            break;
        case Piece::KNIGHT:
            generateMovesKnight(board, piece, pseudoMoves);
            break;
        case Piece::KING:
            generateMovesKing(board, piece, pseudoMoves);
            break;
        default:
            if (piece.getType() == Piece::ROOK || piece.getType() == Piece::BISHOP || piece.getType() == Piece::QUEEN) {
                generateMovesSliding(board, piece, pseudoMoves);
            }
            else {
                throw std::invalid_argument("Invalid Piece for Generation");
            }


    }
}

void MoveGenerator::generateMovesSliding(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {
    static constexpr int directions[] = {9, 11, -9, -11, 1, -1, 10, -10};
    int offset = 0;
    int end = 4;

    // Set direction appropriate to piece type
    if (piece.getType() == Piece::ROOK) {
        offset = 4;
        end = 8;
    }
    if (piece.getType() == Piece::QUEEN) {
        end = 8;
    }

    // Iterate over every valid direction for the piece
    for (int i = offset; i < end; i++) {
        // Iterate over every square that the piece can go in the given direction

    }

}
