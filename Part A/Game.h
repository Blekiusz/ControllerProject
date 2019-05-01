#pragma once
#include "SDL.h"
#include <serial\serial.h>
#include "SerialInterface.h"
#include "Player.h"
#include "Enemy.h"
#include <list>
#include <SDL_ttf.h>
#include <string>

class Game
{
public:
	Game();
	~Game();

	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void render();
	void update();
	void handleEvents();
	void clean();

	bool initSerialConnection();
	bool running() { return isRunning; };
	int score = 0;
	bool previousstate;

private:
	int Menu = 0;//0 = main menu, 1 = game, 2 = game over screen
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	bool isRunning;
	Enemy * EnemyList[10];
	SerialInterface* serial;
	Player* User = new Player;
	SDL_Rect * AmmoBar = new SDL_Rect;

	SDL_Rect * Health = new SDL_Rect;
	TTF_Font * font;
};

