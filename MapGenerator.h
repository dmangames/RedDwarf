#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "Resources.h"
#include "Camera.h"
#include "Tile.h"

class MapGenerator {
	int width, height;
	int seed;
	int randomFillPercent;
	std::vector<std::vector<Tile>>* map;
	std::map<std::tuple<int, int>, Tile>* spawn_loc_map;

	//HELPER FUNCTIONS
	void random_fill_map();
	void create_enemy_spawner();
public:
	MapGenerator(int width, int height, int randomFillPercent);
	void generate_map();
	void add_enemy_spawners(int num);
	std::tuple<int, int> get_random_spawn_loc();
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	std::vector<std::vector<Tile>>* get_map();
	SDL_Point* get_random_empty_cell();
	std::vector<Tile*>* get_neighboring_cells(int x, int y, int range);
	std::vector<Tile*>* get_destroyed_tiles_onscreen(Camera* camera);
	int get_width();
	int get_height();
	~MapGenerator();
};