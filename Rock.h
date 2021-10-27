#pragma once
#include "DestructibleObject.h"



class Rock :public DestructibleObject {
	SDL_Rect anim_rects[8];
public:
	Rock(float x, float y, int screen_w, int screen_h);
	const GameActorType get_id();
	void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera);
	void take_damage(int i);
	void on_destroy();
	void load_animations();
};