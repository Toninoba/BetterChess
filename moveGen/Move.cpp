#include "../pieces/Piece.h"
//
// Created by Tobi on 16.04.2026.
//
struct Move {
    int from = -1;
    int to = -1;
    Piece* movedPiece = nullptr;
    Piece* capturedPiece = nullptr;
    bool isPromotion = false;
    Piece::TYPE promotionType = Piece::KING;
    bool isCastle = false;
    bool isEnPassant = false;

    char prevCastlingRights = 0;
    int prevEnPassantPos = 0;
};