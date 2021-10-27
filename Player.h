#pragma once

#include <string>
#include "GameActor.h"
#include "Hitbox.h"
#include "InputHandler.h"
#include "Fist.h"

enum class AnimState {
	IDLE,
	WALK,
	PUNCH
};

class Player : public GameActor {
	
	// References
	std::vector<GameActor*>* actors;
	Camera* camera;

	// General Variables
	float vx, vy;
	float linear_accel, friction_coeff;
	float max_speed;
	float angle;
	bool alive;
	int player_num;
	bool isColliding;
	float px, py;
	Fist* fist;
	

	
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
	AnimState state = AnimState::IDLE;


	// Helper Functions

	void setAnimState(AnimState state);

public:
	Player(float x, float y, int player_num, int screen_w, int screen_h, std::vector<GameActor*>* actors, Camera* camera);
	void update(float delta);
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	void handle_inputs(float delta, InputHandler* inputs);
	bool is_alive();
	const GameActorType get_id();
	const bool collides();
	bool does_collide(GameActorType id);
	void collide_actor(GameActor* actor);
	int get_player_num();
	void load_animations();
};