#pragma once
#include "GameActor.h"

class DestructibleObject:public GameActor {
	// References

	// General Variables
protected:
	bool alive;
	int damage_taken;
	int health;

public:
	DestructibleObject(float x, float y, int w, int h, int screen_w, int screen_h);
	void update(float delta);
	virtual void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) = 0;
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType id);
	void collide_actor(GameActor* actor);
	virtual void take_damage(int damage)=0;
	int get_damage_taken();
	int get_health();
	virtual void on_destroy() = 0;
};