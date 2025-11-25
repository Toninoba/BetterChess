//
// Created by tobi on 10/22/25.
//

#ifndef BETTERCHESS_PIECE_H
#define BETTERCHESS_PIECE_H




class Piece {
public:
    enum TYPE {
        PAWN = 1,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    };


    static constexpr int WHITE = 1;
    static constexpr int BLACK = -1;

    Piece(const TYPE type, const int color, const int pos) : _type(type), _color(color), _position(pos), _alive(true){}

    [[nodiscard]] int getColor() const {
        return _color;
    }

    [[nodiscard]] int getType() const {
        return _type;
    }

    [[nodiscard]] int getPiece() const {
        return _type * _color;
    }

    [[nodiscard]] int getPosition() const {
        return _position;
    }

    void setPosition(const int position) {
        _position = position;
    }

    void setAlive(const bool status) {
        _alive = status;
    }

    [[nodiscard]] bool isAlive() const {
        return _alive;
    }



    bool operator==(const Piece &other) const {
        return getType() == other.getType() &&
                getColor() == other.getColor() &&
                getPosition() == other.getPosition();
    }



private:
    int _type;
    int _color;
    int _position;
    bool _alive;
};


#endif //BETTERCHESS_PIECE_H