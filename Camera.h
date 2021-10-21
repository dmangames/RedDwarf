#pragma once
#include <SDL.h>

class Camera {
private:
	SDL_Rect camera;
	int width = 1280;
	int height = 720;
	int x_offset = 0;
	int y_offset = 0;
public:
	Camera();
	Camera(int width, int height);
	int get_x_offset(); //this is actually pos + offset
	int get_y_offset(); //this is actually pos + offset
	void setPos(int x, int y);
	
};