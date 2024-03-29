/**
 * @file position.h
 * @author Coserariu Alain & Kies Remy
 * @brief Contains position structure with float
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Position;

/**
 * @brief Create a position object
 * 
 * @param x abscissa
 * @param y ordinate
 * @return new position
 */
Position init_position(float x, float y);

/**
 * @brief Create a position object and scale its coordinates down
 *        according to CELL_SIZE declared in 'display/display_const.h'
 * 
 * @param x 
 * @param y 
 * @return
 */
Position init_scaled_position(float x, float y);

/**
 * @brief Calculate the direction in radians between two point a and b
 * 
 * @param a 
 * @param b 
 * @return float 
 */
float calc_direction(Position a, Position b);

/**
 * @brief Calculate the distance between two point
 * 
 * @param a 
 * @param b 
 * @return float 
 */
float calc_distance(Position a, Position b);

/**
 * @brief Find the center of a cell
 * 
 * @param cell 
 * @return Position
 */
Position cell_center(Position cell);

/**
 * @brief Check if two positions are equals 
 * 
 * @param a 
 * @param b 
 * @return
 */
bool compare_pos(Position a, Position b);

#endif
