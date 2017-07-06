#include <SDL.h>

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return -1;
  }

  SDL_Quit();

  return 0;
}
