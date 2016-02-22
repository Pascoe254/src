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

	float x_pos = 0.0f;
	float y_pos = 0.0f;

	Tank tank1 = Tank(renderer, 0 ,images_dir.c_str(),audio_dir.c_str(),50.0f,50.0f);

	Turret turret1 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),800.0f,500.0f);

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
			}
			else{
						x_pos = bkgdRect.x;
			}
		}


			if((tank1.posRect.x <= 0) && (tank1.Xvalue < 8000)){

			x_pos += (tank1.speed)*deltaTime;

			if((bkgdRect.x < 0)){

				bkgdRect.x=(int)(x_pos + 0.5f);
			}else{
				x_pos = bkgdRect.x;
			}



		}

			if((tank1.posRect.y >= 768 - tank1.posRect.h) && (tank1.Yvalue > 8000)){

				y_pos -= (tank1.speed) * deltaTime;

				if((bkgdRect.y > -768)){

					bkgdRect.y=(int)(y_pos + 0.5f);
				}
				else{
							y_pos = bkgdRect.y;
				}
			}


				if((tank1.posRect.y <= 0) && (tank1.Yvalue < 8000)){

				y_pos += (tank1.speed)*deltaTime;

				if((bkgdRect.y < 0)){

					bkgdRect.y=(int)(y_pos + 0.5f);
				}else{
					y_pos = bkgdRect.y;
				}



			}

		turret1.Update(deltaTime, tank1.posRect);


		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer,bkgd, NULL, &bkgdRect);



		tank1.Draw(renderer);

		turret1.Draw(renderer);
		//turret1.Draw(renderer);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);

	SDL_Quit();


	return 0;
}
