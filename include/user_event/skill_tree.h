#ifndef SKILL_TREE_H
#define SKILL_TREE_H

#include <stdbool.h>

#include "utils/sector.h"

#define WAVE_OFFSET 1
#define NB_SKILLS 4
#define SKILLS_PROPOSAL 3


typedef enum {
    GIVE_MANA,
    FREE_TOWERS,
    KILL_MONSTER,
    FREE_UPGRADE,
} Skill;

typedef struct {
    Skill skills[SKILLS_PROPOSAL];
    int give[SKILLS_PROPOSAL];
    Sector sectors[SKILLS_PROPOSAL];
    bool has_sectors;
} SkillTree;

SkillTree init_skill_tree(int wave);

#endif
