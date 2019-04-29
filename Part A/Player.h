#pragma once
#include "SDL.h"

class Player
{
public:
	Player();
	~Player();

	SDL_Surface * image;
	SDL_Texture * texture;
	SDL_Rect * rect;
	SDL_Point * center;
	void LoadImage(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path);
	void Render(SDL_Renderer * renderer, double rotation);

	void GetDamaged() { health--; }
	void GetHealed() { health++; }
	int GetBullets() { return bullets; }
	int GetHealth() { return health; }
	void ShootBullets() { bullets--; }
	void Reload() { bullets = maxbullets; }

	SDL_Rect position;

private:

	const int maxhealth = 10;
	const int maxbullets = 30;
	int health = 10;
	int bullets = 30;

};

