#include "Rock.h"

#define DEBUG true

#define WIDTH 32
#define HEIGHT 32

Rock::Rock(float x, float y, int screen_w, int screen_h)
	: DestructibleObject(x, y, WIDTH, HEIGHT, screen_w, screen_h)
{
	this->x = x;
	this->y = y;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	this->hitbox = new Hitbox(0, 0, w, h);
	this->health = 7;
	load_animations();
}

const GameActorType Rock::get_id()
{
	return GameActorType::ROCK;
}

void Rock::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
	SDL_Texture* texture;
	texture = resources->get_texture("redrock", 1);
	SDL_Rect dst;
	dst.x = this->x - camera->get_x_offset();
	dst.y = this->y - camera->get_y_offset();
	dst.w = this->w;
	dst.h = this->h;
	SDL_RenderCopyEx(renderer, texture, &anim_rects[damage_taken], &dst, 0, NULL, SDL_FLIP_NONE);

	//DEBUG Render hitbox
	if (DEBUG) {
		hitbox->render_corners(renderer, camera);
	}
}

void Rock::take_damage(int damage) {
	damage_taken += damage;
	printf("Damage taken: %d,\n", damage_taken);
	if (damage_taken > health) {
		on_destroy();
	}
}

void Rock::on_destroy()
{
	alive = false;
}

void Rock::load_animations() {
	//Set sprite clips
	// Destruction
	anim_rects[0].x = 0;
	anim_rects[0].y = 0;
	anim_rects[0].w = 32;
	anim_rects[0].h = 32;

	anim_rects[1].x = 32;
	anim_rects[1].y = 0;
	anim_rects[1].w = 32;
	anim_rects[1].h = 32;

	anim_rects[2].x = 64;
	anim_rects[2].y = 0;
	anim_rects[2].w = 32;
	anim_rects[2].h = 32;

	anim_rects[3].x = 96;
	anim_rects[3].y = 0;
	anim_rects[3].w = 32;
	anim_rects[3].h = 32;

	anim_rects[4].x = 128;
	anim_rects[4].y = 0;
	anim_rects[4].w = 32;
	anim_rects[4].h = 32;

	anim_rects[5].x = 160;
	anim_rects[5].y = 0;
	anim_rects[5].w = 32;
	anim_rects[5].h = 32;

	anim_rects[6].x = 192;
	anim_rects[6].y = 0;
	anim_rects[6].w = 32;
	anim_rects[6].h = 32;

	anim_rects[7].x = 224;
	anim_rects[7].y = 0;
	anim_rects[7].w = 32;
	anim_rects[7].h = 32;


}