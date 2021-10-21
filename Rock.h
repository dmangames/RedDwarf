#pragma once
#include "DestructibleObject.h"



class Rock :public DestructibleObject {
public:
	Rock(float x, float y, int screen_w, int screen_h);
	void render(SDL_Renderer* renderer, Resources* resources, float delta);
};