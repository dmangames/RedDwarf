#pragma once

#include <vector>
#include <string>
#include <map>
#include "LTexture.h"
#include "Resources.h"
#include "Hitbox.h"
#include "Camera.h"
#include "MapGenerator.h"

enum class GameActorType {
	PLAYER,
	ENEMY,
	DESTRUCTABLE,
	WEAPON,
	ROCK
};


class GameActor {
protected:
	float x, y;
	int w, h;
	int screen_w, screen_h;
	Hitbox* hitbox;
	bool active; // Used to toggle if world should take this actor into account
	float get_center_x();
	float get_center_y();
	void check_bounds();
//private:
//	std::string m_name;
//	int m_xpos;
//	int m_ypos;
//	//Animation animations[5];
//	//Idle animation
//	int IDLE_ANIMATION_FRAMES = 2;
//	//Walking animation
//	int WALKING_ANIMATION_FRAMES = 8;
//	std::map <std::string, Animation> animationMap;
//	SDL_Rect* gSpriteClips;
//	LTexture gSpriteSheetTexture;
public:
	GameActor();
	GameActor(float x, float y, int w, int h, int screen_w, int screen_h);
	Hitbox* get_hitbox();
	float get_x();
	float get_y();
	void set_x(float x);
	void set_y(float y);
	void set_active(bool isActive);
	bool is_active();
	//GameActor(std::string name, int idleFrames = 0, int walkingFrames = 0);
	//GameActor(std::string name, Animation aList[]);
	virtual ~GameActor() = 0;
	virtual void update(float delta) = 0;
	virtual void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) = 0;
	virtual bool is_alive() = 0;
	virtual const GameActorType get_id() = 0;
	virtual const bool collides() = 0;
	virtual bool does_collide(GameActorType type) = 0;
	virtual void collide_actor(GameActor* actor) = 0;
	virtual void collide_tile(Tile* tile) = 0;
	virtual void resolve_collisions() = 0;
	virtual void take_damage(int damage) = 0;



	//bool loadSpriteSheet(std::string pathToSpriteSheet, int cellWidth, int cellHeight);
	//void addAnimation(std::string animName, int startFrame, int numFrames);
	//void playAnimation(std::string animName);
};

//GameActor::~GameActor() {
//	gSpriteSheetTexture.free();
//}
//
//bool GameActor::loadSpriteSheet(std::string pathToSpriteSheet, int cellWidth, int cellHeight) {
//	bool success = true;
//	if (!gSpriteSheetTexture.loadFromFile(pathToSpriteSheet))
//	{
//		printf("Failed to load sprite sheet %s!\n", pathToSpriteSheet);
//		success = false;
//	}
//	else
//	{
//		//Set sprite clips
//		//IDLE
//		gSpriteClips[0].x = 0;
//		gSpriteClips[0].y = 0;
//		gSpriteClips[0].w = 32;
//		gSpriteClips[0].h = 32;
//
//		gSpriteClips[1].x = 32;
//		gSpriteClips[1].y = 0;
//		gSpriteClips[1].w = 32;
//		gSpriteClips[1].h = 32;
//
//		//WALK LEFT
//		gSpriteClips[2].x = 64;
//		gSpriteClips[2].y = 0;
//		gSpriteClips[2].w = 32;
//		gSpriteClips[2].h = 32;
//
//		gSpriteClips[3].x = 96;
//		gSpriteClips[3].y = 0;
//		gSpriteClips[3].w = 32;
//		gSpriteClips[3].h = 32;
//
//		gSpriteClips[4].x = 128;
//		gSpriteClips[4].y = 0;
//		gSpriteClips[4].w = 32;
//		gSpriteClips[4].h = 32;
//
//		gSpriteClips[5].x = 160;
//		gSpriteClips[5].y = 0;
//		gSpriteClips[5].w = 32;
//		gSpriteClips[5].h = 32;
//
//		gSpriteClips[6].x = 192;
//		gSpriteClips[6].y = 0;
//		gSpriteClips[6].w = 32;
//		gSpriteClips[6].h = 32;
//
//		gSpriteClips[7].x = 224;
//		gSpriteClips[7].y = 0;
//		gSpriteClips[7].w = 32;
//		gSpriteClips[7].h = 32;
//
//		gSpriteClips[8].x = 256;
//		gSpriteClips[8].y = 0;
//		gSpriteClips[8].w = 32;
//		gSpriteClips[8].h = 32;
//
//		gSpriteClips[9].x = 288;
//		gSpriteClips[9].y = 0;
//		gSpriteClips[9].w = 32;
//		gSpriteClips[9].h = 32;
//	}
//	return success;
//}
//
//void GameActor::render() {
//	//Render current frame
//	SDL_Rect* currentClip = &gSpriteClips[frame / 4 + IDLE_ANIMATION_FRAMES];
//	//gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, 0, 0, direction);
//	gSpriteSheetTexture.render(player->getX(), player->getY(), currentClip, 0, 0, direction);
//}