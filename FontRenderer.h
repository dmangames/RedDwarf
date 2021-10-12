#pragma once
#include "Graphics.h"

class FontRenderer {

	Resources* resources;
	SDL_Renderer* renderer;

public:

	FontRenderer(SDL_Renderer* renderer, Resources* resources);
	bool load_font_texture(SDL_Texture** texture, std::string font,
		std::string text, SDL_Color text_color);

};
