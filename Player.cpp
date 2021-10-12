#include "Player.h"

#define WIDTH 32
#define HEIGHT 32

// PUBLIC FUNCTIONS

Player::Player(float x, float y, int player_num, int screen_w, int screen_h, std::vector<GameActor*>* actors)
        : GameActor(x, y, WIDTH, HEIGHT, screen_w, screen_h){
	this->w = WIDTH;
	this->h = HEIGHT;
    hitbox = new Hitbox(0, 0, w, h);
    vx = vy = 0.0f;
    linear_accel = 500.0f;
    friction_coeff = 0.8f;
    max_speed = 300.0f;
    angle = 0;
    alive = true;

    this->player_num = player_num;
    this->actors = actors;

    load_animations();
}

void Player::update(float delta) {

    printf("X Velocity: %4.2f\n", vx);

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




    //check_bounds();

    //hitbox->update_pos(x, y, angle);
}

void Player::render(SDL_Renderer* renderer, Resources* resources, float delta) {
	SDL_Texture* texture;
	texture = resources->get_texture("dwarf", 1);

    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);
    SDL_Rect dst = {
        (int)x,
        (int)y,
        w,
        h
    };

    /*SDL_RenderCopyEx(renderer, texture, NULL, &dst, angle / M_PI * 180 + 90,
        NULL, SDL_FLIP_NONE);*/
    SDL_RenderCopyEx(renderer, texture, &anim_rects[0], &dst, angle, NULL, flip);
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

const int Player::get_id() {
    return 0;
}

const bool Player::collides() {
    return true;
}

// Checks for collisions with Missiles
bool Player::does_collide(int id) {
    return id == 1 || id == 4;
}

void Player::collide_actor(GameActor* actors) {


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