//
// Created by tobi on 04.12.25.
//

#include "BitboardData.h"

BitboardData::BitboardData(Board &board) {
   // Parse every white piece into bitboards
   for (const Piece &piece: board.getPieceList(Piece::WHITE)) {

      const int position = getBitboardPosition(piece.getPosition());

      whitePieces |= 1ULL << position;
      occupied |= 1ULL << position;

      parsePieceTypeBitboard(piece, position);
   }

   for (const Piece &piece : board.getPieceList(Piece::BLACK)) {
      const int position = getBitboardPosition(piece.getPosition());

      blackPieces |= 1ULL << position;
      occupied |= 1ULL << position;

      parsePieceTypeBitboard(piece, position);
   }
}


int BitboardData::getBitboardPosition(const int mailboxPos) {

   // a1 = 21 -> 0
   const int file = (mailboxPos - 21) % 10;
   const int rank = (mailboxPos - 21) / 10;
   return rank * 8 + file;
}

void BitboardData::parsePieceTypeBitboard(const Piece &piece, const int position) {
   if (piece.getColor() == Piece::WHITE) {
      switch (piece.getType()) {
         case Piece::PAWN:
            whitePawns |= 1ULL << position;
            break;
         case Piece::KNIGHT:
            whiteKnights |= 1ULL << position;
            break;
         case Piece::BISHOP:
            whiteBishops |= 1ULL << position;
            break;
         case Piece::ROOK:
            whiteRooks |= 1ULL << position;
            break;
         case Piece::QUEEN:
            whiteQueens |= 1ULL << position;
            break;
         case Piece::KING:
            whiteKing |= 1ULL << position;
            break;
         default:
            throw std::runtime_error("Invalid piece type while parsing bitboard data");
      }
   }
   else {
      switch (piece.getType()) {
         case Piece::PAWN:
            blackPawns |= 1ULL << position;
            break;
         case Piece::KNIGHT:
            blackKnights |= 1ULL << position;
            break;
         case Piece::BISHOP:
            blackBishops |= 1ULL << position;
            break;
         case Piece::ROOK:
            blackRooks |= 1ULL << position;
            break;
         case Piece::QUEEN:
            blackQueens |= 1ULL << position;
            break;
         case Piece::KING:
            blackKing |= 1ULL << position;
            break;
         default:
            throw std::runtime_error("Invalid piece type while parsing bitboard data");
      }
   }
}


