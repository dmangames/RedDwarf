#include "Weapon.h"

Weapon::Weapon(float x, float y, int w, int h, int screen_w, int screen_h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	alive = true;
	active = false;
}

void Weapon::update(float delta)
{
	hitbox->update_pos(x, y);
}


const GameActorType Weapon::get_id()
{
	return GameActorType::WEAPON;
}

const bool Weapon::collides()
{
	return active;
}

bool Weapon::does_collide(GameActorType id)
{
	return id == GameActorType::DESTRUCTABLE || id == GameActorType::ENEMY;
}

bool Weapon::is_alive() {
	return alive;
}

void Weapon::set_active(bool isActive) {
	active = isActive;
}