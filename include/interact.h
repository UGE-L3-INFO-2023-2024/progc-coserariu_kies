#ifndef INTERACT_H
#define INTERACT_H

#include "tower.h"
#include "gem.h"
#include "field.h"
#include "player.h"

typedef enum {
    NO_ACTION,
    PLACING_TOWER,
    MOVING_GEM
} Action;

typedef struct {
    Action current_action;
    union {
        Tower selected_tower;
        Gem selected_gem;
    };
} Interaction;

/**
 * @brief Init interactoin object with no current action
 * 
 * @return
 */
Interaction init_interact(void);

/**
 * @brief Set the current interaction to `PLACING_TOWER` and set the tower
 * 
 * @param interact 
 * @param tower 
 * @return
 */
Error set_interact_tower_placement(Interaction* interact, Tower tower);

Error drop_tower(Interaction* interact, Field* field, Player* player);

#endif
