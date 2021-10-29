#pragma once

#include <vector>
#include "GameActor.h"


class CollisionManager {

	std::vector<GameActor*>* actors;

	//HELPER FUNCTIONS

	bool circleRect(Hitbox* circle, Hitbox* rect);
public:

	CollisionManager(std::vector<GameActor*>* actors);
	void check_collisions();


};
