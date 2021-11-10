#pragma once

#include <string>
#include "GameActor.h"
#include "Hitbox.h"


class Enemy : public GameActor {
	// References

	// General Variables
	float vx, vy;
	float linear_accel, friction_coeff;
	float max_speed;
	float angle;
	float delta;
	bool alive;
	bool isColliding;




	//Animation Variables
	const uint16_t IDLE_ANIMATION = 2;
	const uint16_t IDLE_ANIMATION_START = 0;
	const uint16_t IDLE_UPDATE_RATE = 16;
	const uint16_t WALK_ANIMATION = 8;
	const uint16_t WALK_ANIMATION_START = 2;
	const uint16_t WALK_UPDATE_RATE = 4;
	const uint16_t PUNCH_ANIMATION = 7;
	const uint16_t PUNCH_ANIMATION_START = 10;
	const uint16_t PUNCH_UPDATE_RATE = 4;
	const uint16_t TOTAL_ANIMATION = 17;
	SDL_Rect anim_rects[17];
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int frame;
	int startFrame;
	int updatesPerFrame;



	// Helper Functions


public:
	Enemy(float x, float y, int w, int h, int screen_w, int screen_h);
	void update(float delta);
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType id);
	void collide_actor(GameActor* actor);
	void collide_tile(Tile* tile);
	void resolve_collisions();
	void load_animations();
	void take_damage(int damage);
};