#pragma once
#include "Command.h"
#include "MoveCommand.h"
#include <SDL.h>

class InputHandler {
private:
	Command* up_arrow_;
	Command* down_arrow_;
	Command* left_arrow_;
	Command* right_arrow_;
	Command* quit_;

public:
	InputHandler();
	Command* handleInput();

};

//default bindings
InputHandler::InputHandler() {
	up_arrow_ = new MoveCommand(MOVE_UP);
	down_arrow_ = new MoveCommand(MOVE_DOWN);
	left_arrow_ = new MoveCommand(MOVE_LEFT);
	right_arrow_ = new MoveCommand(MOVE_RIGHT);
}


//returns a command to be used on an actor
Command* InputHandler::handleInput() {
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_UP]) {
		return up_arrow_;
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN]) {
		return down_arrow_;
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		return left_arrow_;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		return right_arrow_;
	}
	return NULL;
}