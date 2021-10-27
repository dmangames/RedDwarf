#include "DestructibleObject.h"

DestructibleObject::DestructibleObject(float x, float y, int w, int h, int screen_w, int screen_h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	alive = true;
}

void DestructibleObject::update(float delta)
{
	hitbox->update_pos(x, y);
}


const GameActorType DestructibleObject::get_id()
{
	return GameActorType::DESTRUCTABLE;
}

const bool DestructibleObject::collides()
{
	return true;
}

bool DestructibleObject::does_collide(GameActorType id)
{
	return id == GameActorType::PLAYER || id == GameActorType::ENEMY;
}


void DestructibleObject::collide_actor(GameActor* actor)
{
}

bool DestructibleObject::is_alive() {
	return alive;
}

int DestructibleObject::get_damage_taken() {
	return damage_taken;
}

int DestructibleObject::get_health() {
	return health;
}
