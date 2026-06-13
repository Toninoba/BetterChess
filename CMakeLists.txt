cmake_minimum_required(VERSION 4.0)
project(BetterChess)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

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

add_executable(BetterChessGUI
        BetterChessGUI.cpp
)



target_link_libraries(BetterChess PRIVATE Board MoveGen Piece SDL3::SDL3)
target_link_libraries(BetterChessGUI PRIVATE Board MoveGen Piece SDL3::SDL3)

add_executable(PerftTests
        tests/performPerftTests.cpp
)

target_link_libraries(PerftTests Board MoveGen Piece)

add_custom_command(TARGET BetterChessGUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:BetterChessGUI>/assets
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})