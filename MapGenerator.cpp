#include "MapGenerator.h"
#include <ctime>
#include <vector>

//PRIVATE HELPER FUNCTIONS

void MapGenerator::random_fill_map()
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//Make a solid border around the map
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				(*map)[x][y] = 1;
			}
			else {
				(*map)[x][y] = (rand() % 100 < randomFillPercent) ? 1 : 0;
			}
		}
	}
}

//PUBLIC FUNCTIONS

//randomFillPercent range should be 1-100
MapGenerator::MapGenerator(int width, int height, int randomFillPercent) {
	this->width = width;
	this->height = height;
	srand(time(NULL));
	this->randomFillPercent = randomFillPercent;
	map = new std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
}

void MapGenerator::generate_map() {
	random_fill_map();
}

SDL_Point* MapGenerator::get_random_empty_cell()
{
	SDL_Point* empty_cell = nullptr;
	srand(time(NULL));
	while (empty_cell == nullptr) {
		int rx = rand() % width;
		int ry = rand() % height;
		if ((*map)[rx][ry] == 0) {
			empty_cell = new SDL_Point();
			empty_cell->x = rx;
			empty_cell->y = ry;
		}
	}
	return empty_cell;
}

std::vector<std::vector<int>>* MapGenerator::get_map()
{
	return map;
}

MapGenerator::~MapGenerator() {
	delete map;
}

