#include "Item.h"

Item::Item():GameActor()
{
}

Item::Item(float x, float y, int w, int h):GameActor(x, y, w, h)
{
}

void Item::update(float delta)
{
}

void Item::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
}

bool Item::is_alive()
{
	return true;
}

const GameActorType Item::get_id()
{
	return GameActorType::ITEM;
}

const bool Item::collides()
{
	return false;
}

bool Item::does_collide(GameActorType type)
{
	return false;
}

void Item::collide_actor(GameActor* actor)
{
}

void Item::collide_tile(Tile* tile)
{
}

void Item::resolve_collisions()
{
}
