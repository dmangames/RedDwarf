/*Game Actor*/
#include "GameActor.h"
#include "Hitbox.h"

// PRIVATE HELPER METHODS

float GameActor::get_center_x() {
	return x + w / 2;
}

float GameActor::get_center_y() {
	return y + h / 2;
}

void GameActor::check_bounds() {
	// check bounds can be used for now to make the player not leave the screen
	if (this->get_center_x() < 0) {
		x = 0 + w / 2;
	}
	else if (this->get_center_x() > screen_w) {
		x = screen_w - w / 2;
	}
	if (this->get_center_y() < 0) {
		y = 0 + h / 2;
	}
	else if (this->get_center_y() > screen_h) {
		y = screen_h - h / 2;
	}
}

// PUBLIC METHODS

GameActor::GameActor() {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	screen_w = 0;
	screen_h = 0;
	hitbox = NULL;
	active = true;
	alive = true;
}

GameActor::GameActor(float x, float y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->screen_w = 0;
	this->screen_h = 0;
}

GameActor::GameActor(float x, float y, int w, int h, int screen_w, int screen_h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	hitbox = NULL;
	active = true;
	alive = true;
}

Hitbox* GameActor::get_hitbox() {
	return hitbox;
}

float GameActor::get_x() {
	return x;
}

float GameActor::get_y() {
	return y;
}

void GameActor::set_x(float x) {
	this->x = x;
}

void GameActor::set_y(float y) {
	this->y = y;
}

void GameActor::set_active(bool isActive) {
	this->active = isActive;
}

bool GameActor::is_active() {
	return active;
}

void GameActor::set_alive(bool aliveStatus)
{
	alive = aliveStatus;
}

GameActor::~GameActor() {
	if (hitbox != NULL) {
		delete hitbox;
	}
}

void GameActor::render_text(FontRenderer* font_renderer, SDL_Renderer* renderer, Resources* resources, Camera* camera, std::string text)
{
	// Texture to hold the drawn text
	SDL_Texture* text_texture = NULL;
	int text_width;
	int text_height;
	SDL_Color color = { 255, 255, 255, 255 };

	font_renderer->load_font_texture(&text_texture, "lazy", text, color);
	SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);

	SDL_Rect dst = {
		x,
		y,
		text_width,
		text_height
	};

	SDL_RenderCopy(renderer, text_texture, NULL, &dst);

	SDL_DestroyTexture(text_texture);
}

void GameActor::take_damage(int damage)
{
}
