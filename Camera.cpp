#include "Camera.h"

#include <cmath>

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

void Camera::update()
{
	float lerpX = std::lerp(camera.x, destX, .3);
	float lerpY = std::lerp(camera.y, destY, .3);

	if (std::abs(destX - lerpX) < .1) {
		camera.x = destX;
	}
	else {
		camera.x = lerpX;
	}
	if (std::abs(destY - lerpY) < .1) {
		camera.y = destY;
	}
	else {
		camera.y = lerpY;
	}
}

void Camera::setDestination(int x, int y)
{
	destX = x;
	destY = y;
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

bool Camera::in_camera_view(SDL_Rect* test)
{
	SDL_Rect current_cam_rect;
	current_cam_rect.x = get_x_offset();
	current_cam_rect.y = get_y_offset();
	current_cam_rect.w = width;
	current_cam_rect.h = height;
	if (SDL_HasIntersection(&current_cam_rect, test)) {
		return true;
	}
	return false;
}

int Camera::get_x() {
	return camera.x;
}

int Camera::get_y() {
	return camera.y;
}

int Camera::get_width()
{
	return width;
}

int Camera::get_height()
{
	return height;
}
