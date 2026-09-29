#pragma once
#include <stdbool.h>
#include "nova_display_types.h"

// Round 1.28" TFT (GC9A01, 240x240, 4-wire SPI) driven through LVGL via
// esp_lvgl_port. Whoever calls nova_display_init() owns the pin numbers -
// this component doesn't hardcode a particular project's wiring.
typedef struct {
    int sclk;
    int mosi;
    int cs;
    int dc;
    int rst;
} nova_display_pins_t;

// Brings up the panel + LVGL and builds every state's screen up front.
// Safe to call even with nothing wired up: SPI is write-only, so (unlike an
// I2C probe) there's no way to detect a missing panel - this just logs a
// warning and every other call below becomes a no-op.
void nova_display_init(const nova_display_pins_t *pins);
bool nova_display_ready(void);

void nova_display_set_state(nova_display_state_t state);
void nova_display_set_label(const char *text);
void nova_display_set_task_light(nova_display_task_light_t light);
void nova_display_time_synced(void);  // call once SNTP lands the first fix
