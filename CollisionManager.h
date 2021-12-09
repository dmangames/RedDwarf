#pragma once

#include <vector>
#include "GameActor.h"
#include "MapGenerator.h"

class CollisionManager {

	//Lists of actor by types
	//std::vector<GameActor*>* actors;
	//std::vector<EnemyWeapon*>* enemy_weapons;
	
	/*std::vector<GameActor*>* actors;*/

	//HELPER FUNCTIONS

	/*bool circleRect(Hitbox* circle, Hitbox* rect);
	bool intersect_rect_tile(int tile_left, int tile_right, int tile_bottom, int tile_top, Hitbox* h1);
	bool intersect_circle_tile(Hitbox* circle, int tile_x, int tile_y, int tile_w, int tile_h);*/
public:
	CollisionManager();
	void check_actor_collisions(std::vector<GameActor*>* actors);
	void check_collisions(std::vector<GameActor*>& actors1, std::vector<GameActor*>& actors2);
	void check_tile_collisions(MapGenerator* Map, std::vector<GameActor*>* actors);
	Tile* check_actor_tile_collisions(GameActor* actor, MapGenerator* map);

};
