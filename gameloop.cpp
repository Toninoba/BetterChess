//
// Created by tobi on 28.11.25.
//

#include "gameloop.h"
#include "Board.h"
#include "FenParser.h"
#include "moveGen/MoveLogic.h"


void startGameLoop(Board& board) {
    using namespace std;

    while (true) {

        cout << board;

        if (board.getTurnToMove() == Piece::WHITE) {
            cout << "White to move: ";
        }
        else {
            cout << "Black to move: ";
        }

        string playerMove;
        cin >> playerMove;

        if (playerMove == "u") {
            MoveLogic::undoLastMove(board);
        }
        else if (playerMove == "q") {
            break;
        }
        else {
            processPlayerInput(playerMove, board);
        }


        cout << "\n\n\n\n\n\n\n" << endl;
    }
}

void processPlayerInput(std::string &moveString, Board &board) {

    if (moveString.size() != 4) {
        throw std::runtime_error("Illegal move");
    }

    int fromFile = moveString[0] - 'a';
    int fromRank = moveString[1] - '1';

    int toFile = moveString[2] - 'a';
    int toRank = moveString[3] - '1';

    int fromPos = fromRank * 10 + 21 + fromFile;
    int toPos = toRank * 10 + 21 + toFile;

    Move move;

    move.from = fromPos;
    move.to = toPos;
    move.movedPiece = board.getPieceFromList(fromPos);
    move.capturedPiece = board.getPieceFromList(toPos);
    move.isCastle = false;
    move.isEnPassant = false;
    move.isPromotion = false;

    MoveLogic::performMove(board, move);
}



void start2PlayerGame() {
    Board playingBoard;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FenParser::parseFen(playingBoard, fen);

    startGameLoop(playingBoard);
}


