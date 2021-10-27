#pragma once
#include "Command.h"
#include "MoveCommand.h"
#include <SDL.h>

// KEY CODE DEFINITIONS
// Edit this file to change key bindings

#define KEY_P1_MOVE_UP      SDL_SCANCODE_W
#define KEY_P1_MOVE_DOWN    SDL_SCANCODE_S
#define KEY_P1_MOVE_LEFT    SDL_SCANCODE_A
#define KEY_P1_MOVE_RIGHT   SDL_SCANCODE_D
#define KEY_P1_PUNCH		SDL_SCANCODE_SPACE
#define KEY_P1_BUILD		SDL_SCANCODE_E

#define KEY_P2_MOVE_UP      SDL_SCANCODE_UP
#define KEY_P2_MOVE_DOWN    SDL_SCANCODE_DOWN
#define KEY_P2_MOVE_LEFT    SDL_SCANCODE_LEFT
#define KEY_P2_MOVE_RIGHT   SDL_SCANCODE_RIGHT
#define KEY_P2_FIRE_MISSILE SDL_SCANCODE_KP_ENTER
#define KEY_P2_FIRE_LASER   SDL_SCANCODE_KP_PERIOD

class InputHandler {
private:
	Command* up_arrow_;
	Command* down_arrow_;
	Command* left_arrow_;
	Command* right_arrow_;
	Command* quit_;

	bool quit;
	int mouse_x;
	int mouse_y;
	// Boolean array for all keys. See above definitions
	const Uint8* key_states;
	// Returns true if ESC or 'X' button is pressed
	bool check_for_quit(SDL_Event event);

public:
	InputHandler();
	void update();
	bool get_quit();
	bool is_key_down(int key);
	Command* handleInput();

};

////default bindings
//InputHandler::InputHandler() {
//	up_arrow_ = new MoveCommand(MOVE_UP);
//	down_arrow_ = new MoveCommand(MOVE_DOWN);
//	left_arrow_ = new MoveCommand(MOVE_LEFT);
//	right_arrow_ = new MoveCommand(MOVE_RIGHT);
//}
//
//
////returns a command to be used on an actor
//Command* InputHandler::handleInput() {
//	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
//	if (currentKeyStates[SDL_SCANCODE_UP]) {
//		return up_arrow_;
//	}
//	if (currentKeyStates[SDL_SCANCODE_DOWN]) {
//		return down_arrow_;
//	}
//	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
//		return left_arrow_;
//	}
//	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
//		return right_arrow_;
//	}
//	return NULL;
//}