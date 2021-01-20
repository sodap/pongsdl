# pongsdl
A humble pong clone made in C89 using SDL2.

## Build with gcc
gcc \src\\*.c -w -Wl IC:\MinGW\include\SDL2 -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o sodapong.exe