#include "Player.h"

#define DEBUG false


#define WIDTH 32
#define HEIGHT 32
#define XOFFSET 16
#define YOFFSET 10
#define YADJUST 4

#define RIGHT 1
#define LEFT 2
#define TOP 4
#define BOTTOM 8
#define TOPLEFT 16
#define TOPRIGHT 32
#define BOTTOMLEFT 64
#define BOTTOMRIGHT 128

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
    friction_coeff = 0.2f;
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

    // Store where I came from (old position before update)
    px = x;
    py = y;

    this->delta = delta;
   
    if (isColliding) {
        isColliding = false;
        if (collision_flags < 15) { // if not a corner, turn slide off
            slideX = 0;
            slideY = 0;
        }
        collision_flags = 0;
    }
    else{
        x += delta * vx;
        y += delta * vy;
        slideX = 0;
        slideY = 0;
    }

    

    //if (isColliding) {
    //    //printf("COLLIDING\n");
    //    // Apply change in x and y directions plus slide to the old position
    //    x = px + delta * (vx + slideX);
    //    y = py + delta * (vy + slideY);

    //    //isColliding = false;
    //}
    //else {
    //    px = x;
    //    py = y;
    //    // Apply change in x and y directions
    //    x += delta * vx;
    //    y += delta * vy;
    //}

    //x += delta * slideX;
    //y += delta * slideY;

    //if (!isColliding) {
    //    slideX = 0;
    //    slideY = 0;
    //}
    //else {
    //    isColliding = false;
    //}




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
    camera->setDestination((int)x, (int)y);
    
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
        bool activate_friction_x = true;
        bool activate_friction_y = true;

        bool is_up_key_pressed = inputs->is_key_down(KEY_P1_MOVE_UP);
        bool is_down_key_pressed = inputs->is_key_down(KEY_P1_MOVE_DOWN);
        bool is_left_key_pressed = inputs->is_key_down(KEY_P1_MOVE_LEFT);
        bool is_right_key_pressed = inputs->is_key_down(KEY_P1_MOVE_RIGHT);

        // Movement Controls
        if (inputs->is_key_down(KEY_P1_MOVE_UP)) {
            if (vy > 0) {
                vy = 0;
            }
            if (inputs->is_key_down(KEY_P1_MOVE_LEFT) || inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
                vy -= delta * linear_accel * .707;
            }
            else {
                vy -= delta * linear_accel;
            }
            activate_friction_y = false;
        }
        if (inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
            if (vy < 0) {
                vy = 0;
            }
            if (inputs->is_key_down(KEY_P1_MOVE_LEFT) || inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
                vy += delta * linear_accel * .707;
            }
            else {
                vy += delta * linear_accel;
            }
            activate_friction_y = false;
        }
        if (inputs->is_key_down(KEY_P1_MOVE_RIGHT)) {
            if (vx < 0) {
                vx = 0;
            }
            if (inputs->is_key_down(KEY_P1_MOVE_UP) || inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
                vx += delta * linear_accel * .707;
            }
            else {
                vx += delta * linear_accel;
            }
            activate_friction_x = false;
            if (vx > 0.1) {
                // Face Right
                flip = SDL_FLIP_HORIZONTAL;
            }
        }
        if (inputs->is_key_down(KEY_P1_MOVE_LEFT)) {
            if (vx > 0) {
                vx = 0;
            }
            if (inputs->is_key_down(KEY_P1_MOVE_UP) || inputs->is_key_down(KEY_P1_MOVE_DOWN)) {
                vx -= delta * linear_accel * .707;
            }
            else {
                vx -= delta * linear_accel;
            }
            activate_friction_x = false;
            if (vx < -0.1) {
                // Face Left
                flip = SDL_FLIP_NONE;
            }
        }
        //if (activate_friction_x || isColliding) {
        //    vx *= friction_coeff;
        //}
        //if (activate_friction_y || isColliding) {
        //    vy *= friction_coeff;
        //}

        if (activate_friction_x ) {
            vx *= friction_coeff;
        }
        if (activate_friction_y ) {
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
    isColliding = true;

    SDL_Rect r;
    r.x = tile->x * 32;
    r.y = tile->y * 32;
    r.w = 32;
    r.h = 32;

    // Side points
    SDL_Point tp, bp, lp, rp;
    tp.x = x + WIDTH / 2;
    tp.y = y;
    bp.x = x + WIDTH / 2;
    bp.y = y + HEIGHT;
    lp.x = x;
    lp.y = y + HEIGHT / 2;
    rp.x = x + WIDTH;
    rp.y = y + HEIGHT / 2;

    // Corner points
    SDL_Point tlp, blp, trp, brp;
    tlp.x = x;
    tlp.y = y;
    blp.x = x;
    blp.y = y + HEIGHT;
    trp.x = x + WIDTH;
    trp.y = y;
    brp.x = x + WIDTH;
    brp.y = y + HEIGHT;

    //check to see if i'm colliding at the top, bottom, left, or right
    if (SDL_PointInRect(&rp, &r)) {
        //the right side is colliding
        if (vx > 0) {
            vx = 0;
        }
        collision_flags = collision_flags | RIGHT;
    }
    if (SDL_PointInRect(&lp, &r)) {
        // the left side is colliding
        if (vx < 0) {
            vx = 0;
        }
        collision_flags = collision_flags | LEFT;
    }
    if (SDL_PointInRect(&bp, &r)) {
        // the bottom side is colliding
        if (vy > 0) {
            vy = 0;
        }
        collision_flags = collision_flags | BOTTOM;
    }
    if (SDL_PointInRect(&tp, &r)) {
        // the top side is colliding
        if (vy < 0) {
            vy = 0;
        }
        collision_flags = collision_flags | TOP;
    }

        //check to see if i'm colliding at the corners
        if (SDL_PointInRect(&tlp, &r)) {
            //the top left corner is colliding

            collision_flags = collision_flags | TOPLEFT;
        }
        if (SDL_PointInRect(&trp, &r)) {
            // the top right corner is colliding

            collision_flags = collision_flags | TOPRIGHT;
        }
        if (SDL_PointInRect(&blp, &r)) {
            // the bottom left corner is colliding

            collision_flags = collision_flags | BOTTOMLEFT;
        }
        if (SDL_PointInRect(&brp, &r)) {
            // the bottom right is colliding

            collision_flags = collision_flags | BOTTOMRIGHT;
        }
    
    



    //Set back to position that doesn't collide with tile
    /*x = px;
    y = py;*/
    //TODO: why do i add 16 to y as well? I thought sdl originated top left?
    //The positive y axis points down, do the origin is found by adding half the width and half the height to x and y

    //if(collision_flags > 0 && ((collision_flags & 0x0f) == 0)) // we are dealing with only corners
        calculate_slide(tile->x * 32 +16, tile->y * 32 + 16);



    //Temp variables
    float tvx = vx;
    float tvy = vy;

    //If I only have side and corner collisions on the same side, I go with the corner collision

    // See if I have any side collisions
    if ((collision_flags & 0x0f) != 0)
    {
        if ((collision_flags & RIGHT) == RIGHT) {

                tvx = std::min(vx, 0.0f);
                printf("Colliding: RIGHT ");
            
        }
        if ((collision_flags & LEFT) == LEFT) {

                tvx = std::max(vx, 0.0f);
                printf("Colliding: LEFT ");
            
        }
        if ((collision_flags & TOP) == TOP) {

                tvy = std::max(vy, 0.0f);
                printf("Colliding: TOP ");
            
        }
        if ((collision_flags & BOTTOM) == BOTTOM) {

                tvy = std::min(vy, 0.0f);
                printf("Colliding: BOTTOM ");
            
        }

    }
    else {
        // this means there's no side collisions so just apply the corner slides
        if ((collision_flags & 0xf0) != 0) {
            if ((collision_flags & TOPLEFT) == TOPLEFT) {
                if (std::abs(vx) > std::abs(vy)) {
                    //tvx = (vx + slideX) / 2;
                    tvy = slideY;
                }
                else {
                    tvx = slideX;
                    //tvy = (vy + slideY) / 2;
                }
                printf("Colliding: TOPLEFT ");
            }
            if ((collision_flags & TOPRIGHT) == TOPRIGHT) {
                if (std::abs(vx) > std::abs(vy)) {
                    //tvx = (vx + slideX) / 2;
                    tvy = slideY;
                }
                else {
                    tvx = slideX;
                    //tvy = (vy + slideY) / 2;
                }
                printf("Colliding: TOPRIGHT ");
            }
            if ((collision_flags & BOTTOMLEFT) == BOTTOMLEFT) {
                if (std::abs(vx) > std::abs(vy)) {
                    //tvx = (vx + slideX) / 2;
                    tvy = slideY;
                }
                else {
                    tvx = slideX;
                    //tvy = (vy + slideY) / 2;
                }
                printf("Colliding: BOTTOMLEFT ");
            }
            if ((collision_flags & BOTTOMRIGHT) == BOTTOMRIGHT) {
                if (std::abs(vx) > std::abs(vy)) {
                    //tvx = (vx + slideX) / 2;
                    tvy = slideY;
                }
                else {
                    tvx = slideX;
                    //tvy = (vy + slideY) / 2;
                }
                printf("Colliding: BOTTOMRIGHT ");
            }
        }
    }
    
    //apply the adjusted velocity
    x = px + delta * (tvx + superSlideX);
    y = py + delta * (tvy + superSlideY);


    printf("\n");

    if (collision_flags == 0)
        isColliding = false;
    

    camera->setDestination((int)x, (int)y);
}

void Player::calculate_slide(float ax, float ay) {

    //float distX = get_center_x() - ax;
    //float distY = get_center_y() - ay;
    //float magnitude = sqrt(pow(distX, 2) + pow(distY, 2));
    //float dirX = distX / magnitude;
    //float dirY = distY / magnitude;
    //slideX += dirX * 10;
    //slideY += dirY * 10;

    float distX = get_center_x() - ax;
    float distY = get_center_y() - ay;
    //float magnitude = pow(distX, 2) + pow(distY, 2);
    //float dirX = distX / magnitude;
    //float dirY = distY / magnitude;

    if (distX < 30) {
        superSlideX = distX;
    }
    if (distY < 30) {
        superSlideY = distY;
    }
    slideX += distX/2;
    slideY += distY/2;

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