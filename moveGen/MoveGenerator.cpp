//
// Created by Tobi on 14.12.2025.
//

#include "MoveGenerator.h"


MoveGenerator::bitboard MoveGenerator::pseudoKnightBitboard(BitboardData &bitboards) {
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

    std::vector<Piece> &pieces = board.getPieceList(board.getTurnToMove());

    // Start generating pseudo legal moves
    for (Piece &piece: pieces) {
        generateMovesPiece(board, piece, pseudoLegalMoves);
    }

    for (Move &m: pseudoLegalMoves) {
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
            } else {
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
                    Piece *capturedPiece = board.getPieceFromList(newPos);
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
            pseudoMoves.emplace_back(Move{piece.getPosition(), newPos, &piece});

            // Update position
            newPos += dir;
        }
    }
}

void MoveGenerator::generateMovesKnight(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {
    static constexpr int directions[] = {19, 21, -19, -21, 12, -12, 8, -8};

    for (int dir: directions) {
        int newPos = piece.getPosition() + dir;
        if (board[newPos] == Board::OUTSIDE) {
            continue;
        }

        if (board[newPos] != Board::EMPTY) {
            if (sgn(board[newPos]) != piece.getColor()) {
                Piece *capturedPiece = board.getPieceFromList(newPos);
                // Test for nullptr
#ifndef NDEBUG
                if (capturedPiece == nullptr) {
                    throw std::invalid_argument("Captured Piece in sliding move generation is a nullptr");
                }
#endif

                pseudoMoves.emplace_back(piece.getPosition(), newPos, &piece, capturedPiece);
            }
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), newPos, &piece});
        }
    }
}

void MoveGenerator::generateMovesKing(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {
    static constexpr int directions[] = {9, 11, -9, -11, 1, -1, 10, -10};

    // check normal moves
    for (int dir : directions) {
        int nextPos = piece.getPosition() + dir;

        if (board[nextPos] == Board::EMPTY) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), nextPos, &piece});
        }
        if (board[nextPos] != Board::EMPTY && sgn(board[nextPos]) != piece.getColor()) {
            Piece *capturedPiece = board.getPieceFromList(nextPos);
            // Test for nullptr
#ifndef NDEBUG
            if (capturedPiece == nullptr) {
                throw std::invalid_argument("Captured Piece in sliding move generation is a nullptr");
            }
#endif

            pseudoMoves.emplace_back(piece.getPosition(), nextPos, &piece, capturedPiece);
        }
    }

    // check for castling
    if (board.canCastleKingSide(piece.getColor())) {
        // Check for empty squares on white side
        if (piece.getColor() == Piece::WHITE && board[F1] == Board::EMPTY && board[G1] == Board::EMPTY) {
            pseudoMoves.emplace_back(piece.getPosition(), G1, &piece, nullptr, false, Piece::KING, true);
        }
        // Check for empty squares on black side
        else if (piece.getColor() == Piece::BLACK && board[F8] == Board::EMPTY && board[G8] == Board::EMPTY) {
            pseudoMoves.emplace_back(piece.getPosition(), G8, &piece, nullptr, false, Piece::KING, true);
        }

    }

    if (board.canCastQueenSide(piece.getColor())) {
        // Check for empty squares on white side
        if (piece.getColor() == Piece::WHITE && board[B1] == Board::EMPTY && board[C1] == Board::EMPTY && board[D1] == Board::EMPTY) {
            pseudoMoves.emplace_back(piece.getPosition(), C1, &piece, nullptr, false, Piece::KING, true);
        }
        // Check for empty squares on black side
        else if (piece.getColor() == Piece::BLACK && board[B8] == Board::EMPTY && board[C8] == Board::EMPTY && board[D8] == Board::EMPTY) {
            pseudoMoves.emplace_back(piece.getPosition(), C8, &piece, nullptr, false, Piece::KING, true);
        }
    }

}

void MoveGenerator::generateMovesPawn(Board &board, Piece &piece, std::vector<Move> &pseudoMoves) {
    // Differentiate between black and white (white moves 1 square up, black -1 squares)
    int moveDirection = piece.getColor() * 10;
    int attackPos1 = piece.getPosition() + moveDirection + 1;
    int attackPos2 = piece.getPosition() + moveDirection - 1;
    int movePos = piece.getPosition() + moveDirection;

    // first check for space infront

    if (board[movePos] == Board::EMPTY) {
        // Check for double pawn moves
        // TODO simplify expression
        if (((piece.getPosition() > 30 && piece.getPosition() < 39 && piece.getColor() == Piece::WHITE) ||
             (piece.getPosition() > 80 && piece.getPosition() < 89 && piece.getColor() == Piece::BLACK)) &&
            board[movePos + moveDirection] == Board::EMPTY) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos + moveDirection, &piece});
        }

        // Check for Pawn conversions

        if (movePos / 20 == 1 || movePos / 90 == 1) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::BISHOP});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::ROOK});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::KNIGHT});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::QUEEN});
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece});
        }
    }

    // check for capturing pieces for each attack position


    if (board[attackPos1] != Board::EMPTY && sgn(board[attackPos1]) != piece.getColor()) {
        Piece *capturedPiece = board.getPieceFromList(attackPos1);
        // Test for nullptr
#ifndef NDEBUG
        if (capturedPiece == nullptr) {
            throw std::invalid_argument("Captured Piece in pawn move generation attackpos1 is a nullptr");
        }
#endif


        // check for capturing pawn conversions
        if (movePos / 20 == 1 || movePos / 90 == 1) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::BISHOP});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::ROOK});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::KNIGHT});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::QUEEN});
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece});
        }
    }

    if (board[attackPos2] != Board::EMPTY && sgn(board[attackPos2]) != piece.getColor()) {
        Piece *capturedPiece = board.getPieceFromList(attackPos2);
        // Test for nullptr
#ifndef NDEBUG
        if (capturedPiece == nullptr) {
            throw std::invalid_argument("Captured Piece in pawn move generation attackpos2 is a nullptr");
        }
#endif

        if (movePos / 20 == 1 || movePos / 90 == 1) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos2, &piece, capturedPiece, true, Piece::BISHOP});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos2, &piece, capturedPiece, true, Piece::ROOK});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos2, &piece, capturedPiece, true, Piece::KNIGHT});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos2, &piece, capturedPiece, true, Piece::QUEEN});
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos2, &piece, capturedPiece});
        }
    }

    // check for en passant
    if ((board.getEnPassant() == piece.getPosition() - 1 ||
         board.getEnPassant() == piece.getPosition() + 1) &&
        sgn(board[board.getEnPassant()]) != piece.getColor()) {
        int moveTo = board.getEnPassant() + (-10 * sgn(board[board.getEnPassant()]));
        Piece *capturedPiece = board.getPieceFromList(board.getEnPassant());
        // Test for nullptr
#ifndef NDEBUG
        if (capturedPiece == nullptr) {
            throw std::invalid_argument("Captured Piece in pawn move generation attackpos2 is a nullptr");
        }
#endif


        pseudoMoves.emplace_back(piece.getPosition(), moveTo, &piece, capturedPiece, false, Piece::KING, false, true);
    }
}
