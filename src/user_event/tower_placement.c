#include <MLV/MLV_all.h>

#include "utils/position.h"
#include "game_engine/tower.h"

Tower init_tower_at_mouse(int width) {
    int x, y;
    MLV_get_mouse_position(&x, &y);

    return init_tower(init_position(x / width, y / width));
}

void update_tower_placement(Tower* tower, int width) {
    int x, y;
    MLV_get_mouse_position(&x, &y);

    tower->pos = init_position(x / width, y / width);
}
