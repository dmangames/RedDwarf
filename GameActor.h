#pragma once
#include <string>

class GameActor {
private:
	std::string m_name;
	int m_xpos;
	int m_ypos;
public:
	GameActor(std::string name);
	void setName(std::string name);
	void setPos(int x, int y);

	std::string getName() { return m_name; }
	int getX() { return m_xpos; }
	int getY() { return m_ypos; }
};