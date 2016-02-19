#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include<SDL2_image/SDL_image.h>
#include<SDL2_mixer/SDL_mixer.h>
#include<SDL2_ttf/SDL_ttf.h>
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

float deltaTime=0.0;
int thisTime = 0;
int lastTime = 0.0;

int main(){

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = nullptr;

	SDL_Renderer *renderer = nullptr;

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

	Tank tank1 = Tank(renderer, 0 ,images_dir.c_str(),audio_dir.c_str(),50.0f,50.0f);

	Turret turret1 = Turret(renderer , images_dir.c_str(),audio_dir.c_str(),800.0f,500.0f);

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
					tank1.OnControllerAxis(e.caxis);
					break;
			}
		}

		tank1.Update(deltaTime);

		SDL_RenderClear(renderer);

		tank1.Draw(renderer);

		turret1.Draw(renderer);
		//turret1.Draw(renderer);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);

	SDL_Quit();


	return 0;
}
