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
			actor.setPos(actor.getX(), actor.getY() + 5);
			break;
		case MOVE_DOWN:
			actor.setPos(actor.getX(), actor.getY() - 5);
			break;
		case MOVE_LEFT:
			actor.setPos(actor.getX() - 5, actor.getY());
			break;
		case MOVE_RIGHT:
			actor.setPos(actor.getX() + 5, actor.getY());
			break;
		}
	}
};