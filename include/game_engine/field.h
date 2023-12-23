#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>

#include "utils/errors.h"
#include "game_engine/monster.h"
#include "game_engine/player.h"
#include "utils/position.h"
#include "game_engine/tower.h"
#include "game_engine/nest.h"

#define WIDTH 28
#define HEIGHT 22

#define MAX_LEN WIDTH* HEIGHT

typedef enum {
    TOWER,
    NEST,
    CAMP,
    EMPTY,
    PATH,
} Objects;

typedef struct {
    int cur_len;
    Position path[MAX_LEN];
} MonsterPath;

typedef struct {
    Position camp;
    Nest nest;
    TowerArray towers;      // list of towers
    MonsterArray monsters;  // list of monsters
    Objects board[HEIGHT][WIDTH];
    MonsterPath monster_path;
} Field;

//-------------------------------General-------------------------------

/**
 * @brief Check if a given position is inside the field dimensions 
 * 
 * @param pos position to check
 * @return true if the position is inside the field, else false
 */
bool in_field(Position pos);

/**
 * @brief Get the field object at given position
 * 
 * @param field field to get the object
 * @param pos position to check
 * @return object store at position
 */
Objects get_field(Field* field, Position pos);

/**
 * @brief Add an object to field
 * 
 * @param field
 * @param pos
 * @param object
 */
void add_to_field(Field* field, Position pos, Objects object);

//-------------------------------Tower related-------------------------------

/**
 * @brief Create a tower, remove the required mana from the 
 *        player and add it to the field if the place is empty
 * 
 * @param field 
 * @param player 
 * @param tower 
 * @return
 */
Error place_tower(Field* field, Player* player, Tower tower);

Error load_gem(Field* field, Gem gem, Position pos);

Error unload_gem(Field* field, Gem* gem, Position pos);

//-------------------------------Monster related-------------------------------

/**
 * @brief Add a monster on the field spawning on the nest
 *
 * @param field
 * @param wave_nb
 * @param type_wave
 * @return Error
 */
Error spawn_monster_field(Field* field, int wave_nb, TypeWave type_wave);

/**
 * @brief Find the next destination for a monster. If he step on the camp the
 * ban process will occurs
 *
 * @param monster
 * @param field
 * @param player
 */
void update_monster_dest(Monster* monster, const Field* field, Player* player);

/**
 * @brief Load a new wave in a nest
 *
 * @param nest
 */
void init_new_wave(Nest* nest, int nb_wave);

#endif
