#pragma once

#include <vector>
#include <string>
#include <map>
#include "Resources.h"
#include "Hitbox.h"
#include "Camera.h"
#include "Tile.h"

enum class GameActorType {
	PLAYER,
	ENEMY,
	DESTRUCTABLE,
	WEAPON,
	ROCK,
	ITEM
};


class GameActor {
protected:
	float x, y;
	int w, h;
	int screen_w, screen_h;
	Hitbox* hitbox;
	bool active; // Used to toggle if world should take this actor into account
	float get_center_x();
	float get_center_y();
	void check_bounds();
public:
	GameActor();
	GameActor(float x, float y, int w, int h);
	GameActor(float x, float y, int w, int h, int screen_w, int screen_h);
	Hitbox* get_hitbox();
	float get_x();
	float get_y();
	void set_x(float x);
	void set_y(float y);
	void set_active(bool isActive);
	bool is_active();
	virtual ~GameActor() = 0;
	virtual void update(float delta) = 0;
	virtual void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) = 0;
	virtual bool is_alive() = 0;
	virtual const GameActorType get_id() = 0;
	virtual const bool collides() = 0;
	virtual bool does_collide(GameActorType type) = 0;
	virtual void collide_actor(GameActor* actor) = 0;
	virtual void collide_tile(Tile* tile) = 0;
	virtual void resolve_collisions() = 0;
	virtual void take_damage(int damage);


};