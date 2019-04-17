#pragma once
#include "SDL.h"
#include <serial\serial.h>
#include "SerialInterface.h"

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

private:
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	bool isRunning;

	SerialInterface* serial;

	SDL_Rect playerPosition;
	SDL_Rect playerTwoPosition;
};

