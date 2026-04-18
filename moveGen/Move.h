//
// Created by Tobi on 17.04.2026.
//

#ifndef BETTERCHESS_MOVE_H
#define BETTERCHESS_MOVE_H

#include "../pieces/Piece.h"

struct Move {
    int from = -1;
    int to = -1;
    Piece* movedPiece = nullptr;
    Piece* capturedPiece = nullptr;
    bool isPromotion = false;
    Piece::TYPE promotionType = Piece::KING;
    bool isCastle = false;
    bool isEnPassant = false;

    // DO NOT SET THESE WHILE CONSTRUCTING A MOVE
    // used only for reconstructing previous board
    char prevCastlingRights = 0;
    int prevEnPassantPos = 0;
};

#endif //BETTERCHESS_MOVE_H
