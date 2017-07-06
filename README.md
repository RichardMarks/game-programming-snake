# Game Programming: Snake

## Introduction

### What is Snake?

Quoting Wikipedia:
> Snake is a casual video game that originated during the late 1970s in arcades and has maintained popularity since then, becoming something of a classic. After it became the standard pre-loaded game on Nokia mobile phones in 1998, Snake found a massive audience.

### Who is this article for?

In order to get the most out of this article, you should be familiar with the C++ programming language, and have written and successfully compiled a few programs (or games) on your own.

### What will I learn?

If I do my job right, you should learn how to develop a Snake clone game. We are going to talk about the basics first, that is, getting something on the screen. Then we will cover gathering input from the player and making things move. Next we will go through the architecture of the game, and finally we will take each step one by one and by the end you should have a solid understanding of how to sit down and write your very own Snake game from scratch.

### What do I need?

I will be using a Windows machine, however you may use a Linux or a Mac computer because we are using a cross-platform library so that our game can target as many platforms as possible without changing the code. That said, here is a list of things to bring with you.

+ C++ compiler (I will be using the Mingw C++ compiler provided with the Code::Blocks IDE)
+ SDL version 1.2.15 which may be obtained here.
+ Pen and Paper (or the digital equivalent) for taking notes.

## Compiler Setup

### First things first.

The first thing we need to do is download and setup our compiler (Remember, we are using Mingw through the Code::Blocks IDE here) to use SDL. Head on over to the SDL download page and download **SDL-devel-1.2.15-mingw32.tar.gz** then extract the contents to your computer.

You should have a folder called **SDL-1.2.15** containing a bunch of files. I like to keep my computer tidy, and I create a root folder called "CLibs" in which I install all my libraries that I will use with the C and C++ programming languages. I would recommend that you do the same in order to avoid confusion later on.

We will only need the **include**, **lib**, and **bin** folders from the **SDL-1.2.15** folder. Go ahead and copy those folders into **C:\CLibs\SDL-1.2.15** and then we can move on over to configuring Code::Blocks so that it is able to find the SDL library files it needs.

Start up Code::Blocks and click on the **Settings** menu, then click on the **Compiler and Debugger...** menu item. In the window that opens, choose the **Search directories** tab, and under the **Compiler** sub-tab, you need to click on the **Add** button and browse for the **C:\CLibs\SDL-1.2.15\include** folder and click the **OK** button to add the folder to the compiler search directory paths. Now click on the **Linker** sub-tab and repeat the process for the **C:\CLibs\SDL-1.2.15\lib** folder. Click on the **OK** button and you are finished with telling Code::Blocks where the SDL files are located.

> A vital last step for Windows users is to copy the file **SDL.dll** into the bin directory in your compiler. In my case, on Windows 7, this happens to be **C:\Program Files (x86)\CodeBlocks\MinGW\bin**

### Testing the install.

We are going to write a very small program to test that Code::Blocks is correctly configured to use SDL.

**Step 1.**

Select **File** -> **New** -> **Project** from the menu in Code::Blocks, and then select **Empty project** from the templates and click on the **Go** button.

**Step 2.**

If you see "Welcome to the new empty project wizard!" just click on the **Next** button. Otherwise, if you see "Please select the folder where you want the new project to be created as well as its title." then type in a name for your project. In this case, we are testing SDL, so type in the name **SDLTest** into the **Project title** field and then click the **Next** button, and the **Finish** button on the next screen.

**Step 3.**

Select **File** -> **New** -> **Empty file** from the menu and click **Yes** when asked "Do you want to add this new file in the active project (has to be saved first)?" and enter **sdltest-main.cpp** as the file name, and save the file to the SDLTest folder created along with your new project. When asked to select the targets, just click the **OK** button.

**Step 4.**

Finally we get to write a little code. Enter each line exactly as you see it. I will not explain this test code, because all we are looking for is a yay or nay about SDL being configured correctly, and this article's purpose is not to go over basic code.

```c++
#include <SDL.h>

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return -1;
  }

  SDL_Quit();

  return 0;
}
```

**Step 5.**

Last thing, we need to specify the SDL libraries so we can compile the project. Right-click on the **SDLTest** item in the **Projects** panel and select **Build options...** then select the **Linker settings** tab. Under the **Other linker options** box, type in the following items each on their own line.

```
-lmingw32
-lSDLmain
-lSDL
```

*Take note that each line begins with minus sign then a lowercase letter L*.

**Step 6.**

Well, I know I said last thing in the previous step, but this really is the last thing. Build the project and Run the project. Select **Build** -> **Build** from the menu and as long as there are no errors, select **Build** -> **Run** from the menu. If you have any errors, go back to Step 1 and read through and make sure you have done everything as specified. If you continue to have trouble, then contact me with the full error you are having, and I will see if I can help you get things sorted out.

You should get a black console window appear and it should say

```
Process returned 0 (0x0) execution time : 0.201 s Press any key to continue.
```

If this is not the case, then something went wrong. If you see the text like above, then you are ready to proceed further into this article.

### All system are go.

Okay great, SDL is installed, Code::Blocks is configured, you know how to get an SDL project created and compiled, we're good to move on to something a little more exciting. We are going to take a small step away from the code and have a look at some pretty pictures to help you understand what we are going to be doing over the next few sections of the article.

![The game loop](https://www.gamedev.net/uploads/monthly_05_2013/ccs-137814-0-70750100-1369527491.png)

This is called the game-loop. Every single game has something like this inside of it. From Pong to Halo to Farmville to World of Warcraft. (Disclaimer: These games and their names are copyright and/or trademarks of their respective owners.) **Every Single Game Has A Game Loop** of some kind. This fun little loop is what keeps the game going. If nothing ever looped, the game would start, and then they would just about immediately end. That would be really boring. So let's see what exactly is going on here. First we do a little housekeeping, like handling events and what-not, next we update the game world, finally we render the game world, and the whole thing repeats until something tells it not to.

Now, there are hundreds if not thousands of articles and books out there that will explain this in at least a hundred different ways in more detail. I'm not going to add to that total with this article. Instead I am going to jump ahead right into our next project which is called **Big Blue Box** *(Disclaimer: I did not check to see if Big Blue Box is trademarked or copyrighted to anyone. If it is, sorry for using your name.)* in which our job as a programmer is to make a big blue box appear on the screen. Fun? Nah... but we have to start somewhere before we can get to the fun stuff.

## SDL Basics

### Beginnings of Big Blue Box

Just like the **SDLTest** project, **create a new project** and **configure the linker settings** so the project will compile, and also **throw in a .cpp file** into the mix so we have somewhere to write our code. I'm calling my project **BigBlueBox** and the source file **bigbluebox-main.cpp** you can do the same, or change the name. Your choice.

Okay, first we need some include statements and then we need to define some constants for our little program.

```c++
#include <iostream>
#include <SDL.h>

static const unsigned WINDOW_WIDTH = 800;
static const unsigned WINDOW_HEIGHT = 600;
static const unsigned BOX_WIDTH = 256;
static const unsigned BOX_HEIGHT = 256;
```

Everything should be self-explanatory. Our window will be **800x600** and the box will be **256x256**. Simple enough. We need **iostream** because we will be using the *cerr* error output stream for error reporting and naturally we need **SDL.h** because we are writing a program which uses SDL. Next, we write the main code.

```c++
int main(int argc, char* argv[]) {
  return 0;
}
```

This should be pretty much stuck in your brain if you have had any programming experience with C and C++. The rest of our program's code will fall between the starting brace of the function and the return zero line.

I usually declare my variables when they are used, however in this particular project I will declare all the variables I will use at the top of the function just so that you can refer to a single location to see the datatype of a variable being used.

We are going to need a pointer to an SDL Surface, to represent our primary drawing surface and our program's window. We need an SDL Event object for our event handling. We need an SDL Rect object to define our box's position and dimensions. We need a couple unsigned integers for our box color and the color to make the background, and finally we need a boolean variable for keeping track of our game loop. Let's see the variable declarations now.

```c++
SDL_Surface* displaySurface = NULL;
SDL_Event sdlEvent;
SDL_Rect boxRect;
unsigned boxColor;
unsigned backgroundColor;
bool isRunning;
```

The next thing we need to do is initialize SDL. This is accomplished with the SDL_Init function. Then we need to set the video mode to create our program's window. This is accomplished with the SDL_SetVideoMode function. We then set the window caption text to something relevant to our program. This is accomplished with the SDL_WM_SetCaption function. If we don't do this, the caption will be "SDL_app". Let's have a look at the code for all of this now.

```c++
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
```

Next we are going to set up our box to be the right size using our constants we defined at the start of our code, and then we will position our box in the center of the screen.

> Tip: To center an object on the screen, first subtract the object's size from the size of the screen and then you divide the difference by two.

```c++
boxRect.w = BOX_WIDTH;
boxRect.h = BOX_HEIGHT;
boxRect.x = (WINDOW_WIDTH - boxRect.w) / 2;
boxRect.y = (WINDOW_HEIGHT - boxRect.h) / 2;
```

Now we are going to set up our color variables. We want obviously a blue box. And I want a black background, so we are going to use the SDL_MapRGB function to get the right color values. Lastly, we set our running boolean to true.

```c++
backgroundColor = SDL_MapRGB(displaySurface->format, 0, 0, 0);
boxColor = SDL_MapRGB(displaySurface->format, 0, 0, 255);
```

We finally arrive at the game loop. Our game loop will be very simple, since our program is not really a game, we don't have much to do here. We just need to handle a couple events and then draw our screen.

```c++
isRunning = true;

while (isRunning) {
  // handle events

  // render game
}
```

The most complex part of this will be handling the events, and honestly, this is not really that difficult. We need to handle the SDL_QUIT event, which is sent when the user tries to close the window. If we don't handle this event, well the window would refuse to close and your user will be pretty irritated with you.

> Tip: ALWAYS remember to handle the SDL_QUIT event or your users will be very unhappy with you and your game.

Handling the event is pretty simple really. We just need to tell our game loop to stop looping, in our case, we just set our running boolean variable to false. Easy peasy. We would also like to be able to close our game by pressing the ESC key on the keyboard. To accomplish this feat, we need to handle the SDL_KEYDOWN event and check for the SDLK_ESCAPE key, and same as the SDL_QUIT handler, tell our game loop to stop looping. That wraps up the event handling we need to do here.

Now, that we know what events we have to handle, we need to know how to actually find out what events are being fired off. To do this, we need to call the SDL_PollEvent function inside a loop so that we can handle all events that get queued up when our game is running. Let's have a look at the code for our event handling. Remember that this code falls within our game loop.

```c++
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
```

Great, now the only thing left to wrap up our game loop is the rendering of our game screen. We will use the SDL_FillRect function to color our background, and to draw our box on the screen. After everything gets drawn, we need to call the SDL_Flip function in order to make our rendering visible on the screen because we are using double buffering (and we definitely want to be using double buffering). And lastly in order to keep our CPU happy, we will call the SDL_Delay function to give a few time slices back to the system.

```c++
SDL_FillRect(displaySurface, NULL, backgroundColor);
SDL_FillRect(displaySurface, &boxRect, boxColor);
SDL_Flip(displaySurface);

SDL_Delay(20);
```

Our game loop is done. The rest of the code that follows should be outside the game loop's closing brace. In order to wrap up the game code, we need to clean up after ourselves. Because our game is very simple and we don't allocate anything other than the display surface, we can clean up by calling the SDL_Quit function.

```c++
SDL_Quit();
```

And we are done! Now go ahead and build and run the project. You should see a big blue box on a black screen. Press the ESC key on your keyboard and the window should close.

![Behold the Big Blue Box](https://www.gamedev.net/uploads/monthly_05_2013/ccs-137814-0-25440700-1369527491.png)

> **UPDATE** Here is the full source code for Big Blue Box

```c++
#include <iostream>
#include <SDL.h>

static const unsigned WINDOW_WIDTH = 800;
static const unsigned WINDOW_HEIGHT = 600;
static const unsigned BOX_WIDTH = 256;
static const unsigned BOX_HEIGHT = 256;

int main(int argc, char* argv[]) {
  SDL_Surface* displaySurface = NULL;
  SDL_Event sdlEvent;
  SDL_Rect boxRect;
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

    SDL_FillRect(displaySurface, NULL, backgroundColor);
    SDL_FillRect(displaySurface, &boxRect, boxColor);
    SDL_Flip(displaySurface);

    SDL_Delay(20);
  }

  SDL_Quit();

  return 0;
}
```

### Moving Boxes.

Our next little project will build on the **BigBlueBox** project and we will add the ability to move our box around the screen using the arrow keys. We are going to need to add a couple additional variables to the top of our main function. We need an SDL_Rect to define the boundaries in which we can move our box, and an unsigned integer that defines the speed at which the box is able to move.

```c++
SDL_Rect boxBounds;
unsigned boxSpeed;
```

Now after we set the running boolean to true, before we start the game loop, we need to set up our bounding box rect. There are four variables in an SDL_Rect which we are going to use to specify the left, top, right, and bottom edges of a bounding box in which our box will be able to move. We subtract the size of our box from the size of our window to get the limits of the upper-left corner of our box.

```c++
boxSpeed = 6;

boxBounds.x = 0;
boxBounds.y = 0;
boxBounds.w = WINDOW_WIDTH - boxRect.w;
boxBounds.h = WINDOW_HEIGHT - boxRect.h;
```

We now are going to check if any of the arrow keys are being pressed, and if they are being pressed, move our box by adding or subtracting the box speed. We also need to ensure the box does not move outside out bounding box we have defined. But first, we have to have a way to read directly the current state of the keyboard. We are able to accomplish this task with the SDL_GetKeyState function. The code should be added to the game loop after the event handling loop and before the code that handles the rendering.

```c++
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
```

Now that takes care of the UP and DOWN movement, so let's take care of the LEFT and RIGHT next.

```c++
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
```

And that is all we are going to need to do. You should be able to build and run the project as before, and this time you can move the box around the screen using the arrow keys, and the box should not move beyond the edges of the screen. We have now covered enough of the basics that you should have enough knowledge of using SDL at this point to start writing your Snake game. So I'm done here.

...just kidding. Well, just about the last part. I'm not done here. I still have to teach you about programming Snake, since that is what this article is really about. This has all just been a crash-course introduction to get your appetite whetted. Now, before we can code our Snake game, we have to talk about how we are going to engineer our game code.

> **UPDATE** Here is the full source code to the updated Big Blue Box project with input handling

```c++
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
```

## A Look At Snake

I am going to use a very simple design for our little Snake game. For brevity and to help keep the focus on the core subject, I will not be implementing sound, I will not be using any external bitmaps, I will not implement scoring or text output, and I will not implement any sort of game state management or anything that would otherwise further complicate the program. This article is not about how to go about creating a polished, retail-ready game. It is about how to program a Snake game, and that is all I am going to do here.

For our game, we need two objects. The Snake, and a "Collectable Coin" which causes the Snake to grow in length. The rules are simple as well. If the Snake collides with the edge of the screen or itself, then the Snake will die. The player uses the Arrow keys to control the Snake in any of the four directions; Up, Down, Left, and Right. The Snake cannot be made to move in the opposite direction of it's movement, as that would cause the snake to bite itself instantly causing death. We will use simple filled rectangles to represent the various visuals of the game.

The code will consist of three classes and the main() function which houses the sample init and game loop code you have seen in the BigBlueBox project. The body of the Snake will be composed of multiple pieces, referred to as segments which will be held in a deque, or double-ended queue which can be found in the C++ STL. You can read more about deques here.

In order to make the Snake move, we will prepend a segment positioned in the location where the Snake is going to move, and if the Snake is moving into a location where there is no "Collectable Coin", the tail end of the Snake is chopped off. Using a deque makes this process straightforward through the push_front and pop_back methods. In order to cause the Snake to grow in length, we do not chop off the tail if we collide with a "Collectable Coin".

The game screen is divided into a number of "Cells". A single segment and the "Collectable Coin" are both sized the same as a single Cell. The Snake begins in the center of the screen. This should be enough information for us to start writing the code for our little Snake game.

### Building Snake

Finally, we get to have some fun programming. In order for us to begin, we will need to include a few header files, and then define our constants. We will need iostream for using cerr for our error reporting, deque to contain our Snake Segments, and of course SDL.h to access the SDL library functionality which powers our game.

We have our window dimensions, the four directions the Snake can move, and some constants to define the scale of the game world, and lastly the starting location and length of the Snake are defined as constants as well.

```c++
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
```

The first class in our little game is nothing more than a data structure that keeps track of the position of each segment of the snake.

```c++
class SnakeSegment {
  public:
    unsigned x;
    unsigned y;

    SnakeSegment(unsigned x, unsigned y) {
      this->x = x;
      this->y = y;
    }
};
```

Next up we have the Coin class which keeps track of the position of the coin. The same data as the SnakeSegment, however I wanted to have two distinct classes. The only notable difference between the SnakeSegment class and the Coin class is a method which randomly sets the position of the "Collectable Coin".

```c++
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
```

The third class of our game is where the majority of the work is located, and is also inevitably the most complex. The Snake class is a composite of the previous classes along with a few members necessary to implement the desired mechanics. We need to keep track of the direction the Snake is moving, we need a timing mechanism to keep from calling our collision detection code every frame, and a few colors. I also reuse a single SDL_Rect object for all the rendering to minimize the allocations and deallocations per-frame. We need methods to restart the game, update the game, render the game, determine if the game is over, and handle player input and collisions with the walls, the "Collectable Coin" and the snake itself.

```c++
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
```

Before I get into the code that implements the Snake class methods, I want to show you the main function and how the Snake class is used to create the gameplay flow. You have already seen most of the code in the BigBlueBox project, however there are a few small changes in the code arrangement. The layout is still the same: Init, Game Loop (Handle Events, Update, Render), Cleanup.

In the Init section of the main function, the following code should be written. If it is not immediately obvious, the background color is dark green, the head of the snake is red, the body of the snake is bright green, and the coin is yellow.

```
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
```

In the SDL_KEYDOWN section of the event handling code of the Game Loop, after the code that handles SDLK_ESCAPE, the following code needs to be added to allow the player to restart the game after dying by pressing the ENTER (RETURN) key.

```c++
else if (sdlEvent.key.keysym.sym == SDLK_RETURN) {
  if (snake.isDead()) {
    snake.restart();
  }
}
```

Simply call the Update method of the snake for the Update section of the Game Loop, and call the Render method in the Render section. That wraps up the code for the main function. Next we will take a look at the method implementations of the Snake class.

> Be sure to call the Render method AFTER calling the SDL_FillRect function, and BEFORE calling the SDL_Flip function.

> **UPDATE** Here is the source for snake.cpp up to this point for clarity. Note, it will not compile yet because there is still code missing that will be fleshed out later in the tutorial.

```c++
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
```

### Constructing the Snake

First up is the class constructor. Most of the construction process has been delegated to the Restart method, to avoid code duplication. The constructor consists of setting the colors and then calling the Restart function to finish the construction.

```c++
Snake::Snake(unsigned headColor, unsigned segmentColor, unsigned coinColor) {
  this->headColor = headColor;
  this->segmentColor = segmentColor;
  this->coinColor = coinColor;

  this->restart();
}
```

### Destructing the Snake

The class destructor just needs to clear out the deque since we do not have any dynamic memory allocations which need to be freed.

```c++
Snake::~Snake() {
  this->segments.clear();
}
```

### Restarting the Snake

As mentioned before, the construction of the Snake has been delegated to the Restart method to avoid code duplication. When the Snake is restarted, the deque is cleared out, then the starting snake segments are added to the deque. The direction of movement is reset as well as the dead flag and the timing mechanism.

```c++
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
```

### Adding Snake Segments

We add segments to the tail of the starting Snake by pushing to the back of the deque. This only occurs when building the initial Snake body in the Restart method.

```c++
void Snake::addSegment(unsigned x, unsigned y) {
  SnakeSegment segment(x, y);

  this->segments.push_back(segment);
}
```

### Updating the Snake

If the Snake is dead, we do not continue the update. First we update the input controls, then update the timing mechanism. If the time has not timed out, we do not continue the update. If the time has timed out, we check for collisions and finally move the snake.

```c++
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
```

### Controlling the Snake

We do not want the Snake to be able to reverse its direction of movement, so our movement code is a little more than just if key pressed, then move. But nothing too difficult.

```c++
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
```

### When the Snake hits the Wall

Checking for collision between the head of the Snake and a "Wall" is a straightforward process of testing if the head of the Snake has reached any of the four edges of the game world.

```c++
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
```

### Watch Out For Your Tail

If the Snake's head touches another segment of the Snake body, it dies from the poison. Just loop through the deque starting at the segment after the head and if the head is in the same location as the iterated segment, you have a collision.

```c++
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
```

### Collecting Coins

This is the easiest collision check of them all. If the head touches the coin, you collect the coin. Pretty easy stuff after all the code we have written so far.

```c++
bool Snake::checkForCoinCollision() {
  unsigned headX = this->segments[0].x;
  unsigned headY = this->segments[0].y;

  return this->coin.x == headX && this->coin.y == headY;
}
```

### Moving the Snake

In order to make the Snake appear to move, we push a new head segment to the front of the deque. Since the Update method takes care of popping the back end segment off of the deque, this gives the illusion that the segments are following the head segment, which looks like the Snake moves as an intelligent entity.

```c++
void Snake::moveSnake() {
  static const int moveX[] = { 0, 0, -1, 1 };
  static const int moveY[] = { -1, 1, 0, 0 };

  unsigned x = this->segments[0].x + moveX[this->direction];
  unsigned y = this->segments[0].y + moveY[this->direction];

  SnakeSegment segment(x, y);

  this->segments.push_front(segment);
}
```

### Drawing the Snake

If the Snake is dead, we do not continue the rendering process. Coins are rendered first, then the Snake itself. Since all visuals are the same size rectangle, we are able to reuse a single SDL_Rect structure and simply update the x and y values for each component to be rendered in the correct location. Every x and y value needs to be multiplied by the size of a single "Cell" in the game world, so that the on-screen positions line up correctly.

```c++
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
```

### The End is Here

![Snake](https://www.gamedev.net/uploads/monthly_05_2013/ccs-137814-0-34657600-1369527492.png)

With that, our little Snake game is complete. Go ahead of build and run the project and you should get a dark green screen with a red & green snake that you can control using the arrow keys to collect yellow coins that randomly appear after you collect each one. If you bite yourself or hit a wall, you die, and nothing will be drawn but the dark green screen. Pressing the Enter key will restart the game, and pressing ESC will quit the game.

If you have any problems with the project, or would like to discuss the project. Feel free to contact me. My email address is ccpsceo@gmail.com. **Please include "Programming Snake" in your subject when emailing me.** You may also contact me via Skype by adding the user bbastudios and when asking for me to add you, **include that you have read this Programming Snake article**

Thank you for reading. I hope you have enjoyed this as much as I have.

Sincerely,

Richard Marks

Senior Software Engineer

http://www.ccpssolutions.com


> **Update** Here is the full source code for the Snake game.

```c++
/*
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
```
