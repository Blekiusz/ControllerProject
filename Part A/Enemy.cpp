#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
	position.x = 100;
	position.y = 100;
}


Enemy::~Enemy()
{
}

//Function removes green background from the picture creates a texture and sets ups variables used because of issues caused by SDL_image
void Enemy::LoadImage(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path)
{
	image = SDL_LoadBMP(image_path);
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 0));
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	rect = new SDL_Rect({ pos_x - (w / 2), pos_y - (h / 2), w, h });
	center = new SDL_Point{ rect->w / 2, rect->h / 2 };
}

//Function renders the picture and rotates it if needed
void Enemy::Render(SDL_Renderer * renderer, double rotation)
{
	SDL_RenderCopyEx(renderer, texture, NULL, rect, rotation, center, SDL_FLIP_NONE);
}

//function respawns enemy off screen giving it new random speed
void Enemy::RandomSpawnOffScreen(Enemy * Enemy)
{
	Enemy->rect->x = rand() % 520;
	Enemy->rect->y = -100;
	speed = rand() % 7 + 3;
	
}
