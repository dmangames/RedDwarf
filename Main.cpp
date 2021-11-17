/*Dmangames 2021*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include "InputHandler.h"

#include <stdlib.h>
#include <time.h>
#include "Graphics.h"
#include "World.h"
#include "AStar.hpp"
#include <iostream>

enum GameState {
	MAIN_MENU,
	IN_GAME,
	GAME_OVER,
	TEST
};


void mainMenu() {

}



int main(int argc, char* args[])
{

	InputHandler inputs;
	Graphics graphics;
	World world(graphics.get_width(), graphics.get_height());


	//Game state
	GameState gameState = MAIN_MENU;

	//GameState gameState = TEST;

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;



	//While application is running
	while (!quit)
	{
		switch (gameState) {
			case MAIN_MENU:
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					graphics.render_mainmenu();
					graphics.present_renderer();
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//User pressed any button while in main menu
					else if (e.type == SDL_KEYDOWN && gameState == MAIN_MENU) {
						if (e.key.keysym.sym == SDLK_SPACE)
							gameState = IN_GAME;
					}
				}
				break;
			case IN_GAME:
				
				inputs.update();
				quit = inputs.get_quit();

				world.update(&inputs);

				graphics.clear_screen();
				graphics.render_tiles(world.get_map(), world.get_delta(), world.get_camera());
				graphics.render_actors(world.get_actors(), world.get_delta(), world.get_camera());
				graphics.render_overlay();

				graphics.present_renderer(world.get_delta());
				break;
			case TEST:
				inputs.update();
				quit = inputs.get_quit();
				world.update(&inputs);

				break;
			default:
				break;
		}
				
	}


	return 0;
}