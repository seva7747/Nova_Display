#pragma once
#include "lvgl.h"

// exec callbacks for lv_anim_t - opacity of a whole object, or of just its
// arc indicator (for the ring widgets).
void nova_disp_set_opa(void *obj, int32_t v);
void nova_disp_set_arc_opa(void *obj, int32_t v);

// slow in/out opacity breathe, forever (used for the glow behind a ring).
void nova_disp_breathe_anim(lv_obj_t *obj);
// faster pulse between near-transparent and full opacity, forever (used
// for the background-task indicator, amber while running).
void nova_disp_pulse_anim(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb);

// a blank, non-scrolling, black full-screen object - the base every state
// screen starts from.
lv_obj_t *nova_disp_make_screen(void);
// an arc with no knob, background track hidden, sized to sit concentric on
// the round face - the shape both the task-light ring and the state ring
// are built from.
lv_obj_t *nova_disp_make_ring(lv_obj_t *parent, int size, int width);
