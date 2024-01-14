#include "user_event/skill_tree.h"

#include <stdbool.h>

#include "utils/util.h"

/**
 * @brief Give the amount of mana to be given at a certain wave
 * 
 * @param wave 
 * @return
 */
static inline int give_mana_value(int wave) {
    return wave*10;
}

/**
 * @brief Give the number of monsters to kill at a certain wave
 * 
 * @param wave 
 * @return
 */
static inline int give_monster_kill_value(int wave) {
    return max(1, wave / 6);
}

/**
 * @brief Give the number of free towers to give at a certain wave
 * 
 * @param wave 
 * @return
 */
static inline int give_free_towers_value(int wave) {
    return max(1, wave / 5);
}

/**
 * @brief Do nothing
 * 
 * @param wave 
 * @return
 */
static inline int give_upgrade(int wave) {
    return 0;
}

/**
 * @brief Give the level of the pure gem to be created at a certain wave
 * 
 * @param wave 
 * @return
 */
static inline int give_gem_level(int wave) {
    return max(1, wave / 3);
}

typedef int(*v_func)(int);

static v_func value_function[] = {
    [GIVE_MANA] = give_mana_value,
    [FREE_TOWERS] = give_free_towers_value,
    [KILL_MONSTER] = give_monster_kill_value,
    [FREE_UPGRADE] = give_upgrade,
    [GIVE_GEM] = give_gem_level
}; 

/**
 * @brief Find a skill to replace the one at replace_index
 * 
 * @param tree 
 * @param replace_index 
 * @return
 */
static Skill new_random_skill(SkillTree* tree, int replace_index) {
    bool valid = false;
    Skill skill;
    while (!valid) {
        valid = true;
        skill = random_int(0, NB_SKILLS - 1);
        for (int i = 0; i < NB_SKILLS; i++) {
            if (i != replace_index) {
                if (skill == tree->skills[i]) {
                    valid = false;
                }
            }
        }
    }
    return skill;
}

/**
 * @brief Fill skills on a new SkillTree structure
 * 
 * @param tree 
 */
static void fill_skills(SkillTree* tree) {
    int index = 0;
    Skill skill;
    while (index != SKILLS_PROPOSAL) {
        skill = random_int(0, NB_SKILLS - 1);
        int i;
        for (i = 0; i < index; i++) {
            if (tree->skills[i] == skill) {
                break;
            }
        }
        if (i == index) {
            tree->skills[i] = skill;
            index++;
        }
    }
}

/**
 * @brief Fill initials values for each skills
 * 
 * @param tree 
 * @param wave 
 */
static void fill_values(SkillTree* tree, int wave) {
    for (int i = 0; i < SKILLS_PROPOSAL; i++) {
       tree->give[i] = value_function[tree->skills[i]](wave);
    }
}

SkillTree init_skill_tree(int wave) {
    SkillTree tree;
    fill_skills(&tree);
    fill_values(&tree, wave);
    tree.has_sectors = false;
    return tree;
}

void replace_skill(SkillTree* tree, int index, int wave) {
    tree->skills[index] = new_random_skill(tree, index);
    tree->give[index] = value_function[tree->skills[index]](wave);
}
