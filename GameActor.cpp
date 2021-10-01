/*Game Actor*/
#include "GameActor.h"

GameActor::GameActor(std::string name) {
	m_name = name;
	setPos(0, 0);
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

void GameActor::setName(std::string name) {
	m_name = name;
}

void GameActor::setPos(int x, int y) {
	m_xpos = x;
	m_ypos = y;
}