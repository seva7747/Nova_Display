# Nova Display Test

An ESP-IDF app for looking at one round-TFT state at a time on the real
board. It builds only against `../components/nova_display` - the panel
driver and per-state screens - so no other component (mic, amp, WiFi,
WebRTC) gets built or wired up. This is currently the only app in the
checkout; the rest of the mic_esp/Nova firmware has been stripped out of
this branch since it's display-only work.

## Files being iterated on

Each state is its own file under `../components/nova_display/src/states/`:

```
state_clock.c        rest screen: minimalist HH:MM
state_connecting.c   wake word heard -> session coming up
state_live_idle.c    session live, waiting for you to talk
state_recording.c    GPT-Live-1 hearing you
state_thinking.c     your turn sent, no answer yet
state_speaking.c     Nova's reply playing back
```

All five non-clock states currently lean on the shared placeholder look in
`orb_template.c` (a soft glow + rotating gradient ring, styled after the web
app's `NovaOrb.tsx`). To redesign a
state, edit its file directly: replace the `nova_orb_*` calls with whatever
LVGL objects the design calls for. Nothing else depends on any state staying
on the template.

## Workflow

```bash
idf.py set-target esp32s3      # once, if this project has never been built
idf.py menuconfig              # "Nova Display Test" -> pick a state (and,
                                # optionally, a task-light overlay to preview)
./flash.ps1                    # auto-detects the COM port
./flash.ps1 COM3               # or name it
```

Look at the board, then repeat: pick the next state in menuconfig, reflash.
`idf.py -p <port> monitor` shows the log line confirming which state loaded
and whether the panel was actually found.

## Wiring

Round 1.28" TFT (GC9A01, 240x240, 4-wire SPI), see `../../README.md`:
SCK 13 / SDA 14 / CS 15 / DC 16 / RST 17. No backlight pin; it's tied
on-board, always on.

## Graduating a design

Once a state's file looks right here, it *is* the code - there's nothing to
port back. A future app that wants this display just depends on the
`nova_display` component the same way this harness does.
