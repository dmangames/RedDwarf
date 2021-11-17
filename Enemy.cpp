#include "Enemy.h"

#define DEBUG true

#define WIDTH 32
#define HEIGHT 32
#define XOFFSET 16
#define YOFFSET 10
#define YADJUST 4

Enemy::Enemy(float x, float y, int w, int h, int screen_w, int screen_h, Player* player)
	: GameActor(x, y, w, h, screen_w, screen_h)
{
    this->player = player;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->px = x;
    this->py = y;
    hitbox = new Hitbox(XOFFSET / 2, YOFFSET / 2 + YADJUST, w - XOFFSET, h - YOFFSET, HitboxType::CIRCLE);
    vx = vy = 0.0f;
    linear_accel =  100.0f + rand() % 200;
    friction_coeff = 0.2f;
    max_speed = 300.0f;
    angle = 0;
    alive = true;
    isColliding = false;
    frame = 0;
    startFrame = 0;
    updatesPerFrame = 16;
    bite_cooldown = 3.0f;
    bite_timer = 0;

    load_animations();
    // Update hitbox
    hitbox->update_pos(x, y, angle);
}

void Enemy::update(float delta)
{
    px = x;
    py = y;
    float xdiff = player->get_x() - this->x;
    float ydiff = player->get_y() - this->y;
    float dist_to_player = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
    float xdir = xdiff / dist_to_player;
    float ydir = ydiff / dist_to_player;

    if (xdir < 0) {
        // Face Left
        flip = SDL_FLIP_HORIZONTAL;
    }
    else {
        flip = SDL_FLIP_NONE;
    }

    vx = xdir * linear_accel;
    vy = ydir * linear_accel;

    // Enforce speed limit
    if (vx > max_speed) {
        vx = max_speed;
    }
    if (vy > max_speed) {
        vy = max_speed;
    }
    if (vx < -max_speed) {
        vx = -max_speed;
    }
    if (vy < -max_speed) {
        vy = -max_speed;
    }

    // Move
    x = px + vx * delta;
    y = py + vy * delta;

    //Decrease bite timer
    if(bite_timer > 0)
        bite_timer -= delta;

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
	return true;
}

bool Enemy::does_collide(GameActorType id)
{
	return id == GameActorType::PLAYER;
}

void Enemy::collide_actor(GameActor* actor)
{
    isColliding = true;
    x = px;
    y = py;
}

void Enemy::collide_tile(Tile* tile)
{
    isColliding = true;
    x = px;
    y = py;
    // Try to damage the tile
    if (bite_timer <= 0) {
        tile->health = tile->health - 1;
        if (tile->health <= 0) {
            tile->type = TileType::EMPTY;
        }
        bite_timer = bite_cooldown;
    }

}

void Enemy::resolve_collisions()
{
    isColliding = false;
}

void Enemy::load_animations()
{
}

void Enemy::take_damage(int damage)
{
}
