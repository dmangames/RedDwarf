#pragma once
#include "Item.h"
class TileItem :
    public Item
{
    TileType type;
    std::string sprite_name;
public:
    TileItem(TileType type, float x, float y, int w, int h);
    void render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) override;
    const GameActorType get_id() override;
    void collide_actor(GameActor* actor) override;
    int get_tile_minerals();
};

