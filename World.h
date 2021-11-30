#pragma once

#include <stdbool.h>
#include <vector>
#include "GameActor.h"
#include "CollisionManager.h"
#include "InputHandler.h"
#include "Clock.h"
#include "Camera.h"
#include "MapGenerator.h"
#include "Player.h"
#include "AStar.hpp"
#include "Item.h"
#include "TileItem.h"

#define NUM_PLAYERS 1

class GameActor;
class CollisionManager;

class World {

	static const float RESPAWN_DELAY, ENEMY_SPAWN_DELAY, ENEMY_PATHFIND_DELAY;
	int seed;
	int screen_w, screen_h;
	Clock clock;
	CollisionManager* collision_manager;
	Camera* camera;
	MapGenerator* map_generator;
	AStar::Generator path_generator;
	std::vector<GameActor*> actors;
	std::vector<Player*> players;
	float player_respawn_timers[NUM_PLAYERS];
	float enemy_spawn_timer;

	void check_spawn_players();
	void spawn_enemies();
	void spawn_tile_item(Tile* t);
	void find_path();

public:

	World(int screen_w, int screen_h);
	void update(InputHandler* inputs);
	float get_delta();
	std::vector<GameActor*>* get_actors();
	Camera* get_camera();
	bool generate_map(); //starting x and y position is 0, 0, map width and height determined by MapGenerator
	MapGenerator* get_map();
	~World();

};