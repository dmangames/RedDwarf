#include "Hitbox.h"

//Size of rects drawn on corners by render_conter() should be a n even iteger
#define POINT_SIZE 4
// Color of rects draw n on corners by render_corners()
#define POINT_R 255
#define POINT_G 255
#define POINT_B 255
#define POINT_A 255

// HELPER FUNCTIONS

// Rotate point around origin by a given angle
void rotate_point(int* x, int* y, float angle) {
	int old_x = *x;
	int old_y = *y;
	*x = old_x * cos(angle) - old_y * sin(angle);
	*y = old_x * sin(angle) + old_y * cos(angle);
}

// PUBLIC FUNCTIONS

Hitbox::Hitbox(float x_offset, float y_offset, int w, int h, HitboxType type) {

	this->x_offset = x_offset;
	this->y_offset = y_offset;
	this->w = w;
	this->h = h;
	radius = pow(w, 2) + pow(h, 2);
	circle_radius = pow(w / 2, 2) + pow(h / 2, 2);
	this->type = type;
	this->rect = new SDL_Rect();
}

// Given new coordinates and angle, update hitbox corners
// TODO: Account for offset center
void Hitbox::update_pos(float x, float y, float angle) {

	// Apply offsets
	this->x = x + x_offset;
	this->y = y + y_offset;

	// Set angle
	this->angle = angle;

	// Calculate center point
	float cx = this->x + w / 2.0f;
	float cy = this->y + h / 2.0f;

	// Side points
	tp.x = cx;
	tp.y = this->y;
	bp.x = cx;
	bp.y = this->y + h;
	lp.x = this->x;
	lp.y = cy;
	rp.x = this->x + w;
	rp.y = cy;

	// Corner points
	tl.x = this->x;
	tl.y = this->y;
	bl.x = this->x;
	bl.y = this->y + h;
	tr.x = this->x + w;
	tr.y = this->y;
	br.x = this->x + w;
	br.y = this->y + h;

	// Inner points
	itp.x = tp.x;
	itp.y = tp.y + 2;
	ibp.x = bp.x;
	ibp.y = bp.y - 2;
	ilp.x = lp.x + 2;
	ilp.y = lp.y;
	irp.x = rp.x - 2;
	irp.y = rp.y;

	//// Top Left Corner
	//tl.x = -w / 2;
	//tl.y = -h / 2;
	//tl.x += cx;
	//tl.y += cy;

	//// Top Right Corner
	//tr.x = w / 2;
	//tr.y = -h / 2;
	//tr.x += cx;
	//tr.y += cy;

	//// Bottom Left Corner
	//bl.x = -w / 2;
	//bl.y = h / 2;
	//bl.x += cx;
	//bl.y += cy;

	//// Bottom Right Corner
	//br.x = w / 2;
	//br.y = h / 2;
	//br.x += cx;
	//br.y += cy;

}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

// Debug method to draw corners
void Hitbox::render_corners(SDL_Renderer* renderer, Camera* camera) {
	// Set draw color
	SDL_SetRenderDrawColor(renderer, POINT_R, POINT_G, POINT_B, POINT_A);
	if (this->type == HitboxType::RECT) {

		// Create rectangles over each corner
		SDL_Rect tl_rect = { tl.x - camera->get_x_offset() - POINT_SIZE / 2, tl.y - camera->get_y_offset() - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE };
		SDL_Rect tr_rect = { tr.x - camera->get_x_offset() - POINT_SIZE / 2, tr.y - camera->get_y_offset() - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE };
		SDL_Rect bl_rect = { bl.x - camera->get_x_offset() - POINT_SIZE / 2, bl.y - camera->get_y_offset() - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE };
		SDL_Rect br_rect = { br.x - camera->get_x_offset() - POINT_SIZE / 2, br.y - camera->get_y_offset() - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE };

		// Draw rectangles
		SDL_RenderFillRect(renderer, &tl_rect);
		SDL_RenderFillRect(renderer, &tr_rect);
		SDL_RenderFillRect(renderer, &bl_rect);
		SDL_RenderFillRect(renderer, &br_rect);
	}
	else if (this->type == HitboxType::CIRCLE) {
		DrawCircle(renderer, get_center_x() - camera->get_x_offset(), get_center_y() - camera->get_y_offset(), sqrt(circle_radius));
	}

}

int Hitbox::get_center_x() {
	return x + w / 2.0f;
}

int Hitbox::get_center_y() {
	return y + h / 2.0f;
}

float Hitbox::get_radius() {
	return radius;
}

float Hitbox::get_circle_radius() {
	return circle_radius;
}

//Get top left point
SDL_Point& Hitbox::get_tl() {
	return tl;
}

//Get top right point
SDL_Point& Hitbox::get_tr() {
	return tr;
}

//Get bottom left point
SDL_Point& Hitbox::get_bl() {
	return bl;
}

//Get bottom right point
SDL_Point& Hitbox::get_br() {
	return br;
}

//Get top point
SDL_Point& Hitbox::get_tp() {
	return tp;
}

//Get bottom point
SDL_Point& Hitbox::get_bp() {
	return bp;
}

//Get left point
SDL_Point& Hitbox::get_lp() {
	return lp;
}

//Get right point
SDL_Point& Hitbox::get_rp() {
	return rp;
}

//Get inner top point
SDL_Point& Hitbox::get_itp() {
	return itp;
}

//Get inner bottom point
SDL_Point& Hitbox::get_ibp() {
	return ibp;
}

//Get inner left point
SDL_Point& Hitbox::get_ilp() {
	return ilp;
}

//Get inner right point
SDL_Point& Hitbox::get_irp() {
	return irp;
}



float Hitbox::get_width() {
	return w;
}

float Hitbox::get_height() {
	return h;
}

float Hitbox::get_x() {
	return x;
}

float Hitbox::get_y() {
	return y;
}

HitboxType Hitbox::get_type() {
	return type;
}

SDL_Rect* Hitbox::get_rect()
{
	rect->x = this->x;
	rect->y = this->y;
	rect->w = this->w;
	rect->h = this->h;
	return rect;
}