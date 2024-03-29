
#include <math.h>
#include <stdbool.h>

#include "game_engine/field.h"
#include "game_engine/game.h"
#include "game_engine/monster.h"
#include "game_engine/player.h"
#include "game_engine/field.h"
#include "game_engine/tower.h"
#include "game_engine/game.h"
#include "utils/position.h"
#include "utils/util.h"

//-------------------------------General-------------------------------

bool in_field(Position pos) {
    return (0 <= pos.x && pos.x < WIDTH) && (0 <= pos.y && pos.y < HEIGHT);
}

Objects get_field(Field* field, Position pos) {
    return field->board[(int)pos.y][(int)pos.x];
}

void add_to_field(Field* field, Position pos, Objects object) {
    if (in_field(pos)) {
        field->board[(int)(pos.y)][(int)(pos.x)] = object;
    }
}

//--------------------------------Tower related---------------------------------

Error place_tower(Field* field, Player* player, Tower tower) {
    if (!in_field(tower.pos)) {
        return OUT_OF_FIELD;
    }
    
    int cost = get_tower_cost(&(field->towers));
    Error err;

    if (player->mana < cost) {
        return NOT_ENOUGHT_MANA;
    }

    if (get_field(field, tower.pos) != EMPTY) {
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

Error load_gem(Field* field, Gem gem, Position pos) {
    Tower* tower;

    if (!in_field(pos)) {
        return OUT_OF_FIELD;
    }

    if (get_tower(field, &tower, pos) != OK) {
        return NO_TOWER_FOUND;
    }

    if (add_gem_to_tower(tower, gem) != OK) {
        return NON_EMPTY_TOWER;
    }
    return OK;
}

Error unload_gem(Field* field, Gem* gem, Position pos) {
    Tower* tower;

    if (!in_field(pos)) {
        return OUT_OF_FIELD;
    }

    if (get_tower(field, &tower, pos) != OK) {
        return NO_TOWER_FOUND;
    }

    if (remove_gem_from_tower(tower, gem) != OK) {
        return NON_EMPTY_TOWER;
    }
    return OK;
}

Error get_tower(Field* field, Tower** tower, Position pos) {
    if (!in_field(pos)) {
        return OUT_OF_FIELD;
    }
    
    for (int i = 0; i < field->towers.cur_len; i++) {
        if (compare_pos(field->towers.lst[i].pos, pos)) {
            *tower = &(field->towers.lst[i]);
            return OK;
        }
    }
    return NO_TOWER_FOUND;
}

/*-------------------------------Monster related------------------------------*/

Error spawn_monster_field(Field* field, int wave_nb, TypeWave type_wave) {
    Monster m = init_monster(cell_center(field->nest.pos), type_wave, wave_nb,
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
    long long mana_loss =
        (monster->max_health * 0.15) * pow(1.3, player->mana_lvl);
    player->mana = max(0, player->mana - mana_loss);

    monster->index_path = 0;
    monster->dest = cell_center(field->monster_path.path[0]);
    monster->pos = cell_center(field->nest.pos);
}

void update_monster_dest(Monster* monster, const Field* field, Player* player) {
    if (monster->index_path == field->monster_path.cur_len - 1) {
        // If monster step on the camp
        ban_monster(monster, player, field);
    } else {
        monster->index_path++;
        monster->dest =
            cell_center(field->monster_path.path[monster->index_path]);
    }
}
