#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include<SDL2_image/SDL_image.h>
#include<SDL2_mixer/SDL_mixer.h>
#include<SDL2_ttf/SDL_ttf.h>
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include <unistd.h>
#endif



#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)

#include<direct.h>
#define getcwd _getcwd

#endif

#include <iostream>
#include <math.h>
#include <stdio.h>
#include<string>
#include<vector>
#include<stdint.h>

#include "tank.h"
#include "turret.h"
using namespace std;

#if defined(__APPLE__)

string currentWorkingDirectory(getcwd(NULL,0));

string images_dir = currentWorkingDirectory +"/src/";

string audio_dir = currentWorkingDirectory + "/src/";
#endif

#if defined(_WIN32) || (_WIN64)


string s_cwd(getcwd(NULL, 0));

//create a string linking to the mac's image folder
string images_dir = s_cwd + "\\src\\";

string audio_dir = s_cwd + "\\src\\";
#endif

#if defined(__linux__)

string currentWorkingDirectory(getcwd(NULL,0));

string images_dir = currentWorkingDirectory +"/src/";

string audio_dir = currentWorkingDirectory + "/src/";
#endif

TTF_Font *font;

SDL_Color colorP1 = {255,255,255,255};
SDL_Surface *playerSurface, *turretSurface;

SDL_Texture *playerTexture, *turretTexture;

SDL_Rect playerPos, turretPos;

int playerHealth = 100;

string tempText = "";

void PlayerText(SDL_Renderer *renderer){
	string Result;
	ostringstream convert;
	convert << playerHealth;
	Result = convert.str();

	tempText = "Player Health: " + Result;

	//surface for font string

	playerSurface = TTF_RenderText_Solid(font, tempText.c_str(), colorP1);

	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);

	SDL_QueryTexture(playerTexture, NULL ,NULL, &playerPos.w, &playerPos.h);

	SDL_FreeSurface(playerSurface);

}

void TurretText(SDL_Renderer *renderer, int turretNum){
	string Result;
	ostringstream convert;
	convert << turretNum;
	Result = convert.str();
	tempText = "Turret " + Result + "was the last hit..";

	if(turretNum == 0){
		tempText = "No Turret was hit...";
	}

	turretSurface = TTF_RenderText_Solid(font, tempText.c_str(),colorP1);

	turretTexture = SDL_CreateTextureFromSurface(renderer, turretSurface);

	SDL_QueryTexture(turretTexture, NULL, NULL , &TurretPos.w,&TurretPos.h);
}






float deltaTime=0.0;
int thisTime = 0;
int lastTime = 0.0;

int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window;

	SDL_Renderer *renderer;

	window = SDL_CreateWindow("Tank Wars!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1024,768,
			SDL_WINDOW_SHOWN);


	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

	bool quit = false;

	SDL_Event e;

	SDL_GameControllerEventState(SDL_ENABLE);

	SDL_GameController*  gGameController0= NULL;

	gGameController0 = SDL_GameControllerOpen(0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	SDL_Rect bkgdRect;
	bkgdRect.x=0;
	bkgdRect.y=0;
	bkgdRect.w=2048;
	bkgdRect.h=1536;

	TTF_Init();

	font = TTF_OpenFont((images_dir + "comic.ttf").c_str(),40);

	playerPos.x=10;
	playerPos.y=10;

	turretPos.x = 600;
	turretPos.y = 10;

	PlayerText(renderer);

	TurretText(renderer, 0);



	float x_pos = 0.0f;
	float y_pos = 0.0f;

	Tank tank1 = Tank(renderer, 0 ,images_dir.c_str(),audio_dir.c_str(),50.0f,50.0f);

	Turret turret1 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),800.0f,500.0f);
	Turret turret2 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),1600.0f,250.0f);
	Turret turret3 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),400.0f,1000.0f);
	Turret turret4 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),1600.0f,1250.0f);


	SDL_Texture *bkgd = IMG_LoadTexture(renderer,(images_dir+"background.png").c_str());

	while (!quit){
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime-lastTime)/1000;
		lastTime = thisTime;

		while(SDL_PollEvent(&e)!=0)
		{
			if(e.type==SDL_QUIT){
				quit=true;

			}

			switch(e.type){

			case SDL_CONTROLLERBUTTONDOWN:
				if(e.cdevice.which ==0){

					if(e.cbutton.button==SDL_CONTROLLER_BUTTON_A){

						tank1.OnControllerButton(e.cbutton);
					break;
					}
				}
				break;
				case SDL_CONTROLLERAXISMOTION:

					//tank1.OnControllerAxis(e.caxis);

					break;
			}
		}

		const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
		const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

		tank1.OnControllerAxis(Xvalue,Yvalue);
		tank1.Update(deltaTime);

		if((tank1.posRect.x >= 1024 - tank1.posRect.w) && (tank1.Xvalue > 8000)){

			x_pos -= (tank1.speed) * deltaTime;

			if((bkgdRect.x > -1024)){

				bkgdRect.x=(int)(x_pos + 0.5f);
				turret1.TankMoveX(-tank1.speed, deltaTime);
				turret2.TankMoveX(-tank1.speed, deltaTime);
				turret3.TankMoveX(-tank1.speed, deltaTime);
				turret4.TankMoveX(-tank1.speed, deltaTime);
			}
			else{
						x_pos = bkgdRect.x;
			}
		}


			if((tank1.posRect.x <= 0) && (tank1.Xvalue < 8000)){

			x_pos += (tank1.speed)*deltaTime;

			if((bkgdRect.x < 0)){

				bkgdRect.x=(int)(x_pos + 0.5f);

				turret1.TankMoveX(tank1.speed, deltaTime);
				turret2.TankMoveX(tank1.speed, deltaTime);
				turret3.TankMoveX(tank1.speed, deltaTime);
				turret4.TankMoveX(tank1.speed, deltaTime);
			}else{
				x_pos = bkgdRect.x;
			}



		}

			if((tank1.posRect.y >= 768 - tank1.posRect.h) && (tank1.Yvalue > 8000)){

				y_pos -= (tank1.speed) * deltaTime;

				if((bkgdRect.y > -768)){

					bkgdRect.y=(int)(y_pos + 0.5f);
					turret1.TankMoveY(-tank1.speed, deltaTime);
					turret2.TankMoveY(-tank1.speed, deltaTime);
					turret3.TankMoveY(-tank1.speed, deltaTime);
					turret4.TankMoveY(-tank1.speed, deltaTime);

				}
				else{
							y_pos = bkgdRect.y;
				}
			}


				if((tank1.posRect.y <= 0) && (tank1.Yvalue < 8000)){

				y_pos += (tank1.speed)*deltaTime;

				if((bkgdRect.y < 0)){

					bkgdRect.y=(int)(y_pos + 0.5f);
					turret1.TankMoveY(tank1.speed, deltaTime);
					turret2.TankMoveY(tank1.speed, deltaTime);
					turret3.TankMoveY(tank1.speed, deltaTime);
					turret4.TankMoveY(tank1.speed, deltaTime);
				}else{
					y_pos = bkgdRect.y;
				}



			}

		turret1.Update(deltaTime, tank1.posRect);
		turret2.Update(deltaTime, tank1.posRect);
		turret3.Update(deltaTime, tank1.posRect);
		turret4.Update(deltaTime, tank1.posRect);


		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer,bkgd, NULL, &bkgdRect);



		tank1.Draw(renderer);

		turret1.Draw(renderer);
		turret2.Draw(renderer);
		turret3.Draw(renderer);
		turret4.Draw(renderer);
		//turret1.Draw(renderer);

		SDL_RenderCopy(renderer, playerTexture, NULL , &PlayerPos);
		SDL_RenderCopy(renderer, turretTexture, NULL , &TurretPos);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);

	SDL_Quit();


	return 0;
}
