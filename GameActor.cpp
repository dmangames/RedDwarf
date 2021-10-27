/*Game Actor*/
#include "GameActor.h"
#include "Hitbox.h"

// PRIVATE HELPER METHODS

float GameActor::get_center_x() {
	return x + w / 2;
}

float GameActor::get_center_y() {
	return y + h / 2;
}

void GameActor::check_bounds() {
	// check bounds can be used for now to make the player not leave the screen
	if (this->get_center_x() < 0) {
		x = 0 + w / 2;
	}
	else if (this->get_center_x() > screen_w) {
		x = screen_w - w / 2;
	}
	if (this->get_center_y() < 0) {
		y = 0 + h / 2;
	}
	else if (this->get_center_y() > screen_h) {
		y = screen_h - h / 2;
	}
}

// PUBLIC METHODS

GameActor::GameActor() {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	screen_w = 0;
	screen_h = 0;
	hitbox = NULL;
	active = true;
}

GameActor::GameActor(float x, float y, int w, int h, int screen_w, int screen_h) {
	x = x;
	y = y;
	w = w;
	h = h;
	screen_w = screen_w;
	screen_h = screen_h;
	hitbox = NULL;
	active = true;
}

Hitbox* GameActor::get_hitbox() {
	return hitbox;
}

float GameActor::get_x() {
	return x;
}

float GameActor::get_y() {
	return y;
}

void GameActor::set_x(float x) {
	this->x = x;
}

void GameActor::set_y(float y) {
	this->y = y;
}

void GameActor::set_active(bool isActive) {
	this->active = isActive;
}

bool GameActor::is_active() {
	return active;
}

GameActor::~GameActor() {
	if (hitbox != NULL) {
		delete hitbox;
	}
}
//GameActor::GameActor(std::string name, int idleFrames, int walkingFrames) {
//	m_name = name;
//	setPos(0, 0);
//	IDLE_ANIMATION_FRAMES = idleFrames;
//	WALKING_ANIMATION_FRAMES = walkingFrames;
//}


//GameActor::GameActor(std::string name, Animation aList[]) {
//	m_name = name;
//	setPos(0, 0);
//	//Copy the animations over
//	for (int i = 0; i < sizeof(aList)/sizeof(Animation); i++) {
//		animations[i] = aList[i];
//	}
//}
