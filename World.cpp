#include <SDL.h>
#include "World.h"
#include "Player.h"
#include "Rock.h"
#include "Enemy.h"
#include <vector>
#include <ctime>
#include "AStar.hpp"
#include <iostream>


#define MAP_SIZE_WIDTH 100 //screen_w/32 + 10
#define MAP_SIZE_HEIGHT 100 //screen_h/32 + 10

// STATIC MEMBERS

const float World::RESPAWN_DELAY = 3.0f;
const float World::ENEMY_SPAWN_DELAY = 1.0f;
const float World::ENEMY_PATHFIND_DELAY = 1.0f;

// PRIVATE HELPER FUNCTIONS

void World::check_spawn_players() {

    bool players_alive[NUM_PLAYERS] = { false };
    for (int i = 0; i < players.size(); i++) {
        players_alive[((Player*)players[i])->get_player_num() - 1] = true;
    }
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (!players_alive[i] && player_respawn_timers[i] == 0) {

            // Create random spawn location
            int spawn_x = ((float)rand()) / RAND_MAX * (screen_w - 64);
            int spawn_y = ((float)rand()) / RAND_MAX * (screen_h - 64);

            Player* p = new Player(spawn_x, spawn_y, i + 1, screen_w, screen_h, &actors, camera);
            actors.push_back(p);
            players.push_back(p);
        }
    }

}

void World::spawn_enemies() {


    // Get spawn location
    std::tuple<int, int> t = map_generator->get_random_spawn_loc();
    int spawn_x = std::get<0>(t) * 32;
    int spawn_y = std::get<1>(t) * 32;
    
    Enemy* e = new Enemy(spawn_x, spawn_y, 32, 32, screen_w, screen_h, dynamic_cast<Player*>(players[0]), enemy_weapons);
    actors.push_back(e);

}

void World::spawn_tile_item(Tile* t) {


    // Get spawn location
    int spawn_x = (t->x * 32);
    int spawn_y = (t->y * 32);

    actors.push_back(new TileItem(t->type, spawn_x, spawn_y, 16, 16));

    t->type = TileType::EMPTY;

}


void World::find_path() {

    std::cout << "Generate path ... \n";
    // This method returns vector of coordinates from target to source.
    auto path = path_generator.findPath({ 0, 0 }, { 20, 20 }, get_map());

    for (auto& coordinate : path) {
        std::cout << coordinate.x << " " << coordinate.y << "\n";
    }
}

// PUBLIC FUNCTIONS

World::World(int screen_w, int screen_h) : player_respawn_timers() {

    // Set seed
    seed = 5832;

    // Set variables
    this->screen_w = screen_w;
    this->screen_h = screen_h;
    enemy_spawn_timer = ENEMY_SPAWN_DELAY;

    if (seed == 0) {
        this->seed = std::time(0);
    }
    else {
        this->seed = seed;
    }
    std::srand(seed);

    collision_manager = new CollisionManager();
    camera = new Camera(screen_w, screen_h);
    map_generator = new MapGenerator(MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, 50);
    generate_map();

    //actors.push_back(new Player(screen_w / 6 + 32, screen_h / 2 - 32, 1, screen_w, screen_h, &actors, camera));
    SDL_Point* spawn_point = map_generator->get_random_empty_cell();
    Player* p = new Player(spawn_point->x * 32, spawn_point->y * 32, 1, screen_w, screen_h, &actors, camera);
    //actors.push_back(p);
    players.push_back(p);


    //actors.push_back(new Player(screen_w * 5 / 6 - 32, screen_h / 2 - 32, 2, screen_w, screen_h, &actors));

    delete spawn_point;

    // Set up AI pathfinding
    // Set 2d map size.
    path_generator.setWorldSize({ MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT });
    // You can use a few heuristics : manhattan, euclidean or octagonal.
    path_generator.setHeuristic(AStar::Heuristic::euclidean);
    path_generator.setDiagonalMovement(false);


}

// Main cycle running game logic (inputs, physics, mechanics, etc.)
void World::update(InputHandler* inputs) {

    // Respawn players if they die
    check_spawn_players();

    // Reset delta since last cycle
    clock.tick();

    // Handle input
    // Deliver input to each Player
    for (GameActor* player : players) {
        ((Player*)player)->handle_inputs(clock.get_delta(), inputs);
    }

    //Update players
    for (GameActor* player : players) {
        ((Player*)player)->update(clock.get_delta());
    }
    // Update entities
    for (int i = 0; i < actors.size(); i++) {
        actors[i]->update(clock.get_delta());
    }

    // Run AI Pathfinding
    //find_path();

    // Check for collisions between players and enemies
    collision_manager->check_collisions(players, actors);
    // Check for player / enemy weapon collisions
    collision_manager->check_collisions(players, enemy_weapons);
    // Check for player weapon collision
    collision_manager->check_actor_collisions(&actors); // this checks all actors if they collide or not, TODO: break out the player's weapons and have them check against enemies
    // Check for tile map collisions
    collision_manager->check_tile_collisions(map_generator, &actors);
    collision_manager->check_tile_collisions(map_generator, &players);

    camera->update();

    // Resolve actor collisions
    for (GameActor* p : players) {
        p->resolve_collisions();
    }
    for (int i = 0; i < actors.size(); i++) {
        actors[i]->resolve_collisions();
    }

    // Prune dead entities from entites vector
    for (int i = 0; i < actors.size(); i++) {
        if (!actors[i]->is_alive()) {
            if (actors[i]->get_id() == GameActorType::PLAYER) {
                player_respawn_timers[
                    ((Player*)actors[i])->get_player_num() - 1] = RESPAWN_DELAY;
            }
            delete actors[i];
            actors[i] = NULL;
            actors.erase(actors.begin() + i);
            i -= 1;
        }
    }

    // Prune dead entities from enemy weapon vector
    for (int i = 0; i < enemy_weapons.size(); i++) {
        if (!enemy_weapons[i]->is_alive()) {
            delete enemy_weapons[i];
            enemy_weapons[i] = NULL;
            enemy_weapons.erase(enemy_weapons.begin() + i);
            i -= 1;
        }
    }

    // Spawn enemies if spawn delay is up
    if (enemy_spawn_timer >= 0) {
        enemy_spawn_timer -= clock.get_delta();
        if (enemy_spawn_timer < 0) {
            spawn_enemies();
            enemy_spawn_timer = ENEMY_SPAWN_DELAY;
        }
    }

    // Spawn items from destroyed tiles
    auto destroyed_tiles = map_generator->get_destroyed_tiles_onscreen(camera);
    for (auto t : *destroyed_tiles) {
        spawn_tile_item(t);
    }
    delete destroyed_tiles;

}

float World::get_delta() {
    return clock.get_delta();
}

std::vector<GameActor*>* World::get_actors() {
    return &actors;
}

std::vector<GameActor*>* World::get_players()
{
    return &players;
}

Camera* World::get_camera() {
    return camera;
}

bool World::generate_map() {
    bool success = true;

    map_generator->generate_map();

    //OLD METHOD WHERE ALL ROCK TILES ARE ACTORS
    //std::vector<std::vector<int>>* map = map_generator->get_map();
    
    //for (int i = 0; i < map->size(); ++i) {
    //    for (int j = 0; j < (*map)[0].size(); ++j) {
    //        //create a rock
    //        if((*map)[i][j] == 1)
    //            actors.push_back(new Rock(i * 32.0f, j * 32.0f, screen_w, screen_h));
    //    }
    //}

    return success;
}

MapGenerator* World::get_map()
{
    return map_generator;
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

    // Free Camera
    delete camera;
    camera = NULL;

    // Free map generator
    delete map_generator;
    map_generator = NULL;
}