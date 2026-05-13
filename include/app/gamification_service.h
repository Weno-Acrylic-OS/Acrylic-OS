#ifndef GAMIFICATION_SERVICE_H
#define GAMIFICATION_SERVICE_H

#include <stdbool.h>

// --- Data Structures ---

typedef enum {
    FITNESS_ZONE_DEMOTION,  // Decreased engagement
    FITNESS_ZONE_ACTIVE,      // Normal engagement
    FITNESS_ZONE_PROMOTION    // Increased engagement
} fitness_zone_t;

typedef enum {
    GOAL_TYPE_DAILY_STEPS,
    GOAL_TYPE_WEEKLY_ACTIVE_MINUTES,
    // Future goal types can be added here
    GOAL_TYPE_COUNT
} goal_type_t;

#define MAX_GOAL_DESC_LEN 64

typedef struct {
    goal_type_t type;
    char description[MAX_GOAL_DESC_LEN];
    int target_value;
    int current_value;
    int xp_reward;
    bool is_complete;
} goal_t;

typedef enum {
    ACHIEVEMENT_FIRST_1K_STEPS,
    ACHIEVEMENT_FIRST_WORKOUT,
    ACHIEVEMENT_FIRST_BREATHING_EXERCISE,
    // Future achievements
    ACHIEVEMENT_COUNT
} achievement_id_t;

typedef struct {
    achievement_id_t id;
    const char * name;
    const char * description;
    bool is_unlocked;
} achievement_t;


// --- Public API ---

void gamification_service_init();

// XP and Levels
void gamification_add_xp(int xp_to_add);
int gamification_get_current_xp();
int gamification_get_xp_for_next_level();
int gamification_get_current_level();

fitness_zone_t gamification_get_fitness_zone();
void gamification_daily_update_task();

// Goals
void gamification_update_goal_progress(goal_type_t type, int value);
const goal_t * gamification_get_goals(int *count);
void gamification_set_goals(const char *goals_json);
int gamification_get_step_count();

// Achievements
void gamification_unlock_achievement(achievement_id_t id);
const achievement_t * gamification_get_achievements(int *count);


#endif // GAMIFICATION_SERVICE_H
