// The rest screen: a large 12-hour clock on a soft, cool off-white face, with
// one faint arc sweeping slowly around the rim behind it.
//
// The palette is deliberately dimmed rather than pure white - this panel has no
// backlight pin (it's tied on at the board), so brightness can only be lowered
// by drawing darker, and a full-white 240x240 face is glaring up close. Tune
// FACE_BG / SWEEP_* below to take it further either way.
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "state_clock.h"
#include "anim_helpers.h"
#include "colors.h"

LV_FONT_DECLARE(nova_font_time_80);

#define FACE_BG       0xC7CED6  // cool off-white, dimmed down from pure white
#define FACE_RING     0xAEB7C1
#define FACE_TEXT     0x2E3742  // cool near-black: high contrast without the harshness
#define SWEEP_COLOR   0x8B96A3
#define SWEEP_MS      9000      // one lap - slow enough to read as ambient, not busy

static lv_obj_t *s_screen;
static lv_obj_t *s_label;
static lv_obj_t *s_task_dot;
static lv_obj_t *s_sweep;

// ------------------------------------------------------------------- clock ---
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
        int hour12 = t.tm_hour % 12;
        if (hour12 == 0) {
            hour12 = 12;
        }
        snprintf(buf, sizeof(buf), "%02d:%02d", hour12, t.tm_min);
    }
    lv_label_set_text(s_label, buf);
}

static void build(void)
{
    s_screen = nova_disp_make_screen();
    lv_obj_set_style_bg_color(s_screen, lv_color_hex(FACE_BG), LV_PART_MAIN);

    lv_obj_t *ring = lv_obj_create(s_screen);
    lv_obj_remove_style_all(ring);
    lv_obj_set_size(ring, 232, 232);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ring, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(ring, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(ring, lv_color_hex(FACE_RING), LV_PART_MAIN);
    lv_obj_set_style_border_opa(ring, LV_OPA_50, LV_PART_MAIN);
    lv_obj_center(ring);

    // One short arc drifting round the rim: low-contrast on purpose, so the
    // time reads first and the motion is only noticed second.
    s_sweep = nova_disp_make_ring(s_screen, 214, 6);
    lv_arc_set_angles(s_sweep, 0, 70);
    lv_obj_set_style_arc_color(s_sweep, lv_color_hex(SWEEP_COLOR), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(s_sweep, LV_OPA_40, LV_PART_INDICATOR);

    lv_anim_t sweep;
    lv_anim_init(&sweep);
    lv_anim_set_var(&sweep, s_sweep);
    lv_anim_set_exec_cb(&sweep, (lv_anim_exec_xcb_t)lv_arc_set_rotation);
    lv_anim_set_values(&sweep, 0, 360);
    lv_anim_set_time(&sweep, SWEEP_MS);
    lv_anim_set_repeat_count(&sweep, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&sweep, lv_anim_path_linear);
    lv_anim_start(&sweep);

    s_label = lv_label_create(s_screen);
    lv_obj_set_style_text_font(s_label, &nova_font_time_80, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_label, lv_color_hex(FACE_TEXT), LV_PART_MAIN);
    lv_label_set_text(s_label, "--:--");
    lv_obj_align(s_label, LV_ALIGN_CENTER, 0, -22);

    s_task_dot = lv_obj_create(s_screen);
    lv_obj_remove_style_all(s_task_dot);
    lv_obj_set_size(s_task_dot, 12, 12);
    lv_obj_set_style_radius(s_task_dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_task_dot, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_align(s_task_dot, LV_ALIGN_BOTTOM_MID, 0, -24);

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
