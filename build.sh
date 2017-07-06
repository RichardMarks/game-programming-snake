#!/bin/sh

# create the build directory if it does not exist
if [ ! -d 'build' ]
then
  mkdir build
fi

# compile and link executables
g++ -o build/sdltest src/sdltest-main.cpp `sdl-config --cflags --libs`

g++ -o build/bigbluebox src/bigbluebox-main.cpp `sdl-config --cflags --libs`

g++ -o build/bigbluebox2 src/bigbluebox2-main.cpp `sdl-config --cflags --libs`

g++ -o build/snake src/snake-main.cpp `sdl-config --cflags --libs`
