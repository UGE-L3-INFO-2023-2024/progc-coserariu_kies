/**
 * @file events.h
 * @author Coserariu Alain & Kies Remy
 * @brief Contains all possible events the user can do
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
    NO_EVENT,
    SUMMON_WAVE,
    SUMMON_TOWER,
    SUMMON_GEM,
    PLACE_TOWER,
    CANCEL_PLACING_TOWER,
    CANCEL_PLACING_GEM,
    PICK_GEM_FROM_FIELD,
    PICK_GEM_FROM_INVENTORY,
    ADD_GEM_LEVEL,
    SUB_GEM_LEVEL,
    QUIT,
    DROP_GEM_IN_INVENTORY,
    DROP_GEM_IN_FIELD,
    CHANGE_GAME_STATUS,
    UPGRADE_MANA_POOL,
    SHOW_TOOLTIP,
    HIDE_TOOLTIP,
    SHOW_UPGRADE_COST,
    SHOW_GEM_COST,
    SHOW_TOWER_COST,
    SHOW_GEM_COST_ADD,
    SHOW_GEM_COST_SUB,
    SHOW_COMBINE_COST,
    CHOOSE_SKILL_A,
    CHOOSE_SKILL_B,
    CHOOSE_SKILL_C,
} Event;

#endif