#include "app/profile.h"
#include "app/gamification_service.h"
#include "app/leaderboard.h"
#include "lvgl.h"

static void create_stats_tab(lv_obj_t * parent) {
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // --- Level Display ---
    lv_obj_t * level_label = lv_label_create(parent);
    lv_label_set_text_fmt(level_label, "Level %d", gamification_get_current_level());
    lv_obj_set_style_text_font(level_label, &lv_font_montserrat_24, 0);

    // --- XP Bar ---
    int current_xp = gamification_get_current_xp();
    int next_level_xp = gamification_get_xp_for_next_level();

    lv_obj_t * xp_bar = lv_bar_create(parent);
    lv_obj_set_width(xp_bar, lv_pct(80));
    lv_bar_set_range(xp_bar, 0, next_level_xp);
    lv_bar_set_value(xp_bar, current_xp, LV_ANIM_OFF);

    lv_obj_t * xp_label = lv_label_create(parent);
    lv_label_set_text_fmt(xp_label, "%d / %d XP", current_xp, next_level_xp);
}

static void create_goals_tab(lv_obj_t * parent) {
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(100));
    lv_obj_center(list);

    int goal_count = 0;
    const goal_t *goals = gamification_get_goals(&goal_count);

    for (int i = 0; i < goal_count; i++) {
        lv_list_add_text(list, goals[i].description);
        lv_obj_t * bar = lv_bar_create(list);
        lv_obj_set_width(bar, lv_pct(95));
        lv_bar_set_range(bar, 0, goals[i].target_value);
        lv_bar_set_value(bar, goals[i].current_value, LV_ANIM_OFF);
    }
}

static void create_awards_tab(lv_obj_t * parent) {
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW); // LV_FLEX_FLOW_WRAP is not defined/supported in this LVGL config
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    int achievement_count = 0;
    const achievement_t *achievements = gamification_get_achievements(&achievement_count);

    for (int i = 0; i < achievement_count; i++) {
        lv_obj_t * cont = lv_obj_create(parent);
        lv_obj_set_size(cont, 100, 100);
        lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        
        lv_obj_t * icon = lv_label_create(cont);
        lv_label_set_text(icon, LV_SYMBOL_EDIT); // Placeholder icon, e.g., a trophy LV_SYMBOL_...
        lv_obj_set_style_text_font(icon, &lv_font_montserrat_32, 0);

        lv_obj_t * name_label = lv_label_create(cont);
        lv_label_set_text(name_label, achievements[i].name);
        
        if (!achievements[i].is_unlocked) {
            lv_obj_set_style_opa(cont, LV_OPA_30, 0);
        }
    }
}

void create_profile_app(lv_obj_t * parent) {
    lv_obj_clean(parent);

    lv_obj_t * tabview = lv_tabview_create(parent, LV_DIR_TOP, 50);

    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Profile");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Goals");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Awards");
    lv_obj_t * tab4 = lv_tabview_add_tab(tabview, "Leaderboard");

    create_stats_tab(tab1);
    create_goals_tab(tab2);
    create_awards_tab(tab3);
    create_leaderboard_app(tab4);
}
