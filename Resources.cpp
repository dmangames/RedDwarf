#include "Resources.h"
#include "Graphics.h"

#include <iostream>

// STATIC MEMBERS

const std::string Resources::RES_DIR = "res/";

// PRIVATE HELPER FUNCTIONS

bool Resources::load_font(TTF_Font** font, std::string filename, int size) {
	// Concatenate filename to resource directory
	std::string filepath = std::string(RES_DIR) + "fonts/" + filename;
	*font = TTF_OpenFont(filepath.c_str(), size);
	if (*font == NULL) {
		printf("Error loading font: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

bool Resources::load_texture(SDL_Texture** texture, std::string filename) {
	// Assumes texture is not already created
	*texture = NULL;

	// Imagefile -> Stuface -> Texture
	// Empty surface to begin with
	SDL_Surface* loaded_surface = NULL;

	// Concatenate filename to resource directory
	std::string filepath = RES_DIR + "images/" + filename;

	// Load imagefile into surface
	loaded_surface = IMG_Load(filepath.c_str());
	if (loaded_surface == NULL) {
		printf("Error loading image: %s\n", IMG_GetError());
		printf("Image path: %s\n", filepath.c_str());
		return false;
	}

	// Transfer the surface into the texture
	*texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if (texture == NULL) {
		printf("Error creating texture: %s\n", SDL_GetError());
	}

	// Mode BLEND allows for rendering with alpha channel changes even on PNGs without an alpha channel
	SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);

	// Free the temporary surface
	SDL_FreeSurface(loaded_surface);

	return true;

}

// PUBLIC FUNCTIONS

// Constructor takes in a renderer
Resources::Resources(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

// Load fonts and textures from the RES_DIR
void Resources::load_resources() {
	// FONTS
	fonts["inconsolata"] = NULL;
	load_font(&fonts["lazy"], "lazy.ttf", 18);

	// TEXTURES
	load_texture(&textures["main_menu"][1], "red_dwarf_main_menu_art.png");
	load_texture(&textures["dwarf"][1], "dwarf.png");
	load_texture(&textures["redrock"][1], "redrock_ss.png");
}

TTF_Font* Resources::get_font(std::string name) {
	return fonts[name];
}

SDL_Texture* Resources::get_texture(std::string name, int frame) {
	return textures[name][frame];
}

Resources::~Resources() {
	// Iterate through fonts and free them
	std::map<std::string, TTF_Font*>::iterator font_it;
	for (font_it = fonts.begin(); font_it != fonts.end(); ++font_it) {
		TTF_CloseFont(font_it->second);
	}

	// Iterate through the textures and free them
	std::map<std::string, std::map<int, SDL_Texture*>>::iterator texture_it;
	for (texture_it = textures.begin(); texture_it != textures.end(); ++texture_it) {
		std::map<int, SDL_Texture*>::iterator frame_it;
		for (frame_it = texture_it->second.begin(); frame_it != texture_it->second.end(); ++frame_it) {
			SDL_DestroyTexture(frame_it->second);
			frame_it->second = NULL;
		}
	}
}