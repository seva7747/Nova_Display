#pragma once
#include <stdint.h>
#include "lvgl.h"
#include "nova_display_types.h"

// Ring rotation period, indicator arc span (degrees), and a colour - one of
// these per orb-like state file. Approximates NovaOrb.tsx's conic-gradient
// chase ring (arcWidth is that component's "% of the circle lit up").
typedef struct {
    uint32_t   period_ms;
    int16_t    arc_deg;
    lv_color_t color;
} nova_orb_look_t;

// The objects one orb-like state screen is built from. Every state file
// that uses this template keeps its own instance - nothing here is shared
// across states, so redesigning one doesn't touch another.
typedef struct {
    lv_obj_t *screen;
    lv_obj_t *glow;
    lv_obj_t *ring;
    lv_obj_t *task_ring;
    lv_obj_t *label;
} nova_orb_screen_t;

// Placeholder look, shared by every orb state until each gets its own
// design: a soft glow + rotating gradient ring + label, styled after the
// web app's NovaOrb.tsx. Replace calls to these with bespoke LVGL code in a
// state's own file whenever its design is ready - nothing else depends on
// this template staying in use.
void nova_orb_build(nova_orb_screen_t *s);
void nova_orb_enter(nova_orb_screen_t *s, const nova_orb_look_t *look);
void nova_orb_set_label(nova_orb_screen_t *s, const char *text);
void nova_orb_set_task_light(nova_orb_screen_t *s, nova_display_task_light_t light);
