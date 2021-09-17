/*Game Actor*/
#include "GameActor.h"

GameActor::GameActor(std::string name) {
	m_name = name;
	setPos(0, 0);
}

void GameActor::setName(std::string name) {
	m_name = name;
}

void GameActor::setPos(int x, int y) {
	m_xpos = x;
	m_ypos = y;
}