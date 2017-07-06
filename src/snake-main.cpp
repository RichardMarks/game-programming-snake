/*
MIT License

Copyright (c) 2017 Richard Marks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include <deque>
#include <SDL.h>

static const unsigned WINDOW_WIDTH = 800;
static const unsigned WINDOW_HEIGHT = 600;

static const unsigned GO_UP = 0;
static const unsigned GO_DOWN = 1;
static const unsigned GO_LEFT = 2;
static const unsigned GO_RIGHT = 3;

static const unsigned CELL_SIZE = 20;
static const unsigned CELL_WIDTH = WINDOW_WIDTH / CELL_SIZE;
static const unsigned CELL_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

static const unsigned START_X = CELL_WIDTH / 2;
static const unsigned START_Y = CELL_HEIGHT / 2;
static const unsigned START_LENGTH = 3;

class SnakeSegment {
  public:
    unsigned x;
    unsigned y;

    SnakeSegment(unsigned x, unsigned y) {
      this->x = x;
      this->y = y;
    }
};

class Coin {
  public:
    unsigned x;
    unsigned y;

    Coin() {
      this->respawn();
    }

    void respawn() {
      this->x = 1 + rand() % (CELL_WIDTH - 2);
      this->y = 1 + rand() % (CELL_HEIGHT - 2);
    }
};

class Snake {
  public:
    Snake(unsigned headColor, unsigned segmentColor, unsigned coinColor);
    ~Snake();
    void restart();
    void update();
    void render(SDL_Surface* dest);
    inline bool isDead() { return this->dead; }

  private:
    std::deque<SnakeSegment> segments;
    Coin coin;
    bool dead;
    unsigned direction;
    unsigned time, timeout;
    unsigned headColor, segmentColor, coinColor;
    SDL_Rect renderRect;

    void updateInputControls();
    void renderSnake(SDL_Surface* dest);
    void renderCoin(SDL_Surface* dest);
    void addSegment(unsigned x, unsigned y);
    void moveSnake();
    bool checkForWallCollision();
    bool checkForSelfCollision();
    bool checkForCoinCollision();
};



int main(int argc, char* argv[]) {
  SDL_Surface* displaySurface = NULL;
  SDL_Event sdlEvent;
  unsigned backgroundColor;
  unsigned snakeHeadColor;
  unsigned snakeSegmentColor;
  unsigned coinColor;
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

  SDL_WM_SetCaption("Snake", NULL);

  backgroundColor = SDL_MapRGB(displaySurface->format, 0, 64, 0);
  snakeHeadColor = SDL_MapRGB(displaySurface->format, 255, 0, 0);
  snakeSegmentColor = SDL_MapRGB(displaySurface->format, 0, 255, 0);
  coinColor = SDL_MapRGB(displaySurface->format, 255, 255, 0);

  isRunning = true;

  Snake snake(snakeHeadColor, snakeSegmentColor, coinColor);

  while (isRunning) {
    while (SDL_PollEvent(&sdlEvent)) {
      switch (sdlEvent.type) {
        case SDL_QUIT: {
          isRunning = false;
        } break;

        case SDL_KEYDOWN: {
          if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
          } else if (sdlEvent.key.keysym.sym == SDLK_RETURN) {
            if (snake.isDead()) {
              snake.restart();
            }
          }
        } break;
      }
    }

    snake.update();

    SDL_FillRect(displaySurface, NULL, backgroundColor);

    snake.render(displaySurface);

    SDL_Flip(displaySurface);

    SDL_Delay(20);
  }

  SDL_Quit();

  return 0;
}

Snake::Snake(unsigned headColor, unsigned segmentColor, unsigned coinColor) {
  this->headColor = headColor;
  this->segmentColor = segmentColor;
  this->coinColor = coinColor;

  this->restart();
}

Snake::~Snake() {
  this->segments.clear();
}

void Snake::restart() {
  this->segments.clear();

  for (unsigned i = 0; i < START_LENGTH; i += 1) {
    this->addSegment(START_X - i, START_Y);
  }

  this->direction = GO_RIGHT;
  this->time = 0;
  this->timeout = 6;
  this->dead = false;

  this->coin.respawn();
}

void Snake::addSegment(unsigned x, unsigned y) {
  SnakeSegment segment(x, y);

  this->segments.push_back(segment);
}

void Snake::update() {
  if (this->dead) {
    return;
  }

  this->updateInputControls();

  this->time += 1;

  if (this->time < this->timeout) {
    return;
  }

  this->time = 0;

  if (this->checkForWallCollision() || this->checkForSelfCollision()) {
    this->dead = true;
    return;
  }

  if (this->checkForCoinCollision()) {
    this->coin.respawn();
  } else {
    this->segments.pop_back();
  }

  this->moveSnake();
}

void Snake::updateInputControls() {
  unsigned char* keys = SDL_GetKeyState(NULL);

  if (keys[SDLK_UP] && this->direction != GO_DOWN) {
    this->direction = GO_UP;
  } else if (keys[SDLK_DOWN] && this->direction != GO_UP) {
    this->direction = GO_DOWN;
  } else if (keys[SDLK_LEFT] && this->direction != GO_RIGHT) {
    this->direction = GO_LEFT;
  } else if (keys[SDLK_RIGHT] && this->direction != GO_LEFT) {
    this->direction = GO_RIGHT;
  }
}

bool Snake::checkForWallCollision() {
  unsigned headX = this->segments[0].x;
  unsigned headY = this->segments[0].y;

  return (
    headX == 0 ||
    headY == 0 ||
    headX == CELL_WIDTH ||
    headY == CELL_HEIGHT
  );
}

bool Snake::checkForSelfCollision() {
  unsigned headX = this->segments[0].x;
  unsigned headY = this->segments[0].y;

  for (unsigned i = 1; i < this->segments.size(); i += 1) {
    if (this->segments[i].x == headX && this->segments[i].y == headY) {
      return true;
    }
  }

  return false;
}

bool Snake::checkForCoinCollision() {
  unsigned headX = this->segments[0].x;
  unsigned headY = this->segments[0].y;

  return this->coin.x == headX && this->coin.y == headY;
}

void Snake::moveSnake() {
  static const int moveX[] = { 0, 0, -1, 1 };
  static const int moveY[] = { -1, 1, 0, 0 };

  unsigned x = this->segments[0].x + moveX[this->direction];
  unsigned y = this->segments[0].y + moveY[this->direction];

  SnakeSegment segment(x, y);

  this->segments.push_front(segment);
}

void Snake::render(SDL_Surface* dest) {
  if (this->dead) {
    return;
  }

  this->renderRect.w = CELL_SIZE;
  this->renderRect.h = CELL_SIZE;

  this->renderCoin(dest);
  this->renderSnake(dest);
}

void Snake::renderCoin(SDL_Surface* dest) {
  this->renderRect.x = this->coin.x * CELL_SIZE;
  this->renderRect.y = this->coin.y * CELL_SIZE;

  SDL_FillRect(dest, &this->renderRect, this->coinColor);
}

void Snake::renderSnake(SDL_Surface* dest) {
  this->renderRect.x = this->segments[0].x * CELL_SIZE;
  this->renderRect.y = this->segments[0].y * CELL_SIZE;

  SDL_FillRect(dest, &this->renderRect, this->headColor);

  for (unsigned i = 1; i < this->segments.size(); i += 1) {
    this->renderRect.x = this->segments[i].x * CELL_SIZE;
    this->renderRect.y = this->segments[i].y * CELL_SIZE;

    SDL_FillRect(dest, &this->renderRect, this->segmentColor);
  }
}
