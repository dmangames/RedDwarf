#pragma once
#include "Command.h"

enum MoveDirection
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

class MoveCommand:public Command {
private:
	MoveDirection m_direction;
public:
	MoveCommand(MoveDirection direction): m_direction(direction) {};
	virtual void execute(GameActor& actor){
		switch (m_direction) {
		case MOVE_UP:
			actor.set_y(actor.get_y() - 5);
			break;
		case MOVE_DOWN:
			actor.set_y(actor.get_y() + 5);
			break;
		case MOVE_LEFT:
			actor.set_x(actor.get_x() - 5);
			break;
		case MOVE_RIGHT:
			actor.set_x(actor.get_x() + 5);
			break;
		}
	}
};