#include "MapGenerator.h"
#include <ctime>
#include <cstdlib>

//PRIVATE HELPER FUNCTIONS

void MapGenerator::random_fill_map()
{
	std::srand(std::time(0));
	printf("Random number: %d\n",std::rand());
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//Make a solid border around the map
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				(*map)[x][y] = 1;
			}
			else {
				(*map)[x][y] = (std::rand() % 100 < randomFillPercent) ? 1 : 0;
			}
		}
	}
}

//PUBLIC FUNCTIONS

//randomFillPercent range should be 1-100
MapGenerator::MapGenerator(int width, int height, int randomFillPercent) {
	this->width = width;
	this->height = height;
	this->randomFillPercent = randomFillPercent;
	map = new std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
}

void MapGenerator::generate_map() {
	random_fill_map();
}

void MapGenerator::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
	//Loop through only the tiles in the camera view for rendering
	int x_start = std::max(camera->get_x_offset() / 32, 0);
	int x_finish = std::min(x_start + camera->get_width() / 32, width);
	int y_start = std::max(camera->get_y_offset() / 32, 0);
	int y_finish = std::min(y_start + camera->get_height() / 32, height);
	for (int x = x_start; x < x_finish; x++) {
		for (int y = y_start; y < y_finish; y++) {
			if ((*map)[x][y] == 1) {
				SDL_Texture* texture;
				texture = resources->get_texture("redrock", 1);
				SDL_Rect dst;
				dst.x = x*32 - camera->get_x_offset();
				dst.y = y*32 - camera->get_y_offset();
				dst.w = 32;
				dst.h = 32;
				SDL_Rect src;
				src.x = 0;
				src.y = 0;
				src.w = 32;
				src.h = 32;
				SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_NONE);
			}
			else {

			}

		}
	}
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

