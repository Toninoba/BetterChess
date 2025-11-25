//
// Created by tobi on 10/29/25.
//

#include "MoveLogic.h"

std::vector<Move> MoveLogic::_moveHistory;

void MoveLogic::performMove(Board &board, const Move &move) {

    // Set Piece to new position
    board.setPieceBoard(move.to, move.movedPiece->getPiece());

    // Clear Piece from previous position
    board.setPieceBoard(move.from, Board::EMPTY);

    // update Piece location information
    move.movedPiece->setPosition(move.to);

    // Check for capture and update captured piece
    if (move.capturedPiece != nullptr) {
        move.capturedPiece->setAlive(false);
    }

    // Check for special moves and perform them
    if (move.isCastle) {
        performCastling(board, move);
    }

    if (move.isEnPassant) {
        performEnPassant(board, move);
    }

    if (move.isPromotion) {
        performPromotion(board, move);
    }

    // Change TurnToMove
    board.setTurnToMove(board.getTurnToMove() == Piece::WHITE ? Piece::BLACK : Piece::WHITE);

    // Push Move to performedMoveStack
   _moveHistory.push_back(move);
}

void MoveLogic::undoLastMove(Board &board) {
    // Get last move performed from stack
    Move& lastMove = _moveHistory.back();
    _moveHistory.pop_back();


    // reset moved piece position
    board.setPieceBoard(lastMove.from, lastMove.movedPiece->getPiece());
    board.setPieceBoard(lastMove.to, Board::EMPTY);
    lastMove.movedPiece->setPosition(lastMove.from);

    // set captured piece back and set it alive
    if (lastMove.capturedPiece != nullptr) {
        lastMove.capturedPiece->setAlive(true);
        board.setPieceBoard(lastMove.capturedPiece->getPosition(), lastMove.capturedPiece->getPiece());
    }

    // Check for special moves and undo them
    if (lastMove.isCastle) {
        undoCastling(board, lastMove);
    }

    if (lastMove.isEnPassant) {
        undoEnPassant(board, lastMove);
    }

    if (lastMove.isPromotion) {
        undoPromotion(board, lastMove);
    }



    // change turnToMove
    board.setTurnToMove(board.getTurnToMove() == Piece::WHITE ? Piece::BLACK : Piece::WHITE);
}

void MoveLogic::performPromotion(Board &board, const Move &move) {

}

void MoveLogic::performEnPassant(Board &board, const Move &move) {

}

void MoveLogic::performCastling(Board &board, const Move &move) {

}

void MoveLogic::undoPromotion(Board &board, const Move &move) {

}

void MoveLogic::undoEnPassant(Board &board, const Move &move) {

}

void MoveLogic::undoCastling(Board &board, const Move &move) {

}





