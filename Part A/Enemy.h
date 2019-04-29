#pragma once
#include "SDL.h"
#include <stdlib.h>

class Enemy
{
public:
	Enemy();
	~Enemy();

	SDL_Surface * image;
	SDL_Texture * texture;
	SDL_Rect * rect;
	SDL_Point * center;
	void LoadImage(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path);
	void Render(SDL_Renderer * renderer, double rotation);

	SDL_Rect position;


	void RandomSpawnOffScreen(Enemy * Enemy);
	void Move() { rect->y += speed; }
	int speed = 3;

};