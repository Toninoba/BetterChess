cmake_minimum_required(VERSION 4.0)
project(BetterChess)

set(CMAKE_CXX_STANDARD 26)

add_library(Board
        board/Board.cpp
        board/Board.h
        board/boardEnum.cpp
        board/FenParser.cpp
        board/FenParser.h
)

target_include_directories(Board PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/board
)

add_library(MoveGen
        moveGen/MoveLogic.cpp
        moveGen/MoveLogic.h
        moveGen/MoveGenerator.cpp
        moveGen/MoveGenerator.h
        moveGen/Move.cpp
        moveGen/Move.h
)

target_include_directories(MoveGen PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/moveGen
)

add_library(Piece
        pieces/Piece.cpp
        pieces/Piece.h
)

target_include_directories(Piece PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/pieces
)


add_executable(BetterChess
        main.cpp
        gameloop.cpp
        gameloop.h
)

target_link_libraries(BetterChess Board MoveGen Piece)

add_executable(PerftTests
        tests/performPerftTests.cpp
)

target_link_libraries(PerftTests Board MoveGen Piece)




