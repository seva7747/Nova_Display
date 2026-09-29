// Wires up nova.h's display API on top of the nova_display component
// (../components/nova_display), which owns the actual GC9A01/LVGL panel and
// every state's screen. This file only adds the bits that are specific to
// this app rather than to the display hardware: turning task_light_t /
// nova_role_t into the component's own types, and the JSON selftest report.
//
// There's also no room for a live transcript on a 240x240 round face, so
// display_text() is a no-op here - the serial log and bridge.py still carry
// the full transcript.
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nova_display.h"
#include "nova.h"

#define TAG "display"

void display_init(void)
{
    const nova_display_pins_t pins = {
        .sclk = PIN_TFT_SCLK,
        .mosi = PIN_TFT_MOSI,
        .cs   = PIN_TFT_CS,
        .dc   = PIN_TFT_DC,
        .rst  = PIN_TFT_RST,
    };
    nova_display_init(&pins);
}

void display_time_synced(void)  // called by net.c once SNTP lands the first fix
{
    nova_display_time_synced();
}

void display_task_light(task_light_t light)
{
    nova_display_task_light_t mapped = NOVA_DISPLAY_TASK_NONE;
    if (light == TASK_LIGHT_RUNNING) {
        mapped = NOVA_DISPLAY_TASK_RUNNING;
    } else if (light == TASK_LIGHT_DONE) {
        mapped = NOVA_DISPLAY_TASK_DONE;
    }
    nova_display_set_task_light(mapped);
}

void display_orb_state(nova_display_state_t st)
{
    nova_display_set_state(st);
}

void display_state(const char *label)
{
    ESP_LOGI(TAG, "[%s]", label);
    nova_display_set_label(label);
}

void display_text(nova_role_t role, const char *text)
{
    (void)role;
    (void)text;
}

void display_selftest(void)
{
    display_state("Display test");
    char line[64];
    snprintf(line, sizeof(line), "{\"type\":\"display_test\",\"panel\":%s}", nova_display_ready() ? "true" : "false");
    serial_emit_raw(line);
}
