#include "app/gamification_service.h"
#include <stdio.h> // For NULL

// --- Module-level State ---

static int current_xp = 0;
static int current_level = 1;

// --- Achievement Definitions ---
static achievement_t achievements[ACHIEVEMENT_COUNT] = {
    [ACHIEVEMENT_FIRST_1K_STEPS] = { 
        .id = ACHIEVEMENT_FIRST_1K_STEPS, 
        .name = "Step Up", 
        .description = "Take your first 1,000 steps.", 
        .is_unlocked = false 
    },
    [ACHIEVEMENT_FIRST_WORKOUT] = { 
        .id = ACHIEVEMENT_FIRST_WORKOUT, 
        .name = "Getting Started", 
        .description = "Complete your first workout.", 
        .is_unlocked = false 
    },
    [ACHIEVEMENT_FIRST_BREATHING_EXERCISE] = {
        .id = ACHIEVEMENT_FIRST_BREATHING_EXERCISE,
        .name = "Inner Peace",
        .description = "Complete your first guided breathing exercise.",
        .is_unlocked = false
    }
};

// --- Goal Definitions ---
static goal_t goals[GOAL_TYPE_COUNT] = {
    [GOAL_TYPE_DAILY_STEPS] = {
        .type = GOAL_TYPE_DAILY_STEPS,
        .description = "Walk 10,000 steps today",
        .target_value = 10000,
        .current_value = 0,
        .xp_reward = 100,
        .is_complete = false
    }
};

// --- Helper Functions ---
static int calculate_xp_for_next_level(int level) {
    return 100 + (level - 1) * 50; // Simple scaling formula
}

// --- Public API Implementation ---

void gamification_service_init() {
    // In a real app, this would load state from persistence
    current_xp = 0;
    current_level = 1;
    
    // Reset goals daily
    goals[GOAL_TYPE_DAILY_STEPS].current_value = 0;
    goals[GOAL_TYPE_DAILY_STEPS].is_complete = false;

    // Reset achievements (for demo purposes, normally persistent)
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
        // In a real app, trigger a "Level Up!" notification
    }
}

int gamification_get_current_xp() {
    return current_xp;
}

int gamification_get_xp_for_next_level() {
    return calculate_xp_for_next_level(current_level);
}

int gamification_get_current_level() {
    return current_level;
}

// --- Goals ---

void gamification_update_goal_progress(goal_type_t type, int value) {
    if (type >= GOAL_TYPE_COUNT) return;

    goal_t *goal = &goals[type];
    if (goal->is_complete) return; // Don't add progress to completed goals

    goal->current_value = value;

    if (goal->current_value >= goal->target_value) {
        goal->is_complete = true;
        gamification_add_xp(goal->xp_reward);

        // Check if this goal unlocks an achievement
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
        // In a real app, trigger an "Achievement Unlocked!" notification
    }
}

const achievement_t* gamification_get_achievements(int *count) {
    *count = ACHIEVEMENT_COUNT;
    return achievements;
}
