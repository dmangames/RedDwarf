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

enum GameState {
	MAIN_MENU,
	IN_GAME,
	GAME_OVER
};
/*
////CLASS HEADERS////
/////////////////////

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Main menu texture
LTexture gMainMenuTexture;

//Globally used font
TTF_Font* gFont = NULL;

//Rendered text texture
LTexture gTextTexture;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//END CLASS HEADERS////////
///////////////////////////

GameState gameState = MAIN_MENU;





//input handler
InputHandler* inputHandler = NULL;

//Player actor
GameActor* player = NULL;

//Current animation frame
int frame = 0;





bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	//Init input handler
	inputHandler = new InputHandler();

	//Init player
	player = new GameActor("Player");

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	

	if (!gMainMenuTexture.loadFromFile("images/red_dwarf_main_menu_art.png")) {
		printf("Failed to load main menu texture!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{

	}

	return success;
}

void close()
{
	//Free loaded images
	gMainMenuTexture.free();
	gTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Release input handler
	delete inputHandler;
	inputHandler = NULL;

	//Release player
	delete player;
	player = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void mainMenu() {
	gMainMenuTexture.render(0, 0);

	//Render text
	SDL_Color textColor = { 255, 255, 255 };
	gTextTexture.loadFromRenderedText("Press any key to start", textColor);
	gTextTexture.render(500, 500);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void playGame() {
	Command* command = inputHandler->handleInput();

	if (command) {
		command->execute(*player);
	}

	//Set red dwarf facing direction based on key state
	SDL_RendererFlip direction = SDL_FLIP_NONE;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		direction = SDL_FLIP_NONE;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		direction = SDL_FLIP_HORIZONTAL;
	}

	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);



	//Update screen
	SDL_RenderPresent(gRenderer);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 4 >= WALKING_ANIMATION_FRAMES)
	{
		frame = 0;
	}
}
*/

void mainMenu() {

}



int main(int argc, char* args[])
{
	// Init random seed
	srand(time(NULL));

	InputHandler inputs;
	Graphics graphics;
	World world(graphics.get_width(), graphics.get_height());

	//Game state
	GameState gameState = MAIN_MENU;

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
			default:
				break;
		}
				
	}


	return 0;
}