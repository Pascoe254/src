#include "turret.h"

Turret::Turret(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y){
	active = true;

	fire = Mix_LoadWAV((audioPath+"fire.wav").c_str());

	string basePath = filePath + "turretBase.png";

	tBase = IMG_LoadTexture(renderer, basePath.c_str());

	string barrelPath = filePath + "turretBarrel.png";

	tBarrel = IMG_LoadTexture(renderer, barrelPath.c_str());

	baseRect.x=x;
	baseRect.y=y;

	int w,h;

	SDL_QueryTexture(tBase,NULL,NULL,&w,&h);

	baseRect.w=w;
	baseRect.h=h;

	barrelRect.x=x+34;
	barrelRect.y=y+34;

	SDL_QueryTexture(tBarrel,NULL,NULL,&w,&h);
	barrelRect.w=w;
	barrelRect.h=h;

	center.x=12;
	center.y=12;

	string bulletPath = filePath + "tBullet.png";

	for(int i = 0; i < 10; i++)
	{
		//TurretBullet tmpBullet(renderer,bulletPath, 500,500);

		//bulletList.push_back(tmpBullet);
	}
	srand(time(NULL));
}


void Turret::Draw(SDL_Renderer *renderer)
{
	/*
	 * for(int i = 0; i < bulletList.size();i++)
	 * {
	 * 		if(bulletList[i].active{
	 * 			bulletList[i].Draw(renderer);
	 * 		}
	 *
	 * {
	 */
	SDL_RenderCopy(renderer, tBase, NULL, &baseRect);

	SDL_RenderCopyEx(renderer, tBarrel, NULL , &barrelRect, turretangle, &center, SDL_FLIP_NONE);

}


