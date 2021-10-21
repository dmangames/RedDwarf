#include <SDL.h>
#include "world.h"
#include "Player.h"
#include "Rock.h"

// STATIC MEMBERS

const float World::respawn_delay = 3.0f;

// PRIVATE HELPER FUNCTIONS

void World::check_spawn_players() {

    bool players_alive[NUM_PLAYERS] = { false };
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->get_id() == GameActorType::PLAYER) {
            players_alive[((Player*)actors[i])->get_player_num() - 1] = true;
        }
    }
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (!players_alive[i] && player_respawn_timers[i] == 0) {

            // Create random spawn location
            int spawn_x = ((float)rand()) / RAND_MAX * (screen_w - 64);
            int spawn_y = ((float)rand()) / RAND_MAX * (screen_h - 64);

            actors.push_back(new Player(spawn_x, spawn_y, i + 1, screen_w, screen_h, &actors));
        }
    }

}

// PUBLIC FUNCTIONS

World::World(int screen_w, int screen_h) : player_respawn_timers() {

    this->screen_w = screen_w;
    this->screen_h = screen_h;
    collision_manager = new CollisionManager(&actors);
    actors.push_back(new Player(screen_w / 6 + 32, screen_h / 2 - 32, 1, screen_w, screen_h, &actors));
    //actors.push_back(new Player(screen_w * 5 / 6 - 32, screen_h / 2 - 32, 2, screen_w, screen_h, &actors));
    generate_map(2, 2);
}

// Main cycle running game logic (inputs, physics, mechanics, etc.)
void World::update(InputHandler* inputs) {

    // Respawn players if they die
    check_spawn_players();

    // Reset delta since last cycle
    clock.tick();

    // Handle input
    // Deliver input to each Player
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->get_id() == GameActorType::PLAYER) {
            ((Player*)actors[i])->handle_inputs(clock.get_delta(), inputs);
        }
    }

    // Update entities
    for (int i = 0; i < actors.size(); i++) {
        actors[i]->update(clock.get_delta());
    }

    // Check for collisions
    collision_manager->check_collisions();

    // Prune dead entities from entites vector
    for (int i = 0; i < actors.size(); i++) {
        if (!actors[i]->is_alive()) {
            if (actors[i]->get_id() == GameActorType::PLAYER) {
                player_respawn_timers[
                    ((Player*)actors[i])->get_player_num() - 1] = respawn_delay;
            }
            delete actors[i];
            actors[i] = NULL;
            actors.erase(actors.begin() + i);
            i -= 1;
        }
    }

    // Update player respawn timers
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (player_respawn_timers[i] > 0) {
            player_respawn_timers[i] -= clock.get_delta();
            if (player_respawn_timers[i] < 0) {
                player_respawn_timers[i] = 0;
            }
        }
    }

}

float World::get_delta() {
    return clock.get_delta();
}

std::vector<GameActor*>* World::get_actors() {
    return &actors;
}

bool World::generate_map(int w, int h) {
    bool success = true;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            //create a rock
            actors.push_back(new Rock(i * 32.0f, j * 32.0f, screen_w, screen_h));
        }
    }

    return success;
}

World::~World() {

    // Free all entities
    for (int i = 0; i < actors.size(); i++) {
        delete actors[i];
        actors[i] = NULL;
    }

    // Free collision manager
    delete collision_manager;
    collision_manager = NULL;

}