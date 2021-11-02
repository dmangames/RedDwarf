#pragma once

#include <SDL.h>
#include "Camera.h"

// Offsets can be used to place the hitbox more accurately

enum class HitboxType {
	RECT,
	CIRCLE
};

class Hitbox {
	float x, y;
	int x_offset, y_offset, w, h;
	float angle;
	float radius, circle_radius;
	SDL_Point tl, tr, bl, br;
	HitboxType type;
	SDL_Rect* rect;

public:
	Hitbox(float x_offset, float y_offset, int w, int h, HitboxType type=HitboxType::RECT);
	void update_pos(float x, float y, float angle = 0.0f);
	void render_corners(SDL_Renderer* renderer, Camera* camera); //DEBUG METHOD
	int get_center_x();
	int get_center_y();
	float get_radius();
	float get_circle_radius();
	SDL_Point get_tl();
	SDL_Point get_tr();
	SDL_Point get_bl();
	SDL_Point get_br();
	float get_width();
	float get_height();
	float get_x();
	float get_y();
	HitboxType get_type();
	SDL_Rect* get_rect();
};