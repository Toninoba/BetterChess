//
// Created by tobi on 10/29/25.
//

#include "MoveLogic.h"

std::vector<Move> MoveLogic::_moveHistory;


void MoveLogic::performMove(Board &board, Move &move) {

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

    // Save current castling rights in performed move for undoing
    move.prevCastlingRights = board.castlingRights;
    // Update Castling Rights if necessary
    updateCastlingRights(board, move);


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

    // Restore Castling rights
    board.castlingRights = lastMove.prevCastlingRights;

    // change turnToMove
    board.setTurnToMove(board.getTurnToMove() == Piece::WHITE ? Piece::BLACK : Piece::WHITE);
}

void MoveLogic::performPromotion(Board &board, const Move &move) {

    // change type of piece in piece object
    move.movedPiece->setType(move.promotionType);

    // change type of piece on board
    board.setPieceBoard(move.to, move.movedPiece->getPiece());
}

void MoveLogic::performEnPassant(Board &board, const Move &move) {
    // update captured pawn position to empty
    board.setPieceBoard(move.capturedPiece->getPosition(), Board::EMPTY);
}

void MoveLogic::performCastling(Board &board, const Move &move) {

    // Check which King was moved
    if (move.movedPiece->getColor() == Piece::WHITE) {
        // Check for direction of castling
        if (move.to == C1) {
            // Move corresponding rook
            Piece* rook = board.getPieceFromList(A1);
            board.setPieceBoard(A1, Board::EMPTY);
            board.setPieceBoard(D1, rook->getPiece());
            rook->setPosition(D1);
        }
        else {
            Piece* rook = board.getPieceFromList(H1);
            board.setPieceBoard(H1, Board::EMPTY);
            board.setPieceBoard(F1, rook->getPiece());
            rook->setPosition(F1);
        }
    }
    else {
        if (move.to == C8) {
            Piece* rook = board.getPieceFromList(A8);
            board.setPieceBoard(A8, Board::EMPTY);
            board.setPieceBoard(D8, rook->getPiece());
            rook->setPosition(D8);
        }
        else {
            Piece* rook = board.getPieceFromList(H8);
            board.setPieceBoard(H8, Board::EMPTY);
            board.setPieceBoard(F8, rook->getPiece());
            rook->setPosition(F8);
        }
    }
}

void MoveLogic::undoPromotion(Board &board, const Move &move) {
    // change piece object to pawn
    move.movedPiece->setType(Piece::PAWN);

    // change piece on board
    board.setPieceBoard(move.from, move.movedPiece->getPiece());
}

void MoveLogic::undoEnPassant(Board &board, const Move &move) {
    // no need to update captured pawn back to pawn, since its handled already

}

void MoveLogic::undoCastling(Board &board, const Move &move) {
    // Check which King was moved
    if (move.movedPiece->getColor() == Piece::WHITE) {
        // Check for direction of castling
        if (move.to == C1) {
            // Move corresponding rook
            Piece* rook = board.getPieceFromList(D1);
            board.setPieceBoard(D1, Board::EMPTY);
            board.setPieceBoard(A1, rook->getPiece());
            rook->setPosition(A1);
        }
        else {
            Piece* rook = board.getPieceFromList(F1);
            board.setPieceBoard(F1, Board::EMPTY);
            board.setPieceBoard(H1, rook->getPiece());
            rook->setPosition(H1);
        }
    }
    else {
        if (move.to == C8) {
            Piece* rook = board.getPieceFromList(D8);
            board.setPieceBoard(D8, Board::EMPTY);
            board.setPieceBoard(A8, rook->getPiece());
            rook->setPosition(A8);
        }
        else {
            Piece* rook = board.getPieceFromList(F8);
            board.setPieceBoard(F8, Board::EMPTY);
            board.setPieceBoard(H8, rook->getPiece());
            rook->setPosition(H8);
        }
    }
}

void MoveLogic::updateCastlingRights(Board &board, const Move &move) {

    // mask castling right bitmask
    if (move.movedPiece->getType() == Piece::KING) {
        if (move.movedPiece->getColor() == Piece::WHITE) {
            board.castlingRights &= 0b00000011;
        }
        else {
            board.castlingRights &= 0b00001100;
        }
    }

    if (move.movedPiece->getType() == Piece::ROOK) {
        if (move.movedPiece->getColor() == Piece::WHITE && move.from == A1) {
            board.castlingRights &= 0b00000111;
        }
        else if (move.movedPiece->getColor() == Piece::WHITE && move.from == H1) {
            board.castlingRights &= 0b00001011;
        }
        else if (move.movedPiece->getColor() == Piece::BLACK && move.from == A8) {
            board.castlingRights &= 0b00001101;
        }
        else if (move.movedPiece->getColor() == Piece::BLACK && move.from == H8) {
            board.castlingRights &= 0b00001110;
        }
        else {
            // Do nothing
        }
    }
}




