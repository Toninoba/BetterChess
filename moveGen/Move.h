//
// Created by Tobi on 17.04.2026.
//

#ifndef BETTERCHESS_MOVE_H
#define BETTERCHESS_MOVE_H

#include <string>

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

    // GPT generated stuff
    static std::string squareToString(int pos) {
        char file = 'a' + (pos % 10) - 1;
        char rank = '0' + (pos / 10) - 1;

        return std::string{file} + rank;
    }

    std::string toString() const {
        std::string move =
            squareToString(from) +
            squareToString(to);

        if (isPromotion) {
            move += "=";

            switch (promotionType) {
                case Piece::QUEEN:  move += 'Q'; break;
                case Piece::ROOK:   move += 'R'; break;
                case Piece::BISHOP: move += 'B'; break;
                case Piece::KNIGHT: move += 'N'; break;
                default: break;
            }
        }

        return move;
    }
};

#endif //BETTERCHESS_MOVE_H
