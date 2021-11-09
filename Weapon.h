#pragma once
#include "GameActor.h"

class Weapon :public GameActor {
	// References


	// General Variables
	bool alive;


public:
	Weapon(float x, float y, int w, int h, int screen_w, int screen_h);
	void update(float delta);
	virtual void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) = 0;
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType type);
	virtual void collide_actor(GameActor* actor) = 0;
	virtual void resolve_collisions() = 0;
	void take_damage(int damage);
};