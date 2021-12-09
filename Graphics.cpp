#include "Graphics.h"
#include "Player.h"

const int Graphics::SCREEN_HEIGHT = 720;
const int Graphics::SCREEN_WIDTH = 1280;

bool Graphics::init_sdl() {

	// Init SDL_video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
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

	if (renderer == NULL) {
		printf("Could not init renderer: %s\n", SDL_GetError());
		return false;
	}

	// Initialize TTF
	if (TTF_Init() == -1) {
		printf("SDL_ttf failed to initialize: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

// PUBLIC FUNCTIONS

Graphics::Graphics() {
	init_sdl();
	resources = new Resources(renderer);
	resources->load_resources();
	font_renderer = new FontRenderer(renderer, resources);
	overlay = new Overlay(SCREEN_WIDTH, SCREEN_HEIGHT, renderer, resources);
	fps_counter = FPSCounter();
}

void Graphics::clear_screen() {
	// Clear the screen with a white background
	SDL_SetRenderDrawColor(renderer, 71, 69, 67, 0); // DARK BEIGE
	SDL_RenderClear(renderer);
}

void Graphics::render_overlay() {
	overlay->render_fps(font_renderer, lround(fps_counter.get_fps()));
}

void Graphics::render_mainmenu() {
	SDL_Texture* texture;
	texture = resources->get_texture("main_menu", 1);
	SDL_Rect dst;
	dst.x = 0;
	dst.y = 0;
	dst.w = SCREEN_WIDTH;
	dst.h = SCREEN_HEIGHT;
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void Graphics::present_renderer() {
	SDL_RenderPresent(renderer);
}

void Graphics::present_renderer(float delta) {
	SDL_RenderPresent(renderer);
	fps_counter.count(delta);
}

void Graphics::render_tiles(MapGenerator* map, float delta, Camera* camera)
{
	map->render(renderer, resources, delta, camera);
}

void Graphics::render_actors(std::vector<GameActor*>* actors, float delta, Camera* camera) {
	for (int i = 0; i < actors->size(); ++i) {
		(*actors)[i]->render(renderer, resources, delta, camera);
	}
}

void Graphics::render_players(std::vector<GameActor*>* players, float delta, Camera* camera) {
	for (int i = 0; i < players->size(); ++i) {
		dynamic_cast<Player*>((*players)[i])->render(renderer, resources, delta, camera);
		dynamic_cast<Player*>((*players)[i])->render_inventory(font_renderer, renderer, resources, camera);
	}
}



int Graphics::get_width() {
	return SCREEN_WIDTH;
}

int Graphics::get_height() {
	return SCREEN_HEIGHT;
}

Graphics::~Graphics() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	delete resources;
	delete font_renderer;
	delete overlay;
}