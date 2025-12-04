cmake_minimum_required(VERSION 4.0)
project(BetterChess)

set(CMAKE_CXX_STANDARD 26)

add_library(Board
        board/Board.cpp
        board/boardEnum.cpp
        board/FenParser.cpp
        board/FenParser.h
)

target_include_directories(Board PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/board
)

add_executable(BetterChess
        main.cpp
        pieces/Piece.cpp
        moveGen/MoveLogic.cpp
        gameloop.cpp
        gameloop.h
        bitboards/BitboardData.cpp
        bitboards/BitboardData.h
)

target_link_libraries(BetterChess Board)
