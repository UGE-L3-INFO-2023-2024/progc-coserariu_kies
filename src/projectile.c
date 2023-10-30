#include <stdlib.h>

#include "../include/projectile.h"
#include "../include/position.h"

Projectile create_projectile(PositionFloat pos, Monster* target, Gem source) {
    Projectile proj;
    proj.pos = pos;
    proj.target = target;
    proj.source = source;
    proj.speed = 3;         // default speed at 3 tiles per seconds
    return proj;
}

Node* create_node(void) {
    return (Node*)malloc(sizeof(Node));
}

Node* create_filled_node(Projectile proj) {
    Node* node = create_node();
    if (node) {
        node->proj = proj;
        node->next = NULL;
    }
    return node;
}

ProjectileArray create_array(void) {
    return NULL;
}

int add_node(ProjectileArray* array, Node* node) {
    if (!node) return 0;

    node->next = *array;
    *array = node;

    return 1;
}

int add_projectile(ProjectileArray* array, Projectile proj) {
    return add_node(array, create_filled_node(proj));
}

Node* pop_node(ProjectileArray* array) {
    if (!(*array)) return NULL;

    Node* node = *array;
    *array = (*array)->next;

    return node;
}

void free_array(ProjectileArray* array) {
    Node* temp;
    while (*array) {
        temp = (*array)->next;
        free(*array);
        *array = temp;
    }
}

// Test allocation and free
// int main(void) {
//     ProjectileArray array = create_array();
//     add_projectile(&array, create_projectile((Position){}, NULL, (Gem){}));
//     add_projectile(&array, create_projectile((Position){}, NULL, (Gem){}));
//     add_projectile(&array, create_projectile((Position){}, NULL, (Gem){}));
//     free_array(&array);
// }
