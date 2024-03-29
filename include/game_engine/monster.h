/**
 * @file monster.h
 * @author Coserariu Alain & Kies Remy
 * @brief Contains functions to create monsters, and manipulate them
 *        by moving them, wound them, and apply effects from gems and 
 *        projectiles
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MONSTER_H
#define MONSTER_H

#include <time.h>

#include "game_engine/effect.h"
#include "game_engine/score.h"
#include "utils/errors.h"
#include "utils/position.h"
#include "utils/util.h"
#include "utils/color.h"

#define MAX_WAVE_SIZE 24
#define MAX_MONSTERS 2000

typedef enum {
    NORMAL,
    CROWD,
    FAST,
    BOSS,
} TypeWave;

typedef struct {
    long long health;
    long long max_health;
    int index_path;                   // Position in the path array in a field
    Status residue;                   // Elementary redidue when hit by pure gem
    Effect status[STACKABLE_STATUS];
    color color;
    float default_speed;
    float speed;  // Number of cases per seconde traveled
    Position pos;
    Position dest;
} Monster;

typedef struct {
    int next_index_write;         // Index to write the next monster
    int array_size;               // Number of monsters in the array
    Monster array[MAX_MONSTERS];  // List of monster
} MonsterArray;

/**
 * @brief Initialize a monster
 *
 * @param pos_init Initial position on the field
 * @param type_wave
 * @param wave_number
 * @param dest
 * @return Monster
 */
Monster init_monster(Position pos_init, TypeWave type_wave, int wave_number,
                     Position dest);

/**
 * @brief Apply damages to the monster and update score
 * 
 * @param monster 
 * @param score 
 * @param damage 
 */
void take_damage(Monster* monster, Score* score, long long damage);

/**
 * @brief Move a monster in toward his destination
 *
 * @param monster
 * @param destination a position on the field
 */
void move_monster(Monster* monster);

/**
 * @brief Tell if a monster is near enough his destination
 *
 * @param monster
 * @return int
 */
int has_reach_dest(const Monster* monster);

/**
 * @brief Tell if monster health is > 0
 *
 * @param monster
 * @return int
 */
int is_alive(const Monster* monster);

/**
 * @brief Add an effect to a monster
 *
 * @param monster
 * @param effect
 */
void add_effect_monster(Monster* monster, Effect effect);

/**
 * @brief Update the effect on the monster
 *
 * @param monster
 */
void update_effect_monster(Monster* monster, Score* score);

/*---------------------------Monster array related---------------------------*/

/**
 * @brief Initialize a monster array
 *
 * @param array
 * @return Error
 */
Error init_monster_array(MonsterArray* array);

/**
 * @brief Realloc the monster array :
 *    -If the curr_size == max_size, increase the array size by
 * MAX_WAVE_SIZE
 *    -If the curr_size < max_size - MAX_WAVE_SIZE
 * reduce the array size by MAX_WAVE_SIZE
 *
 * curr_size > max_size should never happen, return INCOHERENT_TAB_DATA
 *
 * @param array
 * @return Error
 */
Error realloc_monster_array(MonsterArray* array);

/**
 * @brief Add a monster to a monster array, can realloc the array
 *
 * @param array
 * @param monster
 * @return Error
 */
Error add_monster_array(MonsterArray* array, Monster monster);

#endif
