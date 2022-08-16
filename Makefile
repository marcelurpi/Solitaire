all: default run

default:
	g++ src/* -g -o solitaire.exe resources.o -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
run:
	./solitaire.exe