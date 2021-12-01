#pragma once

#include "Graphics.h"

class FontRenderer;

class Overlay {

	int width, height;
	SDL_Renderer* renderer;
	Resources* resources;

public:

	Overlay(int width, int height, SDL_Renderer* renderer, Resources* resources);
	void render_fps(FontRenderer* font_renderer, int fps);
	void render_text(FontRenderer* font_renderer, std::string text, int x, int y);
};