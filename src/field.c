#include "../include/field.h"

#include <math.h>
#include <stdbool.h>

#include "../include/monster.h"
#include "../include/player.h"
#include "../include/position.h"

//-------------------------------General------------------------------- 

bool in_field(Position pos) {
    return (0 <= pos.x && pos.x < WIDTH)
            && (0 <= pos.y && pos.y < HEIGHT);
}

Objects get_field(Field field, Position pos) {
    return field.board[(int)pos.y][(int)pos.x];
}

void add_to_field(Field* field, Position pos, Objects object) {
    if (!in_field(pos)) {
        field->board[(int)(pos.y)][(int)(pos.x)] = object;
    }
}

//-------------------------------Monster related-------------------------------

Error place_tower(Field* field, Player* player, Tower tower) {
    if (!in_field(tower.pos)) {
        return OUT_OF_FIELD;
    }
    
    int cost = field->towers.next_tower_cost;
    Error err;

    if (player->mana < cost) {
        return NOT_ENOUGHT_MANA;
    }

    if (get_field(*field, tower.pos) != EMPTY) {
        return NON_EMPTY_PLACE;
    }

    err = add_tower_array(&(field->towers), tower);
    if (err != OK) {
        return err;
    }

    add_to_field(field, tower.pos, TOWER);
    player->mana -= cost;
    return OK;
}

//-------------------------------Monster related-------------------------------

Error spawn_monster_field(Field* field, int wave_nb, TypeWave type_wave) {
    Monster m = init_monster(cell_center(field->nest), type_wave, wave_nb,
                             cell_center(field->monster_path.path[0]));

    Error err = add_monster_array(&(field->monsters), m);

    return err;
}

/**
 * @brief Decrease the mana amount of the player and tp the monster to the nest
 *
 * @param monster
 * @param player
 * @param field
 */
static void ban_monster(Monster* monster, Player* player, const Field* field) {
    player->mana -=
        (monster->max_health * 15.0 / 100) * pow(1.3, player->mana_lvl);

    monster->index_path = 0;
    monster->dest = cell_center(field->monster_path.path[0]);
    monster->pos = cell_center(field->nest);
}

void update_monster_dest(Monster* monster, const Field* field, Player* player) {
    if (monster->index_path == field->monster_path.cur_len - 1) {
        // If monster step on the camp
        ban_monster(monster, player, field);
    } else {
        monster->index_path++;
        monster->dest = cell_center(field->monster_path.path[monster->index_path]);
    }
}
