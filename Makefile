all:
	g++ src/* -o main.exe -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
run:
	./main.exe