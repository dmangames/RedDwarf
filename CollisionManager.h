#pragma once

#include <vector>
#include "GameActor.h"


class CollisionManager {

	std::vector<GameActor*>* actors;

public:

	CollisionManager(std::vector<GameActor*>* actors);
	void check_collisions();

};
