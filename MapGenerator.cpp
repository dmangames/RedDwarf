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
			//Set tile coordinates (top left origin)
			(*map)[x][y].x = x;
			(*map)[x][y].y = y;
			//Make a solid border around the map
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				(*map)[x][y].type = TileType::METAL;
				(*map)[x][y].health = 16;
				(*map)[x][y].max_health = 16;
			}
			else {
				if ((std::rand() % 100 < randomFillPercent)) {
					(*map)[x][y].type = TileType::DIRT;
					(*map)[x][y].health = 8;
					(*map)[x][y].max_health = 8;
				}
				else {
					(*map)[x][y].type = TileType::EMPTY;
				}
				
			}
		}
	}
	// Pick random areas of the map and hollow them out
	for (int i = 0; i < 20; i++) {
		SDL_Point* p = get_random_empty_cell();
		// Get the cell's neighbors
		for (auto t : *get_neighboring_cells(p->x, p->y, rand()%15)) {
			if (t->type != TileType::METAL) {
				t->type = TileType::EMPTY;
				t->health = 0;
				t->max_health = 0;
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
	map = new std::vector<std::vector<Tile>>(width, std::vector<Tile>(height, { 0, 0, 0, 8, TileType::DIRT }));
}

void MapGenerator::generate_map() {
	random_fill_map();
}

void MapGenerator::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
	//Loop through only the tiles in the camera view for rendering
	int x_start = std::max(camera->get_x_offset() / 32, 0);
	int x_finish = std::min(x_start + camera->get_width() / 32 + 2, width);
	int y_start = std::max(camera->get_y_offset() / 32, 0);
	int y_finish = std::min(y_start + camera->get_height() / 32 + 2, height);
	for (int x = x_start; x < x_finish; x++) {
		for (int y = y_start; y < y_finish; y++) {
			Tile tile = (*map)[x][y];
			if (tile.type == TileType::EMPTY)
				continue;
			SDL_Texture* texture;
			texture = resources->get_texture("redrock", 1);
			SDL_Rect dst;
			dst.x = x * 32 - camera->get_x_offset();
			dst.y = y * 32 - camera->get_y_offset();
			dst.w = 32;
			dst.h = 32;
			SDL_Rect src;
			src.x = 0;
			src.y = 0;
			src.w = 32;
			src.h = 32;
			switch (tile.type) {
			case TileType::DIRT:
					texture = resources->get_texture("redrock", 1);
					SDL_RenderCopyEx(renderer, texture, resources->get_anim_rect_32(tile.max_health - tile.health), &dst, 0, NULL, SDL_FLIP_NONE);

					break;
			case TileType::METAL:
					texture = resources->get_texture("purplerock", 1);
					SDL_RenderCopyEx(renderer, texture, resources->get_anim_rect_32((tile.max_health - tile.health)/2), &dst, 0, NULL, SDL_FLIP_NONE);
					break;
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
		if ((*map)[rx][ry].type == TileType::EMPTY) {
			empty_cell = new SDL_Point();
			empty_cell->x = rx;
			empty_cell->y = ry;
		}
	}
	return empty_cell;
}

std::vector<Tile*>* MapGenerator::get_neighboring_cells(int x, int y, int range) 
{
	std::vector<Tile*>* neighbors = new std::vector<Tile*>();
	for (int i = std::max(x - range, 0); i <= std::min(x + range, width-1); i++) {
		for (int j = std::max(y - range, 0); j <= std::min(y + range, height-1); j++) {
			//if (x != i || y != j) {
				neighbors->push_back(&(*map)[i][j]);
			//}
		}
	}
	return neighbors;
}

std::vector<std::vector<Tile>>* MapGenerator::get_map()
{
	return map;
}

int MapGenerator::get_height() {
	return height;
}

int MapGenerator::get_width() {
	return width;
}

MapGenerator::~MapGenerator() {
	delete map;
}

