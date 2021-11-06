#pragma once
#include <SDL.h>

class Camera {
private:
	SDL_Rect camera;
	int width = 1280;
	int height = 720;
	int x_offset = 0;
	int y_offset = 0;
	int destX, destY = 0;
public:
	Camera();
	Camera(int width, int height);
	void update();
	void setDestination(int x, int y);
	int get_x_offset(); //this is actually pos + offset
	int get_y_offset(); //this is actually pos + offset
	void setPos(int x, int y);
	bool in_camera_view(SDL_Rect* test); //checks to see if the rect is in the camera's view
	int get_x();
	int get_y();
	int get_width();
	int get_height();
	
};