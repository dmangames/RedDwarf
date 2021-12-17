#pragma once

enum class TileType {
	EMPTY,
	REDROCK,
	PURPLEROCK,
	GOLDROCK,
	BLUEROCK,
	SPAWNER
};

struct Tile {
	int x;
	int y;
	int health;
	int max_health;
	TileType type;
};