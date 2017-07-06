#include <iostream>
#include <SDL.h>

static const unsigned WINDOW_WIDTH = 800;
static const unsigned WINDOW_HEIGHT = 600;
static const unsigned BOX_WIDTH = 256;
static const unsigned BOX_HEIGHT = 256;

int main(int argc, char* argv[]) {
  SDL_Surface* displaySurface = NULL;
  SDL_Event sdlEvent;
  SDL_Rect boxBounds;
  SDL_Rect boxRect;
  unsigned boxSpeed;
  unsigned boxColor;
  unsigned backgroundColor;
  bool isRunning;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "SDL_Init Failed: "
      << SDL_GetError()
      << std::endl;

    return -1;
  }

  displaySurface = SDL_SetVideoMode(
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    32,
    SDL_HWSURFACE | SDL_DOUBLEBUF
  );

  if (!displaySurface) {
    std::cerr << "SDL_SetVideoMode Failed:"
      << SDL_GetError()
      << std::endl;

    return -1;
  }

  SDL_WM_SetCaption("Big Blue Box", NULL);

  boxRect.w = BOX_WIDTH;
  boxRect.h = BOX_HEIGHT;
  boxRect.x = (WINDOW_WIDTH - boxRect.w) / 2;
  boxRect.y = (WINDOW_HEIGHT - boxRect.h) / 2;

  boxSpeed = 6;

  boxBounds.x = 0;
  boxBounds.y = 0;
  boxBounds.w = WINDOW_WIDTH - boxRect.w;
  boxBounds.h = WINDOW_HEIGHT - boxRect.h;

  backgroundColor = SDL_MapRGB(displaySurface->format, 0, 0, 0);
  boxColor = SDL_MapRGB(displaySurface->format, 0, 0, 255);

  isRunning = true;

  while (isRunning) {
    while (SDL_PollEvent(&sdlEvent)) {
      switch (sdlEvent.type) {
        case SDL_QUIT: {
          isRunning = false;
        } break;

        case SDL_KEYDOWN: {
          if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
          }
        } break;
      }
    }

    unsigned char* keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_UP]) {
      boxRect.y -= boxSpeed;
      if (boxRect.y < boxBounds.y) {
        boxRect.y = boxBounds.y;
      }
    } else if (keys[SDLK_DOWN]) {
      boxRect.y += boxSpeed;
      if (boxRect.y > boxBounds.h) {
        boxRect.y = boxBounds.h;
      }
    }

    if (keys[SDLK_LEFT]) {
      boxRect.x -= boxSpeed;
      if (boxRect.x < boxBounds.x) {
        boxRect.x = boxBounds.x;
      }
    } else if (keys[SDLK_RIGHT]) {
      boxRect.x += boxSpeed;
      if (boxRect.x > boxBounds.w) {
        boxRect.x = boxBounds.w;
      }
    }

    SDL_FillRect(displaySurface, NULL, backgroundColor);
    SDL_FillRect(displaySurface, &boxRect, boxColor);
    SDL_Flip(displaySurface);

    SDL_Delay(20);
  }

  SDL_Quit();

  return 0;
}
