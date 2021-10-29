#pragma once
#include <string>
#include <vector>

class MapGenerator {
	int width, height;
	std::string seed;
	int randomFillPercent;
	std::vector<std::vector<int>>* map;

	//HELPER FUNCTIONS
	void random_fill_map();
public:
	MapGenerator(int width, int height, int randomFillPercent);
	void generate_map();
	std::vector<std::vector<int>>* get_map();
	~MapGenerator();
};