#pragma once
#include "GameActor.h"

enum class ItemType {
	DIRT,
	STONE,
	METAL
};

class Item :
    public GameActor
{
protected:
	bool alive = true;
public:
	Item();
	Item(float x, float y, int w, int h);
	void update(float delta);
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	bool is_alive();
	virtual const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType type);
	virtual void collide_actor(GameActor* actor);
	void collide_tile(Tile* tile);
	void resolve_collisions();
};

