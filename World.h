#pragma once

#include <stdbool.h>
#include <vector>
#include "GameActor.h"
#include "CollisionManager.h"
#include "InputHandler.h"
#include "Clock.h"

#define NUM_PLAYERS 1

class GameActor;
class CollisionManager;

class World {

	static const float respawn_delay;
	int screen_w, screen_h;
	Clock clock;
	CollisionManager* collision_manager;
	std::vector<GameActor*> actors;
	float player_respawn_timers[NUM_PLAYERS];

	void check_spawn_players();

public:

	World(int screen_w, int screen_h);
	void update(InputHandler* inputs);
	float get_delta();
	std::vector<GameActor*>* get_actors();
	~World();

};