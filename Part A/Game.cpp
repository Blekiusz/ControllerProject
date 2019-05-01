#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "SDL_image.h"
#include <stdio.h>


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

	User->LoadImage(mainRenderer, User->position.x, User->position.y, "c.bmp");

	for (int i = 0; i < 10; i++)
	{
		EnemyList[i] = new Enemy;
		EnemyList[i]->LoadImage(mainRenderer, EnemyList[i]->position.x, EnemyList[i]->position.y, "e.bmp");
		EnemyList[i]->RandomSpawnOffScreen(EnemyList[i]);
	}
	User->Reload();

	AmmoBar->x = 580;
	AmmoBar->y = 600;
	AmmoBar->w = 20;

	Health->x = 0;
	Health->y = 600;
	Health->w = 20;

	Menu = 0;

	TTF_Init();
	font = TTF_OpenFont("arial.ttf", 24);
	return true;
}

/*
* Used to switch off the game with esc button
*/
void Game::handleEvents()
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
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
	//main menu functionalities
	if (Menu == 0) {
		if (!serial->getTrigger()) {
			Menu++;
			for (int i = 0; i < 10; i++)
			{
				EnemyList[i]->RandomSpawnOffScreen(EnemyList[i]);
			}
		}
	}

	//in game functions
	else if (Menu == 1) {
		User->rect->x += serial->getPositionX();
		User->rect->y += serial->getPositionY();
		if (0 > User->rect->x) {
			User->rect->x = 0;
		}
		else if (User->rect->x + User->rect->w > 600) {
			User->rect->x = 600 - User->rect->w;
		}
		if (0 > User->rect->y) {
			User->rect->y = 0;
		}
		else if (User->rect->y + User->rect->w > 600) {
			User->rect->y = 600 - User->rect->w;
		}
		if (!serial->getTrigger() && User->GetBullets() > 0) {
			User->ShootBullets();

			for (int i = 0; i < 10; i++)
			{
				bool x = SDL_HasIntersection(EnemyList[i]->rect, User->rect);
				if (x) {
					EnemyList[i]->RandomSpawnOffScreen(EnemyList[i]);
					score += 10;
				}
			}
		}

		if (serial->getIR() && !previousstate) { User->Reload(); }

		for (int i = 0; i < 10; i++)
		{
			EnemyList[i]->Move();
		}

		previousstate = serial->getIR();


		AmmoBar->h = (-600.f * (User->GetBullets() / 30.f));
		Health->h = (-600.f * (User->GetHealth() / 10.f));

		if (User->GetHealth() == 0) {
			Menu++;
			for (int i = 0; i < 10; i++)
			{
				User->GetHealed();
			}

		}
	}

	// game over screen
	else if (Menu == 2) {
		if (serial->getIR() && !previousstate) { Menu = 0; score = 0; }
		previousstate = serial->getIR();
	}
	
}

void Game::render()
{
	//shows image in main menu
	if (Menu == 0) {
		SDL_Surface* surface = SDL_LoadBMP("s.bmp");
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mainRenderer, surface);
		SDL_FreeSurface(surface);
		SDL_Rect destination;
		destination.x = 0;
		destination.y = 0;
		destination.w = 600;
		destination.h = 600;

		SDL_RenderCopy(mainRenderer, texture, NULL, &destination);
		SDL_RenderPresent(mainRenderer);

		SDL_DestroyTexture(texture);
	}

	//shows player, enemies, health and ammo bar and score
	else if (Menu == 1) {
		// set background color

		SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);

		// clear previous frame
		SDL_RenderClear(mainRenderer);

		// draw to the screen here!
		User->Render(mainRenderer, 45);
		for (int i = 0; i < 10; i++)
		{
			EnemyList[i]->Render(mainRenderer, 0);
			if (EnemyList[i]->rect->y > 600) {
				User->GetDamaged();
				EnemyList[i]->RandomSpawnOffScreen(EnemyList[i]);
			}
		}

		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);

		SDL_RenderFillRect(mainRenderer, AmmoBar);

		SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 255);

		SDL_RenderFillRect(mainRenderer, Health);

		//showing scores
		std::string scorestr = "Score: " + std::to_string(score);
		const char *array = scorestr.c_str();
		SDL_Color color = { 0,0,0 };

		SDL_Surface * surface = TTF_RenderText_Solid(font, array, color);
		SDL_Texture * texture = SDL_CreateTextureFromSurface(mainRenderer, surface);

		int textW = 0;
		int textH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		SDL_Rect dstrect = { 300 - (textW / 2), 0, textW, textH };

		SDL_RenderCopy(mainRenderer, texture, NULL, &dstrect);

		// render new frame
		SDL_RenderPresent(mainRenderer);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

	// shows background image, shows score achieved and information to reload to go to main menu
	else if (Menu == 2) {
		SDL_Surface* surface = SDL_LoadBMP("o.bmp");
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mainRenderer, surface);
		SDL_FreeSurface(surface);
		SDL_Rect destination;
		destination.x = 0;
		destination.y = 0;
		destination.w = 600;
		destination.h = 600;
		SDL_RenderCopy(mainRenderer, texture, NULL, &destination);

		std::string scorestr = "You have achieved score of: " + std::to_string(score);
		const char *array = scorestr.c_str();
		SDL_Color color = { 255,255,255 };

		SDL_Surface * nsurface = TTF_RenderText_Solid(font, array, color);
		SDL_Texture * ntexture = SDL_CreateTextureFromSurface(mainRenderer, nsurface);

		int textW = 0;
		int textH = 0;
		SDL_QueryTexture(ntexture, NULL, NULL, &textW, &textH);
		SDL_Rect dstrect = { 300 - (textW / 2), 400, textW, textH };

		SDL_RenderCopy(mainRenderer, ntexture, NULL, &dstrect);

		std::string m = "Reload to go to main menu.";
		const char *narray = m.c_str();

		SDL_Surface * nnsurface = TTF_RenderText_Solid(font, narray, color);
		SDL_Texture * nntexture = SDL_CreateTextureFromSurface(mainRenderer, nnsurface);

		int ntextW = 0;
		int ntextH = 0;
		SDL_QueryTexture(nntexture, NULL, NULL, &ntextW, &ntextH);
		SDL_Rect ddstrect = { 300 - (textW / 2), 500, ntextW, ntextH };


		SDL_RenderCopy(mainRenderer, nntexture, NULL, &ddstrect);

		SDL_RenderPresent(mainRenderer);

		SDL_DestroyTexture(texture);

		SDL_DestroyTexture(ntexture);
		SDL_FreeSurface(nsurface);

		SDL_DestroyTexture(nntexture);
		SDL_FreeSurface(nnsurface);
	}
}

/*
* clean - Clean up SDL and close the port
*
*/
void Game::clean()
{	
	cout << "Cleaning SDL \n";
	
	//TTF_Quit();
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	SDL_Quit();
}
