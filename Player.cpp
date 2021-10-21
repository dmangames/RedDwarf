#include "Player.h"

#define DEBUG false


#define WIDTH 32
#define HEIGHT 32

// PUBLIC FUNCTIONS

Player::Player(float x, float y, int player_num, int screen_w, int screen_h, std::vector<GameActor*>* actors, Camera* camera)
        : GameActor(x, y, WIDTH, HEIGHT, screen_w, screen_h){
    this->x = x;
    this->y = y;
    this->w = WIDTH;
	this->h = HEIGHT;
    hitbox = new Hitbox(0, 0, w, h);
    vx = vy = 0.0f;
    linear_accel = 500.0f;
    friction_coeff = 0.8f;
    max_speed = 300.0f;
    angle = 0;
    alive = true;
    isColliding = false;
    state = AnimState::IDLE;
    frame = 0;
    startFrame = 0;
    updatesPerFrame = 16;

    this->camera = camera;

    px = x;
    py = y;

    this->player_num = player_num;
    this->actors = actors;

    load_animations();
}

void Player::update(float delta) {

    //printf("X Velocity: %4.2f\n", vx);

    px = x;
    py = y;

    // Apply change in x and y directions
    x += delta * vx;
    y += delta * vy;


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

    if (abs(vy) < 3.0f && abs(vx) < 3.0f) {

        if (state != AnimState::IDLE) {
            state = AnimState::IDLE;
            startFrame = IDLE_ANIMATION_START;
            updatesPerFrame = 16;
            frame = 0;
        }

    }
    else {

        if (state != AnimState::WALK) {
            state = AnimState::WALK;
            startFrame = WALK_ANIMATION_START;
            updatesPerFrame = 4;
            frame = 0;
        }
    }

    switch (state) {
    case AnimState::IDLE:
        if (frame >= (IDLE_ANIMATION) * updatesPerFrame - 1) {
            frame = 0;
        }
        else {
            frame++;
        }
        break;
        case AnimState::WALK:
        if (frame >= (WALK_ANIMATION) * updatesPerFrame - 1) {
            frame = 0;
        }
        else {
            frame++;
        }
        break;
    }

    //check_bounds();

    hitbox->update_pos(x, y, angle);

    // Update camera
    camera->setPos((int)x, (int)y);
    
}

void Player::render(SDL_Renderer* renderer, Resources* resources, float delta, Camera* camera) {
	SDL_Texture* texture;
	texture = resources->get_texture("dwarf", 1);

    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    SDL_Rect dst = {
        (int)x - camera->get_x_offset(),
        (int)y - camera->get_y_offset(),
        w,
        h
    };

    /*SDL_RenderCopyEx(renderer, texture, NULL, &dst, angle / M_PI * 180 + 90,
        NULL, SDL_FLIP_NONE);*/

    SDL_RenderCopyEx(renderer, texture, &anim_rects[frame/updatesPerFrame + startFrame], &dst, angle, NULL, flip);

    //DEBUG Render hitbox
    if (DEBUG) {
        hitbox->render_corners(renderer);
    }
}

void Player::handle_inputs(float delta, InputHandler* inputs) {
    if (player_num == 1) {
        bool activate_friction = true;
        // Movement Controls
        if (inputs->is_key_down(KEY_P1_MOVE_UP)) {
            vy -= delta * linear_accel;
            activate_friction = false;
        }
        if (inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
            vy += delta * linear_accel;
            activate_friction = false;
        }
        if (inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
            vx += delta * linear_accel;
            activate_friction = false;
            if (vx > 0.1) {
                // Face Right
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
        if (inputs->is_key_down(KEY_P1_MOVE_LEFT)) {
            vx -= delta * linear_accel;
            activate_friction = false;
            if (vx < -0.1) {
                // Face Left
                flip = SDL_FLIP_NONE;
            }
        }
        if (activate_friction) {
            vx *= friction_coeff;
            vy *= friction_coeff;
        }
    }
}

bool Player::is_alive() {
    return alive;
}

const GameActorType Player::get_id() {
    return GameActorType::PLAYER;
}

const bool Player::collides() {
    return true;
}

// Checks for collisions
bool Player::does_collide(GameActorType type) {
    return type == GameActorType::DESTRUCTABLE || type == GameActorType::ENEMY;
}

void Player::collide_actor(GameActor* actor) {
    switch (actor->get_id()) {
    case GameActorType::DESTRUCTABLE:
        printf("Colliding with destructible object.\n");
        x = px;
        y = py;
        camera->setPos((int)x, (int)y);
        break;
    case GameActorType::ENEMY:
        printf("Colliding with enemy object.\n");
        break;
    case GameActorType::PLAYER:
        printf("Colliding with player object.\n");
        break;
    default:
        break;
    }

}

int Player::get_player_num() {
    return player_num;
}

void Player::load_animations() {
		//Set sprite clips
		//IDLE
		anim_rects[0].x = 0;
		anim_rects[0].y = 0;
		anim_rects[0].w = 32;
		anim_rects[0].h = 32;

		anim_rects[1].x = 32;
		anim_rects[1].y = 0;
		anim_rects[1].w = 32;
		anim_rects[1].h = 32;

		//WALK LEFT
		anim_rects[2].x = 64;
		anim_rects[2].y = 0;
		anim_rects[2].w = 32;
		anim_rects[2].h = 32;

		anim_rects[3].x = 96;
		anim_rects[3].y = 0;
		anim_rects[3].w = 32;
		anim_rects[3].h = 32;

		anim_rects[4].x = 128;
		anim_rects[4].y = 0;
		anim_rects[4].w = 32;
		anim_rects[4].h = 32;

		anim_rects[5].x = 160;
		anim_rects[5].y = 0;
		anim_rects[5].w = 32;
		anim_rects[5].h = 32;

		anim_rects[6].x = 192;
		anim_rects[6].y = 0;
		anim_rects[6].w = 32;
		anim_rects[6].h = 32;

		anim_rects[7].x = 224;
		anim_rects[7].y = 0;
		anim_rects[7].w = 32;
		anim_rects[7].h = 32;

		anim_rects[8].x = 256;
		anim_rects[8].y = 0;
		anim_rects[8].w = 32;
		anim_rects[8].h = 32;

		anim_rects[9].x = 288;
		anim_rects[9].y = 0;
		anim_rects[9].w = 32;
		anim_rects[9].h = 32;

}