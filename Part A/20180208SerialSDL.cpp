// 20180208SerialSDL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDL.h"
#include "Game.h"
#define FPS 1000/60

Game* game = 0;
static Uint32 next_wait_time;

Uint32 time_left(void)
{
	/**
	Returns the time needed to wait before moving onto the next frame
	*/
	Uint32 current_ticks = SDL_GetTicks();

	if (next_wait_time <= current_ticks)
		return 0;
	else
		return next_wait_time - current_ticks;
}

int main(int argc, char* argv[])
{
	// new game instance
	game = new Game();

	// initialise the game
	game->init("SDL Serial", 100, 100, 600, 600, 0);
	

	next_wait_time = SDL_GetTicks() + FPS;

	// create a game loop
	while (game->running()) 
	{
		game->handleEvents();
		game->update();
		game->render();

		SDL_Delay(time_left());
		next_wait_time += FPS;
	}

	// when the game ends clean SDL and Serial
	game->clean();

    return 0;
}

