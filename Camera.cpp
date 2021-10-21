#include "Camera.h"

Camera::Camera()
{
	camera.w = width;
	camera.h = height;
	camera.x = 0;
	camera.y = 0;
	x_offset = width / 2;
	y_offset = height / 2;
}

Camera::Camera(int width, int height) {
	this->width = width;
	this->height = height;
	camera.w = width;
	camera.h = height;
	camera.x = 0;
	camera.y = 0;
	x_offset = width / 2;
	y_offset = height / 2;
}

int Camera::get_x_offset() {
	return camera.x - x_offset;
}

int Camera::get_y_offset() {
	return camera.y - y_offset;
}

void Camera::setPos(int x, int y) {
	camera.x = x;
	camera.y = y;
}