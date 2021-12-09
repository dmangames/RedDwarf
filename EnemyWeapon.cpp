#include "EnemyWeapon.h"

#define DEBUG true

EnemyWeapon::EnemyWeapon(float x, float y, int w, int h, int damage) : Weapon(x, y, w, h, 0, 0)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->damage = damage;
    this->hitbox = new Hitbox(0, 0, w, h);
}

const GameActorType EnemyWeapon::get_id()
{
    return GameActorType::ENEMY_WEAPON;
}

void EnemyWeapon::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
    //DEBUG Render hitbox
    if (DEBUG) {
        hitbox->render_corners(renderer, camera);
    }
}

void EnemyWeapon::collide_actor(GameActor* actor)
{
    if (actor->get_id() == GameActorType::PLAYER && active) {
        //Apply next layer of destruction
        printf("BITE!\n");
        actor->take_damage(1);
        set_active(false);
        printf("Set active to %d\n", is_active());
    }
}

void EnemyWeapon::collide_tile(Tile* tile)
{
}

void EnemyWeapon::resolve_collisions()
{
}

int EnemyWeapon::get_damage()
{
    return damage;
}
