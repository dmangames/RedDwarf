#pragma once
#include "Weapon.h"
class EnemyWeapon : public Weapon
{
	int damage;
public: 
	EnemyWeapon(float x, float y, int w, int h, int damage);
    const GameActorType get_id() override;
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) override;
	void collide_actor(GameActor* actor) override;
	void collide_tile(Tile* tile) override;
	void resolve_collisions() override;
	int get_damage();
};

