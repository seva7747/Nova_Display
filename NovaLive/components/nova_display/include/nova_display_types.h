#pragma once

// Mirrors main.c's nova_state_t, but only as much as the screen needs to
// pick its look: DISP_CLOCK covers both "no network" and "say the wake
// word" (the clock face itself doesn't distinguish them).
typedef enum {
    DISP_CLOCK,
    DISP_CONNECTING,
    DISP_LIVE_IDLE,
    DISP_RECORDING,
    DISP_THINKING,
    DISP_SPEAKING,
} nova_display_state_t;

// Independent of the app-wide task_light_t in nova.h - this is the same
// three-value idea (background-task indicator), owned by this component so
// it doesn't have to pull in the whole app header just to know the type.
typedef enum {
    NOVA_DISPLAY_TASK_NONE,
    NOVA_DISPLAY_TASK_RUNNING,
    NOVA_DISPLAY_TASK_DONE,
} nova_display_task_light_t;
