#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "Resources.h"
#include "Camera.h"

enum class TileType {
	DIRT,
	ROCK,
	METAL,
};

struct Tile {
	int x;
	int y;
	SDL_Rect hitbox;
	TileType type;
};

class MapGenerator {
	int width, height;
	std::string seed;
	int randomFillPercent;
	std::vector<std::vector<int>>* map;
	//std::map<std::tuple<int, int>, Tile>* tilemap;

	//HELPER FUNCTIONS
	void random_fill_map();
public:
	MapGenerator(int width, int height, int randomFillPercent);
	void generate_map();
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	std::vector<std::vector<int>>* get_map();
	SDL_Point* get_random_empty_cell();
	~MapGenerator();
};