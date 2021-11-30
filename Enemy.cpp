#include "Enemy.h"

#define DEBUG true

#define WIDTH 32
#define HEIGHT 32
#define XOFFSET 16
#define YOFFSET 16
#define YADJUST 0

//PRIVATE FUNCTIONS

void Enemy::setAnimState(AnimState s) {
    switch (s) {
    case AnimState::IDLE:
        if (state != AnimState::IDLE) {
            state = AnimState::IDLE;
            startFrame = IDLE_ANIMATION_START;
            updatesPerFrame = IDLE_UPDATE_RATE;
            frame = 0;
        }
        break;
    case AnimState::WALK:
        if (state != AnimState::WALK) {
            state = AnimState::WALK;
            startFrame = WALK_ANIMATION_START;
            updatesPerFrame = WALK_UPDATE_RATE;
            frame = 0;
        }
        break;
    case AnimState::BITE:
        if (state != AnimState::BITE) {
            state = AnimState::BITE;
            startFrame = BITE_ANIMATION_START;
            updatesPerFrame = BITE_UPDATE_RATE;
            frame = 0;
        }
        break;
    }
}


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
    frame = 1;
    startFrame = 1;
    updatesPerFrame = 16;
    bite_cooldown = 3.0f;
    bite_timer = 0;

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

    //Set anim states
    if (state == AnimState::BITE) {

    }
    else if (abs(vy) < 3.0f && abs(vx) < 3.0f) {
        setAnimState(AnimState::IDLE);
    }
    else {
        setAnimState(AnimState::WALK);
    }

    switch (state) {
    case AnimState::IDLE:
        if (frame >= (IDLE_ANIMATION)*updatesPerFrame - 1) {
            frame = 0;
        }
        else {
            frame++;
        }
        break;
    case AnimState::WALK:
        if (frame >= (WALK_ANIMATION)*updatesPerFrame - 1) {
            frame = 0;
        }
        else {
            frame++;
        }
        break;
    case AnimState::BITE:
        if (frame >= (BITE_ANIMATION)*updatesPerFrame - 1) {
            setAnimState(AnimState::IDLE);
        }
        else {
            frame++;
            //if we are on the bite down frame, then check to see if the mouth point is in the player
            
            //if (frame == BITE_ANIMATION * updatesPerFrame - 15) {
            //    // Activate the fist
            //    if (flip == SDL_FLIP_HORIZONTAL) {
            //        fist->set_x(this->x + 33);
            //        fist->set_y(this->y + 12);
            //    }
            //    else {
            //        fist->set_x(this->x - 10);
            //        fist->set_y(this->y + 12);
            //    }
            //    fist->update(delta);
            //    fist->set_active(true);
            //}
            //else {
            //    fist->set_active(false);
            //}
        }
        break;
    }

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

    SDL_RenderCopyEx(renderer, texture, resources->get_anim_rect_32(startFrame + frame/updatesPerFrame), &dst, angle, NULL, flip);

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
    // Try to bite the player
    if (bite_timer <= 0) {
        if (flip == SDL_FLIP_NONE) {
            bite_box.x = x + w/2;
            bite_box.y = y - 2;
            bite_box.w = w;
            bite_box.h = h + 4;
        }
        else {
            bite_box.x = x - w/2;
            bite_box.y = y - 2;
            bite_box.w = w;
            bite_box.h = h + 4;
        }
        

        SDL_Rect result;
        //check to see if the player is in the right place
        
        if (SDL_IntersectRect(&bite_box, actor->get_hitbox()->get_rect(), &result)) {
            //we are currently able to bite the player, start the bite
            setAnimState(AnimState::BITE);
            bite_timer = bite_cooldown;
        }
        
    }
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
            //tile->type = TileType::EMPTY;
            tile->health = 0;
            tile->max_health = 0;
        }
        setAnimState(AnimState::BITE);
        bite_timer = bite_cooldown;
    }

}

void Enemy::resolve_collisions()
{
    isColliding = false;
}


void Enemy::take_damage(int damage)
{
}
