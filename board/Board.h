//
// Created by tobi on 10/22/25.
//

#ifndef BETTERCHESS_BOARD_H
#define BETTERCHESS_BOARD_H
#include <algorithm>
#include <iosfwd>
#include <ostream>
#include <vector>

#include "../pieces/Piece.h"
#include "boardEnum.cpp"

constexpr int sgn(const int x) {
    return (x > 0) - (x < 0);
}


class Board {
public:
    static constexpr int EMPTY = 0;
    static constexpr int OUTSIDE = 9;

    // Bitmask for describing castling rights
    // 0b 0000 1111
    // last 4 bits in order: White Queen, White King, Black Queen, Black King
    char castlingRights;


    Board() : _halfMoveClock(0), _fullMoveCount(0), _turnToMove(Piece::WHITE),
                _stalemate(false), _checkmate(false),
                castlingRights(0b00001111) {

        _whitePieces.reserve(16);
        _blackPieces.reserve(16);

    }

    void setPieceBoard(const int idx, const int pieceType) {
        _board[idx] = pieceType;
    }

    [[nodiscard]] int getPieceBoard(const int idx) const {
        return _board[idx];
    }

    void addPiece(const Piece& piece) {
        if (piece.getColor() == Piece::WHITE) {
            _whitePieces.push_back(piece);
        }
        else {
            _blackPieces.push_back(piece);
        }
        setPieceBoard(piece.getPosition(), piece.getType() * piece.getColor());
    }

    void removePiece(const Piece& piece) {
        std::vector<Piece>& pieceList = getPieceList(piece.getColor());


        auto it = std::find(pieceList.begin(), pieceList.end(), piece);

        if (it == pieceList.end()) {
            throw std::runtime_error("Figur konnte nicht in der Liste gefunden und gelöscht werden.");
        }

        pieceList.erase(it);
    }

    std::vector<Piece>& getPieceList(const int color) {
        if (color == Piece::WHITE) {
            return _whitePieces;
        }
        return _blackPieces;
    }

    int operator[](const int i) const {
        return _board[i];
    }

    int& operator[](const int i) {
        return _board[i];
    }

    void setTurnToMove(const int color) {
        _turnToMove = color;
    }

    [[nodiscard]] int getTurnToMove() const {
        return _turnToMove;
    }

    [[nodiscard]] int getEnPassant() const {
        return _enPassantPos;
    }

    void setEnPassant(const int pos) {
        _enPassantPos = pos;
    }

    void clearCastlingRights() {
        castlingRights = 0;
    }

    [[nodiscard]] int getHalfMoveClock() const {
        return _halfMoveClock;
    }

    void setHalfMoveClock(const int clock) {
        _halfMoveClock = clock;
    }

    [[nodiscard]] int getFullMoveCounter() const {
        return _fullMoveCount;
    }

    void setFullMoveCounter(const int counter) {
        _fullMoveCount = counter;
    }

    [[nodiscard]] Piece* getPieceFromList(const int pos) {
        for (auto& pieceColorList = getPieceList(sgn(_board[pos])); auto& piece : pieceColorList) {
            if (piece.getPosition() == pos && piece.isAlive()) {
                return &piece;
            }
        }
        return nullptr;
    }




    friend std::ostream& operator<< (std::ostream& stream, const Board& board);


private:

    int _board[120] = {
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 0, 0, 0, 0, 0, 0, 0, 0, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    };
    std::vector<Piece> _whitePieces;
    std::vector<Piece> _blackPieces;

    int _halfMoveClock;
    int _fullMoveCount;
    int _turnToMove;

    // Describes the position of the piece that can be enpassanted
    int _enPassantPos;

    bool _stalemate;
    bool _checkmate;





};

inline std::ostream &operator<<(std::ostream &stream, const Board &board) {
    int row = 8;
    int i = 90;
    stream << "\n";
    stream << row-- << "  ";
    while(i > 20) {

        if (board[i] != Board::OUTSIDE) {
            char p = '.';
            switch (board[i]) {
                case Piece::PAWN * Piece::WHITE:
                    p = 'P';
                    break;
                case Piece::PAWN * Piece::BLACK:
                    p = 'p';
                    break;
                case Piece::KNIGHT * Piece::WHITE:
                    p = 'N';
                    break;
                case Piece::KNIGHT * Piece::BLACK:
                    p = 'n';
                    break;
                case Piece::BISHOP * Piece::WHITE:
                    p = 'B';
                    break;
                case Piece::BISHOP * Piece::BLACK:
                    p = 'b';
                    break;
                case Piece::ROOK * Piece::WHITE:
                    p = 'R';
                    break;
                case Piece::ROOK * Piece::BLACK:
                    p = 'r';
                    break;
                case Piece::QUEEN * Piece::WHITE:
                    p = 'Q';
                    break;
                case Piece::QUEEN * Piece::BLACK:
                    p = 'q';
                    break;
                case Piece::KING * Piece::WHITE:
                    p = 'K';
                    break;
                case Piece::KING * Piece::BLACK:
                    p = 'k';
                    break;
                default:
                    p = '.';
                    break;
            }
            stream << " " << p << " ";

        }


        if (i % 10 == 9) {
            stream << "\n";
            i -= 19;

            if (i > 18 && i < 99) {
                stream << row-- << "  ";
            }
        }
        i++;
    }
    stream << "\n    a  b  c  d  e  f  g  h\n\n";
    stream << std::endl;

    return stream;
}

#endif //BETTERCHESS_BOARD_H