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

    std::vector<Move> pseudoLegalMoves;
    pseudoLegalMoves.reserve(20);

    //Determine which sides moves to be generated

    std::vector<Piece>& pieces = board.getPieceList(board.getTurnToMove());

    // Start generating pseudo legal moves
    for (Piece& piece : pieces) {
        generateMovesPiece(board, piece, pseudoLegalMoves);
    }

    for (Move& m : pseudoLegalMoves) {
        std::cout << m.from << " " << m.to << std::endl;
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
        const int dir = directions[i];
        int newPos = piece.getPosition() + dir;
        // Iterate over every square that the piece can go in the given direction
        while (board[newPos] != Board::OUTSIDE) {
            
            // Check for obstructing pieces
            if (board[newPos] != Board::EMPTY) {
                
                if (sgn(board[newPos]) != piece.getColor()) {
                    Piece* capturedPiece = board.getPieceFromList(newPos);
                    // Test for nullptr
                    #ifndef NDEBUG
                    if (capturedPiece == nullptr) {

                        throw std::invalid_argument("Captured Piece in sliding move generation is a nullptr");
                    }
                    #endif

                    pseudoMoves.emplace_back(piece.getPosition(), newPos, &piece, capturedPiece);
                }
                
                // stop direction and goto next since piece is blocking regardless of color
                break;
            }
            
            // Square is empty and we can continue
            pseudoMoves.emplace_back(Move{piece.getPosition(),newPos, &piece});
            
            // Update position
            newPos += dir;
        }

    }

}

void MoveGenerator::generateMovesKnight(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {

}

void MoveGenerator::generateMovesKing(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {

}

void MoveGenerator::generateMovesPawn(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {

}
