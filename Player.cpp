#include "Player.h"

#define DEBUG true


#define WIDTH 32
#define HEIGHT 32
#define XOFFSET 16
#define YOFFSET 10
#define YADJUST 4

//PRIVATE FUNCTIONS

void Player::setAnimState(AnimState s) {
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
    case AnimState::PUNCH:
        if (state != AnimState::PUNCH) {
            state = AnimState::PUNCH;
            startFrame = PUNCH_ANIMATION_START;
            updatesPerFrame = PUNCH_UPDATE_RATE;
            frame = 0;
        }
        break;
    }
}

// PUBLIC FUNCTIONS

Player::Player(float x, float y, int player_num, int screen_w, int screen_h, std::vector<GameActor*>* actors, Camera* camera)
        : GameActor(x, y, WIDTH, HEIGHT, screen_w, screen_h){
    this->x = x;
    this->y = y;
    this->w = WIDTH;
	this->h = HEIGHT;
    hitbox = new Hitbox(XOFFSET/2, YOFFSET/2 + YADJUST, w - XOFFSET, h - YOFFSET, HitboxType::CIRCLE);
    fist = new Fist(0, 0, 10, 10, screen_w, screen_h);
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

    actors->push_back(fist);

    load_animations();
}

void Player::update(float delta) {

    //printf("X Velocity: %4.2f\n", vx);



    if (isColliding) {
        //printf("COLLIDING\n");
        // Apply change in x and y directions plus slide to the old position
        x = px + delta * (vx + slideX);
        y = py + delta * (vy + slideY);
        //isColliding = false;
    }
    else {
        px = x;
        py = y;
        // Apply change in x and y directions
        x += delta * vx;
        y += delta * vy;
    }

    //x += delta * slideX;
    //y += delta * slideY;

    if (!isColliding) {
        slideX = 0;
        slideY = 0;
    }
    else {
        isColliding = false;
    }




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

    if (state == AnimState::PUNCH) {

    }
    else if (abs(vy) < 3.0f && abs(vx) < 3.0f) {
        setAnimState(AnimState::IDLE);
    }
    else {
        setAnimState(AnimState::WALK);
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
        case AnimState::PUNCH:
            if (frame >= (PUNCH_ANIMATION) * updatesPerFrame - 1) {
                setAnimState(AnimState::IDLE);
            }
            else {
                frame++;
                if (frame == PUNCH_ANIMATION * updatesPerFrame - 15) {
                    // Activate the fist
                    if (flip == SDL_FLIP_HORIZONTAL) {
                        fist->set_x(this->x + 33);
                        fist->set_y(this->y + 12);
                    }
                    else {
                        fist->set_x(this->x - 10);
                        fist->set_y(this->y + 12);
                    }
                    fist->update(delta);
                    fist->set_active(true);
                }
                else {
                    fist->set_active(false);
                }
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
        hitbox->render_corners(renderer, camera);
        fist->render(renderer, resources, delta, camera);
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
        if (activate_friction || isColliding) {
            vx *= friction_coeff;
            vy *= friction_coeff;
        }

        // ACTIONS
        if (inputs->is_key_down(KEY_P1_PUNCH)) {
            if (state != AnimState::PUNCH) {
                state = AnimState::PUNCH;
                startFrame = PUNCH_ANIMATION_START;
                updatesPerFrame = 4;
                frame = 0;
            }
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
    return type == GameActorType::DESTRUCTABLE 
        || type == GameActorType::ENEMY 
        || type == GameActorType::ROCK;
}

void Player::collide_actor(GameActor* actor) {
    switch (actor->get_id()) {
    case GameActorType::DESTRUCTABLE:
        //printf("Colliding with destructible object.\n");
        x = px;
        y = py;
        camera->setPos((int)x, (int)y);
        break;
    case GameActorType::ROCK:
        x = px;
        y = py;
        isColliding = true;
        calculate_slide(actor->get_hitbox()->get_center_x(), actor->get_hitbox()->get_center_y());

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

void Player::collide_tile(Tile* tile)
{
    x = px;
    y = py;
    isColliding = true;
    //TODO: why do i add 16 to y as well? I thought sdl originated top left?
    calculate_slide(tile->x * 32 +16, tile->y * 32 + 16);

    camera->setPos((int)x, (int)y);
}

void Player::calculate_slide(float ax, float ay) {

    float distX = get_center_x() - ax;
    float distY = get_center_y() - ay;
    float magnitude = sqrt(pow(distX, 2) + pow(distY, 2));
    float dirX = distX / magnitude;
    float dirY = distY / magnitude;
    slideX += dirX * 10;
    slideY += dirY * 10;
}

int Player::get_player_num() {
    return player_num;
}

void Player::take_damage(int damage)
{
    //TODO:make player take damage
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

        // PUNCH
        anim_rects[10].x = 320;
        anim_rects[10].y = 0;
        anim_rects[10].w = 32;
        anim_rects[10].h = 32;

        anim_rects[11].x = 352;
        anim_rects[11].y = 0;
        anim_rects[11].w = 32;
        anim_rects[11].h = 32;

        anim_rects[12].x = 384;
        anim_rects[12].y = 0;
        anim_rects[12].w = 32;
        anim_rects[12].h = 32;

        anim_rects[13].x = 416;
        anim_rects[13].y = 0;
        anim_rects[13].w = 32;
        anim_rects[13].h = 32;

        anim_rects[14].x = 448;
        anim_rects[14].y = 0;
        anim_rects[14].w = 32;
        anim_rects[14].h = 32;

        anim_rects[15].x = 480;
        anim_rects[15].y = 0;
        anim_rects[15].w = 32;
        anim_rects[15].h = 32;

        anim_rects[16].x = 512;
        anim_rects[16].y = 0;
        anim_rects[16].w = 32;
        anim_rects[16].h = 32;



}