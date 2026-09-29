#include "anim_helpers.h"
#include "colors.h"

void nova_disp_set_opa(void *obj, int32_t v)
{
    lv_obj_set_style_opa(obj, (lv_opa_t)v, LV_PART_MAIN);
}

void nova_disp_set_arc_opa(void *obj, int32_t v)
{
    lv_obj_set_style_arc_opa(obj, (lv_opa_t)v, LV_PART_INDICATOR);
}

void nova_disp_breathe_anim(lv_obj_t *obj)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, nova_disp_set_opa);
    lv_anim_set_values(&a, 70, 160);
    lv_anim_set_time(&a, 2200);
    lv_anim_set_playback_time(&a, 2200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void nova_disp_pulse_anim(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb)
{
    lv_anim_delete(obj, NULL);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, exec_cb);
    lv_anim_set_values(&a, 40, 255);
    lv_anim_set_time(&a, 800);
    lv_anim_set_playback_time(&a, 800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

lv_obj_t *nova_disp_make_screen(void)
{
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, NOVA_COL_BG, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_scrollable(scr, false);
    return scr;
}

lv_obj_t *nova_disp_make_ring(lv_obj_t *parent, int size, int width)
{
    lv_obj_t *arc = lv_arc_create(parent);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_clickable(arc, false);
    lv_obj_set_size(arc, size, size);
    lv_obj_set_style_arc_opa(arc, LV_OPA_TRANSP, LV_PART_MAIN);  // hide the background track
    lv_obj_set_style_arc_width(arc, width, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, true, LV_PART_INDICATOR);
    lv_obj_center(arc);
    return arc;
}
