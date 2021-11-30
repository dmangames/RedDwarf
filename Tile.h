#pragma once

enum class TileType {
	EMPTY,
	DIRT,
	ROCK,
	METAL,
	SPAWNER
};

struct Tile {
	int x;
	int y;
	int health;
	int max_health;
	TileType type;
};