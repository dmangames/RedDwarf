#include "Fist.h"
#include "Rock.h"

#define DEBUG false

Fist::Fist(float x, float y, int w, int h, int screen_w, int screen_h):
	Weapon(x, y, w, h, screen_w, screen_h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	this->hitbox = new Hitbox(0, 0, w, h);
}

void Fist::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
	//DEBUG Render hitbox
	if (DEBUG) {
		hitbox->render_corners(renderer, camera);
	}
}

void Fist::collide_actor(GameActor* actor)
{
	if (actor->get_id() == GameActorType::ROCK && active) {
		//Apply next layer of destruction
		printf("Applying Damage from Fist!\n");
		actor->take_damage(1);
		set_active(false);
		printf("Set active to %d\n", is_active());
	}
}