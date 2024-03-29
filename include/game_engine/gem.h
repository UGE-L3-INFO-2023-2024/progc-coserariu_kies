/**
 * @file gem.h
 * @author Coserariu Alain & Kies Remy
 * @brief Contains gem structure and functions to create them
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GEM_H
#define GEM_H

typedef enum {  
    PYRO,
    DENDRO,
    HYDRO,
    MIXTE
} TypeGems;

typedef struct {
    int level;
    int color;
    int reload_time;
    TypeGems type;
} Gem;

/**
 * @brief Return the shooting range of a gem.
 * The gem increase its range when its level increase
 * 
 * @param gem 
 * @return float 
 */
float calc_radius_shoot_range(Gem* gem);

/**
 * @brief Creates a gem object.
 * 
 * @param type gem type
 * @param level gem level
 * @param color gem color
 * @return created gem
 */
Gem init_gem(TypeGems type, int level, int color);

/**
 * @brief Create a random gem
 * 
 * @param level 
 * @return
 */
Gem init_random_gem(int level);

#endif
