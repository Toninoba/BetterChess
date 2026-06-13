//
// Created by Tobi on 08.06.2026.
//


#include <chrono>

#include "Board.h"
#include "FenParser.h"
#include "../moveGen/Move.h"
#include "../moveGen/MoveGenerator.h"
#include "../moveGen/MoveLogic.h"

unsigned long long perft(Board& board, int depth) {



    unsigned long long nodes = 0;

    std::vector<Move> moves = MoveGenerator::generateLegalMoves(board);

    if (depth == 1) {
        return moves.size();
    }

    for (Move& move: moves) {
        MoveLogic::performMove(board, move);
        nodes += perft(board, depth - 1);
        MoveLogic::undoLastMove(board);
    }
    return nodes;
}

unsigned long long slowPerft(Board& board, int depth) {

    if (depth == 0) {
        return 1ULL;
    }

    unsigned long long nodes = 0;

    std::vector<Move> moves = MoveGenerator::generateLegalMoves(board);



    for (Move& move: moves) {
        MoveLogic::performMove(board, move);
        nodes += slowPerft(board, depth - 1);
        MoveLogic::undoLastMove(board);
    }
    return nodes;
}




void assertResult(unsigned long long result, unsigned long long expected) {
    if (result != expected) {
        std::cout << "Failed Perft with result: " << result << " expected: " << expected << "\n";
    }
    else {
        std::cout << "Passed Perft with result: " << result << " expected: " << expected << "\n";
    }
}

void testInitialPosition() {
    Board board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    assertResult(perft(board, 1), 20);
    assertResult(perft(board, 2), 400);
    assertResult(perft(board, 3), 8902);
    assertResult(perft(board, 4), 197281);
    assertResult(perft(board, 5), 4'865'609);
    assertResult(perft(board, 6), 119'060'324);
    assertResult(perft(board, 7), 3'195'901'860);

}

void testPosition2() {
    Board board;
    std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    assertResult(perft(board, 1), 48);
    assertResult(perft(board, 2), 2039);
    assertResult(perft(board, 3), 97862);
    assertResult(perft(board, 4), 4085603);
    assertResult(perft(board, 5), 193'690'690);

}

void testPosition3() {
    Board board;
    std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    FenParser::parseFen(board, fen);

    assertResult(perft(board, 1), 14);
    assertResult(perft(board, 2), 191);
    assertResult(perft(board, 3), 2812);
    assertResult(perft(board, 4), 43238);
    assertResult(perft(board, 5), 674624);
    assertResult(perft(board, 6), 11'030'083);
}

void testPosition4() {
    Board board;
    std::string fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    FenParser::parseFen(board, fen);

    assertResult(perft(board, 1), 6);
    assertResult(perft(board, 2), 264);
    assertResult(perft(board, 3), 9467);
    assertResult(perft(board, 4), 422333);
    assertResult(perft(board, 5), 15'833'292);
}

void testPosition5() {
    Board board;
    std::string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ";
    FenParser::parseFen(board, fen);

    assertResult(perft(board, 1), 44);
    assertResult(perft(board, 2), 1486);
    assertResult(perft(board, 3), 62379);
    assertResult(perft(board, 4), 2103487);
    assertResult(perft(board, 5), 89941194);
}

void debugPerft() {
    Board board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    int depth = 6;

    unsigned long long nodes = 0;

    std::vector<Move> moves = MoveGenerator::generateLegalMoves(board);

    for (Move& move: moves) {
        MoveLogic::performMove(board, move);
        unsigned long long localNodes = slowPerft(board, depth - 1);
        nodes += localNodes;
        MoveLogic::undoLastMove(board);

        std::cout << move.toString() << ": " << localNodes << "\n";
    }

    std::cout << "Nodes searched: " << nodes << std::endl;
}


int main() {
    /*
    debugPerft();
    */
    auto start = std::chrono::high_resolution_clock::now();

    testInitialPosition();
    testPosition2();
    testPosition3();
    testPosition4();
    testPosition5();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> dauer = end - start;

    std::cout << "Dauer: " << dauer.count() << " ms\n";
}