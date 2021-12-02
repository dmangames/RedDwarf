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
#include "FontRenderer.h"
#include "FPSCounter.h"
#include "Overlay.h"
#include "Camera.h"
#include "MapGenerator.h"

class Overlay;
class FontRenderer;
class GameActor;
class Player;

class Graphics {
	// width and height of the window in px
	static const int SCREEN_WIDTH, SCREEN_HEIGHT;

	Resources* resources;
	FPSCounter fps_counter;
	FontRenderer* font_renderer;
	Overlay* overlay;
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool init_sdl();
	
public:
	Graphics();
	void clear_screen();
	void render_overlay();
	void render_mainmenu(); // shows the main menu image
	void present_renderer(); // simple render without time delta for fps
	void present_renderer(float delta);
	void render_tiles(MapGenerator* map, float delta, Camera* camera); // render the tiles that can be seen on camera
	void render_actors(std::vector<GameActor*>* actors, float delta, Camera* camera); //render with camera offset
	void render_players(std::vector<Player*>* players, float delta, Camera* camera);
	int get_width();
	int get_height();
	~Graphics();

};

#endif