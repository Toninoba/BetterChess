//
// Created by tobi on 28.11.25.
//

#ifndef BETTERCHESS_GAMELOOP_H
#define BETTERCHESS_GAMELOOP_H

#include "Board.h"

void startGameLoop(Board& board);
void start2PlayerGame();

void processPlayerInput(std::string& moveString, Board& board);

#endif //BETTERCHESS_GAMELOOP_H