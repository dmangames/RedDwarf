#pragma once

#include <string>
#include "GameActor.h"
#include "Hitbox.h"
#include "Player.h"

class Enemy : public GameActor {
	// References
	Player* player;

	// General Variables
	float vx, vy;
	float linear_accel, friction_coeff;
	float max_speed;
	float angle;
	float delta;
	bool alive;
	bool isColliding;
	float px, py;
	float bite_cooldown, bite_timer;



	//Animation Variables
	const uint16_t IDLE_ANIMATION = 7;
	const uint16_t IDLE_ANIMATION_START = 9;
	const uint16_t IDLE_UPDATE_RATE = 8;
	const uint16_t WALK_ANIMATION = 8;
	const uint16_t WALK_ANIMATION_START = 1;
	const uint16_t WALK_UPDATE_RATE = 4;
	const uint16_t BITE_ANIMATION = 7;
	const uint16_t BITE_ANIMATION_START = 16;
	const uint16_t BITE_UPDATE_RATE = 4;
	const uint16_t TOTAL_ANIMATION = 23;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int frame;
	int startFrame;
	int updatesPerFrame;
	AnimState state = AnimState::IDLE;


	// Helper Functions
	void setAnimState(AnimState s);

public:
	Enemy(float x, float y, int w, int h, int screen_w, int screen_h, Player* player);
	void update(float delta);
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType id);
	void collide_actor(GameActor* actor);
	void collide_tile(Tile* tile);
	void resolve_collisions();
	void take_damage(int damage);
};