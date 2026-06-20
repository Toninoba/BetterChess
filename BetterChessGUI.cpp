//
// Created by Tobi on 11.06.2026.
//
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Board.h"
#include "FenParser.h"
#include "Piece.h"

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

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface* chessboardSurface = nullptr;
    SDL_Surface* chessPiecesSurface = nullptr;
    char* chessboard_path = nullptr;
    char* chessPieces_path = nullptr;


    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FenParser::parseFen(board, fen);

    SDL_SetAppMetadata("Better Chess", "1.0", "com.betterchess");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Better Chess", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
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
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{

    float boardSize = 480.0f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, chessboard, NULL, NULL);

    // Render every Piece on board
    auto whitePieces = board.getPieceList(Piece::WHITE);
    auto blackPieces = board.getPieceList(Piece::BLACK);

    for (Piece& piece : whitePieces) {
        renderPiece(piece.getPosition(), piece.getColor(), piece.getType());
    }

    for (Piece& piece : blackPieces) {
        renderPiece(piece.getPosition(), piece.getColor(), piece.getType());
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}