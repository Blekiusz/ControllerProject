#include "stdafx.h"
#include "Game.h"
#include <iostream>

using std::cout;

/*
* Constructor - not used in this case
*
*/
Game::Game()
{
	
}

/*
	Decontructor - also not used in this case
*/
Game::~Game()
{
}

bool Game::initSerialConnection()
{
	serial = new SerialInterface();
	return true;
}
/*
* init - used to initialise and setup SDL
* Return true if everything is successful
*/
bool Game::init(const char * title, int xpos, int ypos, int width, int height, int flags)
{
	// initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL init success \n";

		// Create a window
		mainWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		// if window succesful..
		if (mainWindow != 0) {
			cout << "Window creation success \n";

			// create renderer
			mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);

			// if renderer successful...
			if (mainRenderer != 0) {
				cout << "Renderer creation success \n";
				SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
			}
			else {
				cout << "renderer failed \n";
				return false;
			}
		}
		else {
			cout << "window failed \n";
			return false;
		}

	}
	else {
		cout << "SDL fail \n";
		return false;
	}

	isRunning = true;
	cout << "SDL init success \n";

	initSerialConnection();

	playerPosition.x = 100;
	playerPosition.y = 100;
	playerPosition.w = 25;
	playerPosition.h = 25;

	playerTwoPosition.x = 300;
	playerTwoPosition.y = 100;
	playerTwoPosition.w = 25;
	playerTwoPosition.h = 25;
	return true;
}



/*
* handleEvents - Poll Events and uses switch case to process specific events
*
*/
void Game::handleEvents()
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_w)
			{
				playerPosition.y -= 5.0f;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				playerPosition.y += 5.0f;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				serial->led();
			}
			break;
		default:
			break;
		}

	}
}

/*
* update - Process all variables !in charge of anything to do with rendering
*
*/
void Game::update()
{
	serial->getPositions();
	playerPosition.y = serial->getPot1();
	playerTwoPosition.y = serial->getPot2();
}

void Game::render()
{
	// set background color
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);

	// clear previous frame
	SDL_RenderClear(mainRenderer);

	// draw to the screen here!
	SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(mainRenderer, &playerPosition);
	SDL_RenderFillRect(mainRenderer, &playerTwoPosition);

	// render new frame
	SDL_RenderPresent(mainRenderer);
}

/*
* clean - Clean up SDL and close the port
*
*/
void Game::clean()
{	
	cout << "Cleaning SDL \n";
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	SDL_Quit();
}


