// PLACEHOLDER LOOK - awaiting design instructions. Currently just the
// shared orb template (see orb_template.h) with this state's own speed/arc/
// colour, exactly matching what NovaLive shipped with. Redesign this state
// by replacing the nova_orb_* calls below with bespoke LVGL code - nothing
// else in the firmware depends on this file staying on the template.
#include "state_recording.h"
#include "orb_template.h"

static nova_orb_screen_t s_orb;

// #5eead4 - approximates NovaOrb.tsx's "recording" look
static const nova_orb_look_t LOOK = {
    .period_ms = 2100,
    .arc_deg   = 115,
    .color     = {.blue = 0xD4, .green = 0xEA, .red = 0x5E},
};

static void build(void)      { nova_orb_build(&s_orb); }
static void enter(void)      { nova_orb_enter(&s_orb, &LOOK); }
static void set_label(const char *text) { nova_orb_set_label(&s_orb, text); }
static void set_task_light(nova_display_task_light_t light) { nova_orb_set_task_light(&s_orb, light); }
static lv_obj_t *screen(void) { return s_orb.screen; }

static const nova_state_screen_t OPS = {
    .build = build,
    .enter = enter,
    .set_label = set_label,
    .set_task_light = set_task_light,
    .screen = screen,
};

const nova_state_screen_t *state_recording_ops(void)
{
    return &OPS;
}
