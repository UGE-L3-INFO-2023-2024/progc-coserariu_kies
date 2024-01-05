#include <MLV/MLV_all.h>
#include <stdbool.h>

#include "user_event/event.h"
#include "user_event/interact.h"
#include "user_event/tower_placement.h"
#include "user_event/gem_placement.h"
#include "user_event/get_event.h"
#include "game_engine/gem.h"
#include "game_engine/game.h"
#include "game_engine/player.h"
#include "game_engine/inventory.h"
#include "display/display_game.h"
#include "display/game_sectors.h"
#include "display/display_const.h"

void exit_function(void* data) {
    int* stop = (int*)data;
    *stop = 1;
}

/**
 * @brief Set interaction to select a gem from a tower.
 *        Tower coordinates on screen are (x, y)
 *        where the player clicked
 * 
 * @param game
 */
static void pick_up_gem_from_field(Game* game) {
    int x, y;
    MLV_get_mouse_position(&x, &y);
    Gem gem;
    if (unload_gem(&(game->field), &gem, init_scaled_position(x, y)) == OK) {
        set_interact_gem_movement(&(game->cur_interact), gem);
    }
}

/**
 * @brief Set interaction to select a gem from the inventory.
 *        Selected gem coordinates on screen are (x, y) 
 *        where the player clicked
 * 
 * @param game
 */
static void pick_up_gem_from_inventory(Game* game) {
    int x, y;
    Gem gem;
    MLV_get_mouse_position(&x, &y);

    int inventory_index = from_coord_to_index(&(game->sectors), x, y);

    if (!game->player.inventory.array[inventory_index].empty) {
        remove_gem_at(&(game->player.inventory), &gem, inventory_index);
        set_interact_gem_movement(&(game->cur_interact), gem);
    }
}

/**
 * @brief Drop the selected gem on the inventory square at coordinates (x, y).
 *        Store the gem if the inventory is empty, else mix.
 * 
 * @param game
 */
static void drop_gem_on_inventory(Game* game) {
    int x, y;
    MLV_get_mouse_position(&x, &y);
    Gem gem, res;
    int inventory_index = from_coord_to_index(&(game->sectors), x, y);

    if (game->player.inventory.array[inventory_index].empty) { 
        // if this inventory place is empty, juste place the gem 
        store_gem_at(&(game->player.inventory), game->cur_interact.selected_gem, inventory_index);
        reset_interaction(&(game->cur_interact));
    } else {
        // if this inventory place isn't empty, mix the selected gem with the one currently store
        remove_gem_at(&(game->player.inventory), &gem, inventory_index);
        if (combine_gem(&(game->player), game->cur_interact.selected_gem, gem, &res) != OK) {
            // if gems cannot be combined, store the stored gem in the inventory again
            store_gem_at(&(game->player.inventory), gem, inventory_index);
        } else {
            // store the result of gem mixing
            store_gem_at(&(game->player.inventory), res, inventory_index);
            reset_interaction(&(game->cur_interact));
        }
    }
}

/**
 * @brief Drop the selected gem on a tower of coordinates on screen (x, y).
 * 
 * @param game
 */
static void drop_gem_on_field(Game* game) {
    int x, y;
    MLV_get_mouse_position(&x, &y);
    if (load_gem(&(game->field), game->cur_interact.selected_gem, init_scaled_position(x, y)) == OK) {
        reset_interaction(&(game->cur_interact));
    }
}

/**
 * @brief Summon a gem an store in in the inventory
 * 
 * @param game
 */
static void summon_gem(Game* game) {
    Gem gem;
    if (is_inventory_full(&(game->player.inventory))) {
        return;
    }
    if (generate_gem(&(game->player), game->cur_interact.gem_level, &gem) != OK) {
        return;
    }
    add_inventory(&(game->player.inventory), gem);
}

static void add_gem_level(Game* game) {
    if (game->player.mana >= mana_require_for_gem(game->cur_interact.gem_level + 1)) {
        game->cur_interact.gem_level++;
    }
}

static void sub_gem_level(Game* game) {
    if (game->cur_interact.gem_level) {
        game->cur_interact.gem_level--;
    }
}

static void summon_tower(Game* game) {
    if (game->player.mana > game->field.towers.next_tower_cost) {
        set_interact_tower_placement(&(game->cur_interact), init_tower_at_mouse(game->sectors.panel));
    }
}

static void summon_wave(Game* game) {
    if (game->field.nest.monster_remaining == 0) {
        init_new_wave(&(game->field.nest), game->wave);
        game->wave++;
    }
}

static void place_in_fiend_tower(Game* game) {
    drop_tower(&(game->cur_interact), &(game->field), &(game->player));
}

static void cancel_tower_placement(Game* game) {
    cancel_interaction(&(game->cur_interact));
}

static void toggle_pause(Game* game) {
    game->game_status = !(game->game_status);
}

static void upgrade_pool(Game* game) {
    upgrade_mana_pool(&(game->player));
}

event_function func[] = {
    [SUMMON_WAVE] = summon_wave,
    [SUMMON_TOWER] = summon_tower,
    [SUMMON_GEM] = summon_gem,
    [PLACE_TOWER] = place_in_fiend_tower,
    [CANCEL_PLACING_TOWER] = cancel_tower_placement,
    [PICK_GEM_FROM_FIELD] = pick_up_gem_from_field,
    [PICK_GEM_FROM_INVENTORY] = pick_up_gem_from_inventory,
    [DROP_GEM_IN_INVENTORY] = drop_gem_on_inventory,
    [DROP_GEM_IN_FIELD] = drop_gem_on_field,
    [ADD_GEM_LEVEL] = add_gem_level,
    [SUB_GEM_LEVEL] = sub_gem_level,
    [SUMMON_GEM] = summon_gem,
    [UPGRADE_MANA_POOL] = upgrade_pool,
};

bool process_event(Game* game) {
    Event event = get_event(game->cur_interact, &(game->sectors));
    if (event == QUIT) {
        return true;
    } else if (event == CHANGE_GAME_STATUS) { // need to do this because when game is paused,
                                              // buttons except the pause one no longer works 
        toggle_pause(game);
    }  else if (game->game_status != PAUSE) {
        event_function f;
        if ((f = func[event])) {
            f(game);
        }
        
        if (game->cur_interact.current_action == PLACING_TOWER) {
            update_tower_placement(game->sectors.panel, &(game->cur_interact.selected_tower));
        } else if (game->cur_interact.current_action == MOVING_GEM) {
            update_gem_movement(&(game->cur_interact));
        }
    }
    return false;
}
