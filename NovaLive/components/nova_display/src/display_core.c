#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "nova_display.h"
#include "panel.h"
#include "state_common.h"
#include "state_clock.h"
#include "state_connecting.h"
#include "state_live_idle.h"
#include "state_recording.h"
#include "state_thinking.h"
#include "state_speaking.h"

#define TAG "nova_display"
#define NUM_STATES (DISP_SPEAKING + 1)

static bool s_ready;
static nova_display_state_t s_state = DISP_CLOCK;
static volatile bool s_time_synced;
static const nova_state_screen_t *s_states[NUM_STATES];

bool nova_display_is_time_synced(void)
{
    return s_time_synced;
}

void nova_display_time_synced(void)
{
    s_time_synced = true;
}

bool nova_display_ready(void)
{
    return s_ready;
}

void nova_display_init(const nova_display_pins_t *pins)
{
    esp_lcd_panel_handle_t panel = NULL;
    esp_lcd_panel_io_handle_t io = NULL;
    if (nova_panel_init(pins, &panel, &io) != ESP_OK) {
        ESP_LOGW(TAG, "no round display found - check wiring on SCLK=%d MOSI=%d CS=%d DC=%d RST=%d",
                 pins->sclk, pins->mosi, pins->cs, pins->dc, pins->rst);
        return;
    }

    const lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    if (lvgl_port_init(&port_cfg) != ESP_OK) {
        ESP_LOGW(TAG, "lvgl_port_init failed");
        return;
    }
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io,
        .panel_handle = panel,
        .buffer_size = NOVA_LCD_W * 60,
        .double_buffer = false,
        .hres = NOVA_LCD_W,
        .vres = NOVA_LCD_H,
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {.buff_dma = true, .swap_bytes = true},
    };
    if (lvgl_port_add_disp(&disp_cfg) == NULL) {
        ESP_LOGW(TAG, "lvgl_port_add_disp failed");
        return;
    }

    s_states[DISP_CLOCK]      = state_clock_ops();
    s_states[DISP_CONNECTING] = state_connecting_ops();
    s_states[DISP_LIVE_IDLE]  = state_live_idle_ops();
    s_states[DISP_RECORDING]  = state_recording_ops();
    s_states[DISP_THINKING]   = state_thinking_ops();
    s_states[DISP_SPEAKING]   = state_speaking_ops();

    lvgl_port_lock(0);
    for (int i = 0; i < NUM_STATES; i++) {
        s_states[i]->build();
    }
    lv_disp_load_scr(s_states[DISP_CLOCK]->screen());
    lvgl_port_unlock();

    s_ready = true;
    ESP_LOGI(TAG, "round display up on SPI2");
}

void nova_display_set_state(nova_display_state_t st)
{
    if (!s_ready || st == s_state) {
        return;
    }
    s_state = st;
    lvgl_port_lock(0);
    if (s_states[st]->enter) {
        s_states[st]->enter();
    }
    lv_disp_load_scr(s_states[st]->screen());
    lvgl_port_unlock();
}

void nova_display_set_label(const char *text)
{
    if (!s_ready || !s_states[s_state]->set_label) {
        return;
    }
    lvgl_port_lock(0);
    s_states[s_state]->set_label(text);
    lvgl_port_unlock();
}

// Applied to every state unconditionally, so whichever one is on screen
// (or becomes active next) is already showing the right task light.
void nova_display_set_task_light(nova_display_task_light_t light)
{
    if (!s_ready) {
        return;
    }
    lvgl_port_lock(0);
    for (int i = 0; i < NUM_STATES; i++) {
        if (s_states[i]->set_task_light) {
            s_states[i]->set_task_light(light);
        }
    }
    lvgl_port_unlock();
}
