//
// Created by Tobi on 11.06.2026.
//
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#include "Board.h"
#include "FenParser.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "MoveLogic.h"
#include "Piece.h"
#include "bots/FirstBot.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int chessboard_width = 0;
static int chessboard_height = 0;
static SDL_Texture* chessboard;
static SDL_Texture* chessPieces;
static int SCREEN_WIDTH = 800;
static int SCREEN_HEIGHT = 800;
static Board board;
static std::vector<Move> currentMoves{};
static int markedTile = -1;
static std::vector<int> highlightedTiles;
static bool gameOver = false;
static bool blackWon = false;
static bool whiteWon = false;
static bool isBotGame = false;

void renderPiece(int tile, int color, int type) {
    // Each piece is 377 pixel offset in x and 302 in y
    float sprite_y = color == 1 ? 0.0f : 213.0f;
    float sprite_x = 0.0f;

    switch (type) {
        case Piece::KING:
            sprite_x = 0;
            break;
        case Piece::QUEEN:
            sprite_x = 212;
            break;
        case Piece::BISHOP:
            sprite_x = 212*2;
            break;
        case Piece::KNIGHT:
            sprite_x = 212*3;
            break;
        case Piece::ROOK:
            sprite_x = 212*4;
            break;
        case Piece::PAWN:
            sprite_x = 212*5;
            break;
    }

    int row = tile / 10;
    int col = tile % 10;

    int file = col - 1;
    int rank = row - 2;

    int screenX = file * (SCREEN_WIDTH / 8);
    int screenY = (7 - rank) * (SCREEN_HEIGHT / 8);


    SDL_FRect srcRect = {
        sprite_x,
        sprite_y,
        212,
        213
    };

    SDL_FRect dstRect = {
        (float)screenX,
        (float)screenY,
        (float)SCREEN_WIDTH / 8,
        (float)SCREEN_HEIGHT / 8
    };

    SDL_RenderTexture(renderer, chessPieces, &srcRect, &dstRect);
}

void markToTile(int toTile) {
    int row = toTile / 10;
    int col = toTile % 10;

    int file = col - 1;
    int rank = row - 2;

    int screenX = file * (SCREEN_WIDTH / 8);
    int screenY = (7 - rank) * (SCREEN_HEIGHT / 8);

    SDL_FRect dstRect = {
        (float)screenX,
        (float)screenY,
        (float)SCREEN_WIDTH / 8,
        (float)SCREEN_HEIGHT / 8
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 180);
    SDL_RenderFillRect(renderer, &dstRect);
}

int getTileFromMousePos(int x, int y) {
    int row = 8 - (y / (SCREEN_HEIGHT / 8));
    int col = (x / (SCREEN_WIDTH / 8)) + 1;

    SDL_Log("Row: %d  Col: %d", row, col);

    int tile = ((row + 1) * 10) + col;

    SDL_Log("Tile %d", tile);
    return tile;
}

void renderGameStatus() {
    SDL_Log("Rendering Game Status");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderScale(renderer, 4.0, 4.0);
    if (whiteWon) {
        SDL_RenderDebugText(renderer, 75, 50, "White won!");
    }
    else {
        SDL_RenderDebugText(renderer, 75, 50, "Black won!");
    }


}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface* chessboardSurface = nullptr;
    SDL_Surface* chessPiecesSurface = nullptr;
    char* chessboard_path = nullptr;
    char* chessPieces_path = nullptr;

    isBotGame = true;


    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    currentMoves = MoveGenerator::generateLegalMoves(board);

    SDL_SetAppMetadata("Better Chess", "1.0", "com.betterchess");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Better Chess", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    SDL_asprintf(&chessboard_path, "%sassets/chessboard.png", SDL_GetBasePath());
    SDL_asprintf(&chessPieces_path, "%sassets/chesspieces.png", SDL_GetBasePath());

    chessboardSurface = SDL_LoadPNG(chessboard_path);
    if (!chessboardSurface) {
        SDL_Log("Couldnt load PNG: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    chessPiecesSurface = SDL_LoadPNG(chessPieces_path);
    if (!chessPiecesSurface) {
        SDL_Log("Couldnt load PNG: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }



    SDL_free(chessboard_path);
    SDL_free(chessPieces_path);

    chessboard_width = chessboardSurface->w;
    chessboard_height = chessboardSurface->h;

    chessboard = SDL_CreateTextureFromSurface(renderer, chessboardSurface);
    if (!chessboard) {
        SDL_Log("Couldnt create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    chessPieces = SDL_CreateTextureFromSurface(renderer, chessPiecesSurface);
    if (!chessPieces) {
        SDL_Log("Couldnt create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(chessboardSurface);
    SDL_DestroySurface(chessPiecesSurface);


    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_Z) {
            if (!MoveLogic::_moveHistory.empty()) {
                MoveLogic::undoLastMove(board);
                currentMoves = MoveGenerator::generateLegalMoves(board);
            }
            if (gameOver) {
                gameOver = whiteWon = blackWon = false;
            }

        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            SDL_Log("Linke Maustaste gedrückt!");

            // Mausposition:
            float x = event->button.x;
            float y = event->button.y;

            SDL_Log("Position: %.0f, %.0f", x, y);

            int clickedTile = getTileFromMousePos((int)x, (int)y);
            highlightedTiles.clear();

            for (Move& move : currentMoves) {
                if (move.from == clickedTile) {
                    SDL_Log("Found Move in List");
                    highlightedTiles.push_back(move.to);

                }
                if (move.from == markedTile && move.to == clickedTile) {
                    SDL_Log("Performing Move");
                    MoveLogic::performMove(board, move);
                    currentMoves = MoveGenerator::generateLegalMoves(board);

                    if (currentMoves.empty()) {
                        // Render Text for winning Game
                        gameOver = true;
                        if (board.getTurnToMove() == Piece::WHITE) {
                            blackWon = true;
                        }
                        else {
                            whiteWon = true;
                        }
                    }

                    // Perform Bot Move
                    Move botMove = FirstBot::getBestMove(board);
                    if (botMove.from != -1) {
                        MoveLogic::performMove(board, botMove);
                        currentMoves = MoveGenerator::generateLegalMoves(board);
                    }



                    if (currentMoves.empty()) {
                        // Render Text for winning Game
                        gameOver = true;
                        if (board.getTurnToMove() == Piece::WHITE) {
                            blackWon = true;
                        }
                        else {
                            whiteWon = true;
                        }
                    }

                }
            }

            markedTile = clickedTile;

        }
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

    float boardSize = 480.0f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, chessboard, NULL, NULL);

    for (int tile : highlightedTiles) {
        markToTile(tile);
    }



    // Render every Piece on board
    auto whitePieces = board.getPieceList(Piece::WHITE);
    auto blackPieces = board.getPieceList(Piece::BLACK);

    for (Piece& piece : whitePieces) {
        if (piece.isAlive()) {
            renderPiece(piece.getPosition(), piece.getColor(), piece.getType());
        }

    }

    for (Piece& piece : blackPieces) {
        if (piece.isAlive()) {
            renderPiece(piece.getPosition(), piece.getColor(), piece.getType());
        }

    }

    if (gameOver) {
        renderGameStatus();
        SDL_SetRenderScale(renderer, 1.0, 1.0);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}