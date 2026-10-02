idf_component_register(
  SRCS "src/panel.c" "src/anim_helpers.c" "src/orb_template.c" "src/display_core.c"
       "src/fonts/nova_font_time_80.c"
       "src/states/state_clock.c" "src/states/state_connecting.c" "src/states/state_live_idle.c"
       "src/states/state_recording.c" "src/states/state_thinking.c" "src/states/state_speaking.c"
  INCLUDE_DIRS "include"
  PRIV_INCLUDE_DIRS "src" "src/states"
  REQUIRES esp_lcd esp_driver_spi
)
