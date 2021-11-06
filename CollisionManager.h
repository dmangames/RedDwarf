#pragma once

#include <vector>
#include "GameActor.h"


class CollisionManager {

	std::vector<GameActor*>* actors;

	//HELPER FUNCTIONS

	/*bool circleRect(Hitbox* circle, Hitbox* rect);
	bool intersect_rect_tile(int tile_left, int tile_right, int tile_bottom, int tile_top, Hitbox* h1);
	bool intersect_circle_tile(Hitbox* circle, int tile_x, int tile_y, int tile_w, int tile_h);*/
public:
	CollisionManager(std::vector<GameActor*>* actors);
	void check_collisions();
	void check_tile_collisions(MapGenerator* Map);
	Tile* check_actor_tile_collisions(GameActor* actor, MapGenerator* map);

};
