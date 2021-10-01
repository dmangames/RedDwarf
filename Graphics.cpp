#include "Graphics.h"
#include "GameActor.h"

const int Graphics::SCREEN_HEIGHT = 720;
const int Graphics::SCREEN_WIDTH = 1280;

bool Graphics::init_sdl() {

	// Init SDL_video
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		printf("Error: SDL failed to init: %s\n", SDL_GetError());
		return false;
	}

	//Create SDL Window
	window = SDL_CreateWindow(
		"Red Dwarf",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL
	);
	if (window == NULL) {
		printf("Error! Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Init renderer with flags
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	return true;
}

