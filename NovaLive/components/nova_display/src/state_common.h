#pragma once
#include <stdbool.h>
#include "lvgl.h"
#include "nova_display_types.h"

// One of these per file under states/ - each state owns its own screen
// object(s) and is free to become a completely different widget tree later.
// set_label and set_task_light may be NULL if that state's screen has
// nothing for them to update (e.g. the clock face has no label to speak).
typedef struct {
    void (*build)(void);                                      // create the screen, once, at init
    void (*enter)(void);                                      // called every time this state becomes active
    void (*set_label)(const char *text);
    void (*set_task_light)(nova_display_task_light_t light);
    lv_obj_t *(*screen)(void);
} nova_state_screen_t;

// Implemented in display_core.c; lets state_clock.c know whether SNTP has
// landed a fix yet without reaching into another state file's statics.
bool nova_display_is_time_synced(void);
