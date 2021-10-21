#pragma once
#include "GameActor.h"

class DestructibleObject:public GameActor {
	// References
	std::vector<GameActor*>* actors;

	// General Variables
	bool alive;


public:
	DestructibleObject(float x, float y, int w, int h, int screen_w, int screen_h);
	void update(float delta);
	virtual void render(SDL_Renderer* renderer, Resources* resources, float delta) = 0;
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType type);
	void collide_actor(GameActor* actor);
};