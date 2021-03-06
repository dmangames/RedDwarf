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
	load_texture(&textures["redrock"][2], "redrock_item.png");
	load_texture(&textures["purplerock"][1], "purplerock_ss.png");
	load_texture(&textures["purplerock"][2], "purplerock_item.png");
	load_texture(&textures["bluerock"][1], "bluerock_ss.png");
	load_texture(&textures["spawner"][1], "spawn_pit.png");
	load_texture(&textures["pinkling"][1], "pinkling_ss.png");
	

	// ANIMATION FRAMES
	load_animations();
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

SDL_Rect* Resources::get_anim_rect_32(int frame)
{
	return &anim_rects_32[frame];
}

void Resources::load_animations() {
	//Set sprite clips to be reused
	anim_rects_32[0].x = 0;
	anim_rects_32[0].y = 0;
	anim_rects_32[0].w = 32;
	anim_rects_32[0].h = 32;

	anim_rects_32[1].x = 32;
	anim_rects_32[1].y = 0;
	anim_rects_32[1].w = 32;
	anim_rects_32[1].h = 32;

	anim_rects_32[2].x = 64;
	anim_rects_32[2].y = 0;
	anim_rects_32[2].w = 32;
	anim_rects_32[2].h = 32;

	anim_rects_32[3].x = 96;
	anim_rects_32[3].y = 0;
	anim_rects_32[3].w = 32;
	anim_rects_32[3].h = 32;

	anim_rects_32[4].x = 128;
	anim_rects_32[4].y = 0;
	anim_rects_32[4].w = 32;
	anim_rects_32[4].h = 32;

	anim_rects_32[5].x = 160;
	anim_rects_32[5].y = 0;
	anim_rects_32[5].w = 32;
	anim_rects_32[5].h = 32;

	anim_rects_32[6].x = 192;
	anim_rects_32[6].y = 0;
	anim_rects_32[6].w = 32;
	anim_rects_32[6].h = 32;

	anim_rects_32[7].x = 224;
	anim_rects_32[7].y = 0;
	anim_rects_32[7].w = 32;
	anim_rects_32[7].h = 32;

	anim_rects_32[8].x = 256;
	anim_rects_32[8].y = 0;
	anim_rects_32[8].w = 32;
	anim_rects_32[8].h = 32;

	anim_rects_32[9].x = 288;
	anim_rects_32[9].y = 0;
	anim_rects_32[9].w = 32;
	anim_rects_32[9].h = 32; 

	anim_rects_32[10].x = 320;
	anim_rects_32[10].y = 0;
	anim_rects_32[10].w = 32;
	anim_rects_32[10].h = 32;

	anim_rects_32[11].x = 352;
	anim_rects_32[11].y = 0;
	anim_rects_32[11].w = 32;
	anim_rects_32[11].h = 32;

	anim_rects_32[12].x = 384;
	anim_rects_32[12].y = 0;
	anim_rects_32[12].w = 32;
	anim_rects_32[12].h = 32;

	anim_rects_32[13].x = 416;
	anim_rects_32[13].y = 0;
	anim_rects_32[13].w = 32;
	anim_rects_32[13].h = 32;

	anim_rects_32[14].x = 448;
	anim_rects_32[14].y = 0;
	anim_rects_32[14].w = 32;
	anim_rects_32[14].h = 32;

	anim_rects_32[15].x = 480;
	anim_rects_32[15].y = 0;
	anim_rects_32[15].w = 32;
	anim_rects_32[15].h = 32;

	anim_rects_32[16].x = 512;
	anim_rects_32[16].y = 0;
	anim_rects_32[16].w = 32;
	anim_rects_32[16].h = 32;

	anim_rects_32[17].x = 544;
	anim_rects_32[17].y = 0;
	anim_rects_32[17].w = 32;
	anim_rects_32[17].h = 32;

	anim_rects_32[18].x = 576;
	anim_rects_32[18].y = 0;
	anim_rects_32[18].w = 32;
	anim_rects_32[18].h = 32;

	anim_rects_32[19].x = 608;
	anim_rects_32[19].y = 0;
	anim_rects_32[19].w = 32;
	anim_rects_32[19].h = 32;

	anim_rects_32[20].x = 640;
	anim_rects_32[20].y = 0;
	anim_rects_32[20].w = 32;
	anim_rects_32[20].h = 32;

	anim_rects_32[21].x = 672;
	anim_rects_32[21].y = 0;
	anim_rects_32[21].w = 32;
	anim_rects_32[21].h = 32;

	anim_rects_32[22].x = 704;
	anim_rects_32[22].y = 0;
	anim_rects_32[22].w = 32;
	anim_rects_32[22].h = 32;

	anim_rects_32[23].x = 736;
	anim_rects_32[23].y = 0;
	anim_rects_32[23].w = 32;
	anim_rects_32[23].h = 32;


}