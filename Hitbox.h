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
	SDL_Point tp, bp, lp, rp; //side points
	SDL_Point tl, tr, bl, br; //corner points
	SDL_Point itp, ibp, ilp, irp; //inner points
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
	SDL_Point& get_tp();
	SDL_Point& get_bp();
	SDL_Point& get_lp();
	SDL_Point& get_rp();
	SDL_Point& get_tl();
	SDL_Point& get_tr();
	SDL_Point& get_bl();
	SDL_Point& get_br();
	SDL_Point& get_itp();
	SDL_Point& get_ibp();
	SDL_Point& get_ilp();
	SDL_Point& get_irp();
	float get_width();
	float get_height();
	float get_x();
	float get_y();
	HitboxType get_type();
	SDL_Rect* get_rect();
};