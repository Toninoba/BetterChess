//
// Created by tobi on 10/29/25.
//

#ifndef BETTERCHESS_FENPARSER_H
#define BETTERCHESS_FENPARSER_H
#include <iostream>
#include <regex>

#include "Board.h"


class FenParser {
public:
    static void parseFen(Board& board, std::string& fen) {
        int startPos = A8;

        const auto lineRegex = std::regex("[^/]+");
        std::smatch m;

        for (std::sregex_iterator it(fen.begin(), fen.end(), lineRegex), end; it != end; ++it) {
            std::string line = it->str();

            parseBoardLine(board, line, startPos);
            startPos -= 10;
        }

        std::cout << board << std::endl;
    }
private:
    static void parseBoardLine(Board& board, std::string& line, int& boardPos) {
        for (const char piece : line) {
            Piece::TYPE type = Piece::PAWN;
            int color = 0;

            switch (piece) {
                case 'r':
                    type = Piece::ROOK;
                    color = Piece::BLACK;
                    break;
                case 'R':
                    type = Piece::ROOK;
                    color = Piece::WHITE;
                    break;
                case 'n':
                    type = Piece::KNIGHT;
                    color = Piece::BLACK;
                    break;
                case 'N':
                    type = Piece::KNIGHT;
                    color = Piece::WHITE;
                    break;
                case 'b':
                    type = Piece::BISHOP;
                    color = Piece::BLACK;
                    break;
                case 'B':
                    type = Piece::BISHOP;
                    color = Piece::WHITE;
                    break;
                case 'q':
                    type = Piece::QUEEN;
                    color = Piece::BLACK;
                    break;
                case 'Q':
                    type = Piece::QUEEN;
                    color = Piece::WHITE;
                    break;
                case 'k':
                    type = Piece::KING;
                    color = Piece::BLACK;
                    break;
                case 'K':
                    type = Piece::KING;
                    color = Piece::WHITE;
                    break;
                case 'p':
                    type = Piece::PAWN;
                    color = Piece::BLACK;
                    break;
                case 'P':
                    type = Piece::PAWN;
                    color = Piece::WHITE;
                    break;
                default:
                    type = Piece::PAWN;
                    color = 0;
                    break;

            }

            if ('1' <= piece && piece <= '8') {
                boardPos += piece - '0';
            }
            else {
                Piece pieceToAdd(type, color, boardPos);
                board.addPiece(pieceToAdd);
                boardPos++;
            }
        }
    }
};


#endif //BETTERCHESS_FENPARSER_H