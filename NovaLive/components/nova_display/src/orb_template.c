#include "orb_template.h"
#include "anim_helpers.h"
#include "colors.h"

void nova_orb_build(nova_orb_screen_t *s)
{
    s->screen = nova_disp_make_screen();

    s->glow = lv_obj_create(s->screen);
    lv_obj_remove_style_all(s->glow);
    lv_obj_set_size(s->glow, 170, 170);
    lv_obj_set_style_radius(s->glow, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s->glow, 100, LV_PART_MAIN);
    lv_obj_center(s->glow);
    nova_disp_breathe_anim(s->glow);

    s->task_ring = nova_disp_make_ring(s->screen, 232, 6);
    lv_arc_set_angles(s->task_ring, 0, 360);
    lv_obj_set_style_arc_opa(s->task_ring, LV_OPA_TRANSP, LV_PART_INDICATOR);

    s->ring = nova_disp_make_ring(s->screen, 216, 14);
    lv_arc_set_angles(s->ring, 0, 108);

    s->label = lv_label_create(s->screen);
    lv_obj_set_style_text_font(s->label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(s->label, NOVA_COL_TEXT, LV_PART_MAIN);
    lv_obj_center(s->label);
}

void nova_orb_enter(nova_orb_screen_t *s, const nova_orb_look_t *look)
{
    lv_arc_set_angles(s->ring, 0, look->arc_deg);
    lv_obj_set_style_arc_color(s->ring, look->color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(s->glow, look->color, LV_PART_MAIN);
    lv_arc_set_rotation(s->ring, 0);

    lv_anim_delete(s->ring, NULL);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, s->ring);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_arc_set_rotation);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_set_time(&a, look->period_ms);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);
}

void nova_orb_set_label(nova_orb_screen_t *s, const char *text)
{
    lv_label_set_text(s->label, text);
}

void nova_orb_set_task_light(nova_orb_screen_t *s, nova_display_task_light_t light)
{
    lv_anim_delete(s->task_ring, NULL);
    if (light == NOVA_DISPLAY_TASK_NONE) {
        lv_obj_set_style_arc_opa(s->task_ring, LV_OPA_TRANSP, LV_PART_INDICATOR);
    } else if (light == NOVA_DISPLAY_TASK_RUNNING) {
        lv_obj_set_style_arc_color(s->task_ring, NOVA_COL_AMBER, LV_PART_INDICATOR);
        nova_disp_pulse_anim(s->task_ring, nova_disp_set_arc_opa);
    } else {  // NOVA_DISPLAY_TASK_DONE
        lv_obj_set_style_arc_color(s->task_ring, NOVA_COL_GREEN, LV_PART_INDICATOR);
        lv_obj_set_style_arc_opa(s->task_ring, LV_OPA_COVER, LV_PART_INDICATOR);
    }
}
