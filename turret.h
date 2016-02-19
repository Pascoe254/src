
#include <string>
#include <iostream>

using namespace std;
#include <vector>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include<SDL2_image/SDL_image.h>
#include<SDL2_mixer/SDL_mixer.h>
#include<SDL2_ttf/SDL_ttf.h>
#endif


//#include "turretbullet.h"
class Turret{
public:
	bool active;
	//vector<TurretBullet> bulletList;

	string turretPath;

	SDL_Texture *tBase;
	SDL_Texture *tBarrel;

	SDL_Rect baseRect;
	SDL_Rect barrelRect;

	float x,y,turretangle;

	SDL_Point center;

	float fireTimel = 0.0f;
	float fireRate = 1000.0;

	Mix_Chunk *fire;

	Turret(SDL_Renderer *renderer, string filePath, string audioPath,float x,float y);

	void Update(float deltaTime, SDL_Rect tankRect);

	void Draw(SDL_Renderer *renderer);

	void Reset();

	void CreateBullet(SDL_Rect target);









};
