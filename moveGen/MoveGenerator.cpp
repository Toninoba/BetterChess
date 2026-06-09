//
// Created by Tobi on 14.12.2025.
//

#include "MoveGenerator.h"

#include "MoveLogic.h"

/*
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
*/

std::vector<Move> MoveGenerator::generateLegalMoves(Board &board) {

    std::vector<Move> moves = generatePseudoLegalMoves(board);

    // Now delete every non legal move (this is extremly slow because of O(n) or something deletion)
    // optimize by only adding the legal moves and not checking if they are legal

    // Extract turnToMove to determine which King should be checked for, since it changes after performing move
    const int turnToMove = board.getTurnToMove();

    moves.erase(
    std::remove_if(
        moves.begin(),
        moves.end(),
        [&](Move& m)
        {
            // differentiate between normal moves and castling
            if (m.isCastle) {
                return !isLegalCastle(board, m);
            }
            // perform move
            MoveLogic::performMove(board, m);
            // Check if king is in check
            bool illegal = isKingChecked(board, turnToMove);
            // undo Move
            MoveLogic::undoLastMove(board);

            return illegal; // remove this move
        }),
    moves.end());

    /*
    for (Move &m: moves) {
        std::cout << m.from << " " << m.to << std::endl;
    }
    */

    return moves;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(Board &board) {
    std::vector<Move> pseudoLegalMoves;
    pseudoLegalMoves.reserve(20);

    //Determine which sides moves to be generated

    std::vector<Piece> &pieces = board.getPieceList(board.getTurnToMove());

    // Start generating pseudo legal moves
    for (Piece &piece: pieces) {
        if (piece.isAlive()) {
            generateMovesPiece(board, piece, pseudoLegalMoves);
        }

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
                    throw std::invalid_argument("Captured Piece in Knight move generation is a nullptr");
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

        if (board[nextPos] == Board::OUTSIDE) {
            continue;
        }

        if (board[nextPos] == Board::EMPTY) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), nextPos, &piece});
        }
        if (board[nextPos] != Board::EMPTY && sgn(board[nextPos]) != piece.getColor()) {
            Piece *capturedPiece = board.getPieceFromList(nextPos);
            // Test for nullptr
#ifndef NDEBUG
            if (capturedPiece == nullptr) {
                throw std::invalid_argument("Captured Piece in King move generation is a nullptr");
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

        if (movePos / 10 == 2 || movePos / 10 == 9) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::BISHOP});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::ROOK});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::KNIGHT});
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece, nullptr, true, Piece::QUEEN});
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), movePos, &piece});
        }
    }

    // check for capturing pieces for each attack position


    if (board[attackPos1] != Board::EMPTY && board[attackPos1] != Board::OUTSIDE && sgn(board[attackPos1]) != piece.getColor()) {
        Piece *capturedPiece = board.getPieceFromList(attackPos1);
        // Test for nullptr
#ifndef NDEBUG
        if (capturedPiece == nullptr) {
            throw std::invalid_argument("Captured Piece in pawn move generation attackpos1 is a nullptr");
        }
#endif


        // check for capturing pawn conversions
        if (movePos / 10 == 2 || movePos / 10 == 9) {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::BISHOP});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::ROOK});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::KNIGHT});
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece, true, Piece::QUEEN});
        } else {
            pseudoMoves.emplace_back(Move{piece.getPosition(), attackPos1, &piece, capturedPiece});
        }
    }

    if (board[attackPos2] != Board::EMPTY && board[attackPos2] != Board::OUTSIDE && sgn(board[attackPos2]) != piece.getColor()) {
        Piece *capturedPiece = board.getPieceFromList(attackPos2);
        // Test for nullptr
#ifndef NDEBUG
        if (capturedPiece == nullptr) {
            throw std::invalid_argument("Captured Piece in pawn move generation attackpos2 is a nullptr");
        }
#endif

        if (movePos / 10 == 2 || movePos / 10 == 9) {
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

bool MoveGenerator::isKingChecked(Board &board, int color) {
    auto pieceList = board.getPieceList(color);

    const auto it = std::find_if(pieceList.begin(), pieceList.end(),
        [&color](const Piece& piece){return piece.getColor() == color && piece.getType() == Piece::KING;});

    if (it == pieceList.end()) {
        throw std::invalid_argument("Could not find King in Piece List");
    }

    const Piece& king = *it;

    if (isSquareAttacked(board, king.getPosition(), king.getColor())) {
        return true;
    }
    return false;
}

bool MoveGenerator::isSlidingAttacker(int piece, int dir) {
    if((piece == Piece::ROOK || piece == Piece::QUEEN) &&
           (dir == 1 || dir == -1 || dir == 10 || dir == -10)) return true;

    if((piece == Piece::BISHOP || piece == Piece::QUEEN) &&
        (dir == 9 || dir == -9 || dir == 11 || dir == -11)) return true;

    return false;
}

bool MoveGenerator::isLegalCastle(Board &board, const Move &move) {
    if (move.from == E1 && move.to == G1) {
        return !(isSquareAttacked(board, E1, move.movedPiece->getColor()) ||
                isSquareAttacked(board, F1, move.movedPiece->getColor()) ||
                isSquareAttacked(board, G1, move.movedPiece->getColor()));
    }
    if (move.from == E1 && move.to == C1) {
        return !(isSquareAttacked(board, E1, move.movedPiece->getColor()) ||
                isSquareAttacked(board, D1, move.movedPiece->getColor()) ||
                isSquareAttacked(board, C1, move.movedPiece->getColor()));
    }
    if (move.from == E8 && move.to == G8) {
        return !(isSquareAttacked(board, E8, move.movedPiece->getColor()) ||
                isSquareAttacked(board, F8, move.movedPiece->getColor()) ||
                isSquareAttacked(board, G8, move.movedPiece->getColor()));
    }
    if (move.from == E8 && move.to == C8) {
        return !(isSquareAttacked(board, E8, move.movedPiece->getColor()) ||
                isSquareAttacked(board, D8, move.movedPiece->getColor()) ||
                isSquareAttacked(board, C8, move.movedPiece->getColor()));
    }
    return false;
}

bool MoveGenerator::isSquareAttacked(Board &board, int square, int color) {

    static constexpr int knightDirections[] = {19, 21, -19, -21, 12, -12, 8, -8};
    static constexpr int slidingDirections[] = {9, 11, -9, -11, 1, -1, 10, -10};



    // Go through sliding directions
    for (const int dir : slidingDirections) {
        int checkDirection = square + dir;
        while(board[checkDirection] != Board::OUTSIDE) {

            if(board[checkDirection] != Board::EMPTY && sgn(board[checkDirection]) != color) {
                if(isSlidingAttacker(abs(board[checkDirection]), dir)) {
                    return true;
                }
                break;
            }
            if(board[checkDirection] != 0 && sgn(board[checkDirection]) == color){
                break;
            }
            checkDirection += dir;
        }
    }

    // Check Knight Spots
    for(const int dir : knightDirections) {
        const int checkDirection = square + dir;
        const int potPiece = board[checkDirection];

        if(abs(potPiece) == Piece::KNIGHT && sgn(potPiece) != color) {
            return true;
        }
    }

    // Check Pawns
    int pawnPos1 = square + (11 * color);
    int pawnPos2 = square + (9 * color);

    if ((abs(board[pawnPos1]) == Piece::PAWN && sgn(board[pawnPos1]) != color) ||
        (abs(board[pawnPos2]) == Piece::PAWN && sgn(board[pawnPos2]) != color)) {
        return true;
    }

    return false;
}
