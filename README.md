# Round TFT display

The round 1.28" TFT (GC9A01, 240x240, 4-wire SPI) for the mic_esp/Nova
hardware, in isolation. Everything else that used to live in this repo
(MicScribe, the bridge, WiFi setup page, the full NovaLive voice-assistant
firmware) has been stripped out of this branch - this is display work only.

```
NovaLive/
  components/nova_display/   GC9A01 + LVGL panel driver and every state's screen
    include/                    public API (nova_display.h, nova_display_types.h)
    src/panel.c                 SPI + GC9A01 bring-up
    src/display_core.c          dispatcher: nova_display_* public API
    src/orb_template.c          shared placeholder look for the 5 non-clock states
    src/states/state_*.c        one file per nova_display_state_t

  display_states/            flash-one-state-at-a-time test harness (see its own README)
```

## Wiring

Round 1.28" TFT (GC9A01, 240x240, 4-wire SPI) - no backlight pin on this
board, it's tied on-board, always on:

| TFT pin | ESP32-S3 |
|---|---|
| SCK (SPI clock) | GPIO 13 |
| SDA (SPI data in) | GPIO 14 |
| CS | GPIO 15 |
| DC | GPIO 16 |
| RST | GPIO 17 |

## Building

See [`NovaLive/display_states/README.md`](NovaLive/display_states/README.md)
for the flash-and-test workflow (ESP-IDF, `idf.py menuconfig` to pick a
state, `flash.ps1` to build and flash).
