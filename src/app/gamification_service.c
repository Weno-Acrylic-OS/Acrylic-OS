#include "app/gamification_service.h"
#include "app/persistence.h"
#include "vendor/cjson/cJSON.h"
#include <stdio.h>
#include <string.h>

// --- Module-level State ---

static int current_xp = 0;
static int current_level = 1;

// --- Achievement Definitions ---
static achievement_t achievements[ACHIEVEMENT_COUNT] = {
    [ACHIEVEMENT_FIRST_1K_STEPS] = { .id = ACHIEVEMENT_FIRST_1K_STEPS, .name = "Step Up", .description = "Take your first 1,000 steps.", .is_unlocked = false },
    [ACHIEVEMENT_FIRST_WORKOUT] = { .id = ACHIEVEMENT_FIRST_WORKOUT, .name = "Getting Started", .description = "Complete your first workout.", .is_unlocked = false },
    [ACHIEVEMENT_FIRST_BREATHING_EXERCISE] = { .id = ACHIEVEMENT_FIRST_BREATHING_EXERCISE, .name = "Inner Peace", .description = "Complete your first guided breathing exercise.", .is_unlocked = false }
};

// --- Goal Definitions ---
static goal_t goals[GOAL_TYPE_COUNT] = {
    [GOAL_TYPE_DAILY_STEPS] = { .type = GOAL_TYPE_DAILY_STEPS, .description = "Walk 10,000 steps today", .target_value = 10000, .current_value = 0, .xp_reward = 100, .is_complete = false },
    [GOAL_TYPE_WEEKLY_ACTIVE_MINUTES] = { .type = GOAL_TYPE_WEEKLY_ACTIVE_MINUTES, .description = "Get 150 active minutes this week", .target_value = 150, .current_value = 0, .xp_reward = 250, .is_complete = false }
};

// --- Helper Functions ---
static int calculate_xp_for_next_level(int level) {
    return 100 + (level - 1) * 50; // Simple scaling formula
}

static void save_goals() {
    persistence_write("goals", goals, sizeof(goals));
}

static void load_goals() {
    ssize_t res = persistence_read("goals", goals, sizeof(goals));
    if (res <= 0) {
        // Not found or error, initialize with default goals
        // (The default is already in the `goals` array, so we just save it)
        save_goals();
    }
}

// --- Public API Implementation ---

void gamification_service_init() {
    load_goals();
    // In a real app, xp, level and achievements would also be persisted
    current_xp = 0;
    current_level = 1;
    for(int i = 0; i < ACHIEVEMENT_COUNT; i++) {
        achievements[i].is_unlocked = false;
    }
}

// --- XP and Levels ---
void gamification_add_xp(int xp_to_add) {
    current_xp += xp_to_add;
    int xp_needed = calculate_xp_for_next_level(current_level);
    while (current_xp >= xp_needed) {
        current_level++;
        current_xp -= xp_needed;
        xp_needed = calculate_xp_for_next_level(current_level);
    }
}

int gamification_get_current_xp() { return current_xp; }
int gamification_get_xp_for_next_level() { return calculate_xp_for_next_level(current_level); }
int gamification_get_current_level() { return current_level; }

// --- Goals ---

void gamification_set_goals(const char *goals_json) {
    cJSON *root = cJSON_Parse(goals_json);
    if (!root || !cJSON_IsArray(root)) {
        if (root) cJSON_Delete(root);
        return;
    }

    int array_size = cJSON_GetArraySize(root);
    for (int i = 0; i < array_size && i < GOAL_TYPE_COUNT; i++) {
        cJSON *goal_item = cJSON_GetArrayItem(root, i);
        if (!cJSON_IsObject(goal_item)) continue;

        cJSON *type = cJSON_GetObjectItem(goal_item, "type");
        cJSON *description = cJSON_GetObjectItem(goal_item, "description");
        cJSON *target_value = cJSON_GetObjectItem(goal_item, "target_value");

        if (cJSON_IsNumber(type) && cJSON_IsString(description) && cJSON_IsNumber(target_value)) {
            int goal_type = type->valueint;
            if (goal_type >= 0 && goal_type < GOAL_TYPE_COUNT) {
                strncpy(goals[goal_type].description, description->valuestring, MAX_GOAL_DESC_LEN - 1);
                goals[goal_type].target_value = target_value->valueint;
            }
        }
    }
    cJSON_Delete(root);
    save_goals();
}

void gamification_update_goal_progress(goal_type_t type, int value) {
    if (type >= GOAL_TYPE_COUNT) return;
    goal_t *goal = &goals[type];
    if (goal->is_complete) return;
    goal->current_value = value;
    if (goal->current_value >= goal->target_value) {
        goal->is_complete = true;
        gamification_add_xp(goal->xp_reward);
        if (goal->type == GOAL_TYPE_DAILY_STEPS && goal->target_value >= 1000) {
            gamification_unlock_achievement(ACHIEVEMENT_FIRST_1K_STEPS);
        }
    }
}

const goal_t* gamification_get_goals(int *count) {
    *count = GOAL_TYPE_COUNT;
    return goals;
}

int gamification_get_step_count() {
    return goals[GOAL_TYPE_DAILY_STEPS].current_value;
}

// --- Achievements ---

void gamification_unlock_achievement(achievement_id_t id) {
    if (id >= ACHIEVEMENT_COUNT) return;
    if (!achievements[id].is_unlocked) {
        achievements[id].is_unlocked = true;
    }
}

const achievement_t* gamification_get_achievements(int *count) {
    *count = ACHIEVEMENT_COUNT;
    return achievements;
}
