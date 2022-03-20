//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <string>
#include "Game.h"

int main( int argc, char* args[] )
{
	//game class, contains all variables and all the code
	Game* theGame = new Game();	
	
	//game loop
	while (!theGame->Go()) {}

	return 0;
}