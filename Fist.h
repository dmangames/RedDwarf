#pragma once
#include "Weapon.h"

class Fist : public Weapon {
public:
	Fist(float x, float y, int w, int h, int screen_w, int screen_h);
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	void collide_actor(GameActor* actor);

};
