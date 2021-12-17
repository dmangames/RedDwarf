#include "TileItem.h"

#define DEBUG true

TileItem::TileItem(TileType type, float x, float y, int w, int h):Item(x, y, w, h)
{
    this->type = type;
    switch (type) {
    case TileType::REDROCK:
        sprite_name = "redrock";
        break;
    case TileType::PURPLEROCK:
        sprite_name = "purplerock";
        break;
    default:
        sprite_name = "redrock";
    }
    printf("Loc X: %f", x);

    active = true;
    hitbox = new Hitbox(0, 0, w, h, HitboxType::CIRCLE);
    hitbox->update_pos(x, y, 0);
}

void TileItem::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
    SDL_Texture* texture;
    texture = resources->get_texture(sprite_name, 2);

    SDL_Rect dst = {
        (int)x - camera->get_x_offset(),
        (int)y - camera->get_y_offset(),
        w,
        h
    };

    //SDL_RenderCopyEx(renderer, texture, &anim_rects[frame / updatesPerFrame + startFrame], &dst, angle, NULL, flip);

    SDL_RenderCopyEx(renderer, texture, resources->get_anim_rect_32(0), &dst, 0, NULL, SDL_FLIP_NONE);

    //DEBUG Render hitbox
    if (DEBUG) {
        hitbox->render_corners(renderer, camera);
    }
}

const GameActorType TileItem::get_id()
{
    return GameActorType::TILEITEM;
}

void TileItem::collide_actor(GameActor* actor)
{
    //mark for deletion
    alive = false;
}

int TileItem::get_tile_minerals()
{
    switch (type) {
    case TileType::REDROCK:
        return 1;
        break;
    case TileType::PURPLEROCK:
        return 4;
    case TileType::GOLDROCK:
        return 8;
        break;

    }
}
