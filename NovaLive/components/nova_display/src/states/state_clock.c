// PLACEHOLDER LOOK - awaiting design instructions. This reproduces exactly
// what NovaLive shipped with (a bordered ring + big HH:MM), just moved into
// its own file so it can be redesigned here without touching any other
// state. Nothing else in the firmware references these statics.
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "state_clock.h"
#include "anim_helpers.h"
#include "colors.h"

static lv_obj_t *s_screen;
static lv_obj_t *s_label;
static lv_obj_t *s_task_dot;

static void tick_cb(lv_timer_t *timer)
{
    (void)timer;
    char buf[8];
    if (!nova_display_is_time_synced()) {
        strlcpy(buf, "--:--", sizeof(buf));
    } else {
        time_t now = time(NULL);
        struct tm t;
        localtime_r(&now, &t);
        snprintf(buf, sizeof(buf), t.tm_sec % 2 ? "%02d:%02d" : "%02d %02d", t.tm_hour, t.tm_min);
    }
    lv_label_set_text(s_label, buf);
}

static void build(void)
{
    s_screen = nova_disp_make_screen();

    lv_obj_t *ring = lv_obj_create(s_screen);
    lv_obj_remove_style_all(ring);
    lv_obj_set_size(ring, 224, 224);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ring, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(ring, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(ring, NOVA_COL_RING, LV_PART_MAIN);
    lv_obj_set_style_border_opa(ring, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_center(ring);

    s_label = lv_label_create(s_screen);
    lv_obj_set_style_text_font(s_label, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_label, NOVA_COL_TEXT, LV_PART_MAIN);
    lv_label_set_text(s_label, "--:--");
    lv_obj_center(s_label);

    s_task_dot = lv_obj_create(s_screen);
    lv_obj_remove_style_all(s_task_dot);
    lv_obj_set_size(s_task_dot, 14, 14);
    lv_obj_set_style_radius(s_task_dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_task_dot, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_align(s_task_dot, LV_ALIGN_BOTTOM_MID, 0, -22);

    lv_timer_create(tick_cb, 500, NULL);
}

static lv_obj_t *screen(void)
{
    return s_screen;
}

static void set_task_light(nova_display_task_light_t light)
{
    lv_anim_delete(s_task_dot, NULL);
    if (light == NOVA_DISPLAY_TASK_NONE) {
        lv_obj_set_style_opa(s_task_dot, LV_OPA_TRANSP, LV_PART_MAIN);
    } else if (light == NOVA_DISPLAY_TASK_RUNNING) {
        lv_obj_set_style_bg_color(s_task_dot, NOVA_COL_AMBER, LV_PART_MAIN);
        nova_disp_pulse_anim(s_task_dot, nova_disp_set_opa);
    } else {  // NOVA_DISPLAY_TASK_DONE
        lv_obj_set_style_bg_color(s_task_dot, NOVA_COL_GREEN, LV_PART_MAIN);
        lv_obj_set_style_opa(s_task_dot, LV_OPA_COVER, LV_PART_MAIN);
    }
}

static const nova_state_screen_t OPS = {
    .build = build,
    .enter = NULL,
    .set_label = NULL,  // no room for a transcript on the clock face
    .set_task_light = set_task_light,
    .screen = screen,
};

const nova_state_screen_t *state_clock_ops(void)
{
    return &OPS;
}
