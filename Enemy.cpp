#include "Enemy.h"

#define DEBUG true

#define WIDTH 32
#define HEIGHT 32
#define XOFFSET 16
#define YOFFSET 10
#define YADJUST 4

Enemy::Enemy(float x, float y, int w, int h, int screen_w, int screen_h)
	: GameActor(x, y, w, h, screen_w, screen_h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    hitbox = new Hitbox(XOFFSET / 2, YOFFSET / 2 + YADJUST, w - XOFFSET, h - YOFFSET, HitboxType::CIRCLE);
    vx = vy = 0.0f;
    linear_accel = 500.0f;
    friction_coeff = 0.2f;
    max_speed = 300.0f;
    angle = 0;
    alive = true;
    isColliding = false;
    frame = 0;
    startFrame = 0;
    updatesPerFrame = 16;

    load_animations();
    // Update hitbox
    hitbox->update_pos(x, y, angle);
}

void Enemy::update(float delta)
{
    // Update hitbox
    hitbox->update_pos(x, y, angle);

}

void Enemy::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera)
{
    SDL_Texture* texture;
    texture = resources->get_texture("pinkling", 1);

    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    SDL_Rect dst = {
        (int)x - camera->get_x_offset(),
        (int)y - camera->get_y_offset(),
        w,
        h
    };

    //SDL_RenderCopyEx(renderer, texture, &anim_rects[frame / updatesPerFrame + startFrame], &dst, angle, NULL, flip);

    SDL_RenderCopyEx(renderer, texture, resources->get_anim_rect_32(0), &dst, angle, NULL, flip);

    //DEBUG Render hitbox
    if (DEBUG) {
        hitbox->render_corners(renderer, camera);
    }
}


bool Enemy::is_alive()
{
	return alive;
}

const GameActorType Enemy::get_id()
{
	return GameActorType::ENEMY;
}

const bool Enemy::collides()
{
	return false;
}

bool Enemy::does_collide(GameActorType id)
{
	return false;
}

void Enemy::collide_actor(GameActor* actor)
{
}

void Enemy::collide_tile(Tile* tile)
{
}

void Enemy::resolve_collisions()
{
}

void Enemy::load_animations()
{
}

void Enemy::take_damage(int damage)
{
}
