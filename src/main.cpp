
#include "game.h"

int main(int argc, char* args[])
{
	Game game;
	if (game.init()) 
	{
		while(game.mainLoop());
	}
	return 0;
}