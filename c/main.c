#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  };

  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  }

  // Create Renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    printf("Renderer could not be created! SDL_ERROR: %s\n", SDL_GetError());
    return 1;
  }

  int quit = 0;

  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }

    // Clear Screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Render a filled rectangle
    SDL_Rect fillRect = {200, 200, 200, 200};
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);

    // Update screen
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
