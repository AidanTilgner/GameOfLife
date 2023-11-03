#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 800
#define SPACE_SIZE 20
#define GRID_WIDTH (WIDTH / SPACE_SIZE)
#define GRID_HEIGHT (WIDTH / SPACE_SIZE)
#define FPS 5

typedef struct {
  int alive;
  int x;
  int y;
} Cell;

typedef struct {
  int generation;
  Cell world[GRID_HEIGHT][GRID_WIDTH];
} GameState;

void drawGrid(SDL_Renderer *renderer, int grid_width, int grid_height,
              int space_size);
void updateBoard(SDL_Renderer *renderer, GameState *GameState);
GameState *getInitialGameState(int first_generation, Cell *initial_cells,
                               int initial_cell_length);
void freeGameState(GameState *gamestate);
void renderBoard(SDL_Renderer *renderer, GameState GameState);

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  };

  Cell initial_cells[] = {
      {
          .alive = 1,
          .x = 2,
          .y = 2,
      },
      {
          .alive = 1,
          .x = 2,
          .y = 3,
      },
      {
          .alive = 1,
          .x = 2,
          .y = 4,
      },
  };

  int initial_cell_length = sizeof(initial_cells) / sizeof(initial_cells[0]);

  const int width = WIDTH;
  const int height = HEIGHT;
  const int space_size = SPACE_SIZE;
  const int grid_width = GRID_WIDTH;
  const int grid_height = GRID_HEIGHT;

  SDL_Window *window =
      SDL_CreateWindow("Game of Life", 0, 0, width, height, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    printf("Renderer could not be created! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  }

  int quit = 0;

  SDL_Event e;

  GameState *gamestate =
      getInitialGameState(1, initial_cells, initial_cell_length);

  int generation = 0;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }
    printf("Generation %d\n\n", generation);
    generation++;
    updateBoard(renderer, gamestate);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);

    drawGrid(renderer, grid_width, grid_height, space_size);
    renderBoard(renderer, *gamestate);

    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / FPS);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  freeGameState(gamestate);
  SDL_Quit();

  return 0;
}

void drawGrid(SDL_Renderer *renderer, int grid_width, int grid_height,
              int space_size) {
  SDL_SetRenderDrawColor(renderer, 0x40, 0x3D, 0x3C, 0xFF);

  for (int row = 0; row < grid_height; row++) {
    for (int col = 0; col < grid_width; col++) {
      int x = col * space_size;
      int y = row * space_size;

      SDL_Rect square = {x, y, space_size, space_size};
      SDL_RenderDrawRect(renderer, &square);
    }
  }
}

GameState *getInitialGameState(int first_generation, Cell *initial_cells,
                               int initial_cell_length) {
  GameState *gamestate = malloc(sizeof(GameState));

  gamestate->generation = first_generation;

  for (int r = 0; r < GRID_HEIGHT; r++) {
    for (int c = 0; c < GRID_WIDTH; c++) {
      Cell temp = {
          .alive = 0,
          .x = c,
          .y = r,
      };

      gamestate->world[r][c] = temp;
    }
  }

  for (int i = 0; i < initial_cell_length; i++) {
    Cell cell = initial_cells[i];
    gamestate->world[cell.y][cell.x] = cell;
  }

  return gamestate;
};

void freeGameState(GameState *gamestate) { free(gamestate); }

void renderBoard(SDL_Renderer *renderer, GameState GameState) {
  for (int r = 0; r < GRID_WIDTH; r++) {
    for (int c = 0; c < GRID_HEIGHT; c++) {
      Cell cell = GameState.world[r][c];
      SDL_Rect cellRect = {.h = SPACE_SIZE,
                           .w = SPACE_SIZE,
                           .x = r * SPACE_SIZE,
                           .y = c * SPACE_SIZE};

      if (cell.alive) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &cellRect);
      }
    }
  }
}

void updateBoard(SDL_Renderer *renderer, GameState *GameState) {
  Cell newWorld[GRID_HEIGHT][GRID_WIDTH];

  for (int r = 0; r < GRID_HEIGHT; r++) {
    for (int c = 0; c < GRID_WIDTH; c++) {
      Cell cell = GameState->world[r][c];

      int numNeighborsAlive = 0;
      for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
          if (dr == 0 && dc == 0)
            continue;

          int newR = r + dr;
          int newC = c + dc;

          newR = (newR + GRID_WIDTH) % GRID_WIDTH;
          newC = (newC + GRID_HEIGHT) % GRID_HEIGHT;

          if (GameState->world[newR][newC].alive) {
            numNeighborsAlive++;
          }
        }
      }

      if (numNeighborsAlive == 3 ||
          (numNeighborsAlive == 2 && GameState->world[r][c].alive)) {
        newWorld[r][c].alive = 1;
      } else {
        newWorld[r][c].alive = 0;
      }
    }
  }

  for (int r = 0; r < GRID_WIDTH; r++) {
    for (int c = 0; c < GRID_HEIGHT; c++) {
      GameState->world[r][c] = newWorld[r][c];
    }
  }
}
