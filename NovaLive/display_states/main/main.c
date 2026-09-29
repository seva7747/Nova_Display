// Flashable one-state-at-a-time harness for iterating on NovaLive's round
// TFT screens. Brings up only the display (no WiFi, no audio, no WebRTC
// session) so you can point it at one nova_display_state_t, flash, and look
// at the real hardware - then `idf.py menuconfig` -> "Nova Display Test" to
// pick the next one and reflash.
//
// The actual screens live in ../components/nova_display (shared with the
// real NovaLive firmware), so a design edited there shows up here and in
// the production app identically - this file only picks which one to show.
#include "esp_log.h"
#include "nova_display.h"

#define TAG "display_test"

// Round 1.28" TFT (GC9A01). Keep in sync with NovaLive/main/nova.h's
// PIN_TFT_* - this is the same board's wiring, just not shared code since
// nova.h pulls in the rest of the app (audio/WebRTC headers) that this
// harness deliberately doesn't build against.
static const nova_display_pins_t PINS = {
    .sclk = 13,  // SCK
    .mosi = 14,  // SDA
    .cs   = 15,
    .dc   = 16,
    .rst  = 17,
};

static nova_display_state_t active_state(void)
{
#if CONFIG_NOVA_TEST_STATE_CONNECTING
    return DISP_CONNECTING;
#elif CONFIG_NOVA_TEST_STATE_LIVE_IDLE
    return DISP_LIVE_IDLE;
#elif CONFIG_NOVA_TEST_STATE_RECORDING
    return DISP_RECORDING;
#elif CONFIG_NOVA_TEST_STATE_THINKING
    return DISP_THINKING;
#elif CONFIG_NOVA_TEST_STATE_SPEAKING
    return DISP_SPEAKING;
#else
    return DISP_CLOCK;
#endif
}

static nova_display_task_light_t active_task_light(void)
{
    switch (CONFIG_NOVA_TEST_TASK_LIGHT) {
        case 1:  return NOVA_DISPLAY_TASK_RUNNING;
        case 2:  return NOVA_DISPLAY_TASK_DONE;
        default: return NOVA_DISPLAY_TASK_NONE;
    }
}

void app_main(void)
{
    nova_display_init(&PINS);
    nova_display_time_synced();               // so the clock state shows a real time, not --:--
    nova_display_set_label("Test label");      // demo text for states that show one
    nova_display_set_task_light(active_task_light());
    nova_display_set_state(active_state());
    ESP_LOGI(TAG, "showing state %d (nova_display_ready=%d)", active_state(), nova_display_ready());
}
