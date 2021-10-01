#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Resources.h"
#include "GameActor.h"

class Graphics {
	// width and height of the window in px
	static const int SCREEN_WIDTH, SCREEN_HEIGHT;

	Resources* resources;
	//FPSCounter fps_counter;
	//FontRenderer* font_renderer;
	//Overlay* overlay;
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool init_sdl();
	
public:
	Graphics();
	void clear_screen();
	void render_overlay(std::vector<GameActor*>* actors);
	void present_renderer(float delta);
	void render_actors(std::vector<GameActor*>* actors, float delta);
	int get_width();
	int get_height();
	~Graphics();

};

#endif