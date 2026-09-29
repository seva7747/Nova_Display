#pragma once
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "nova_display.h"

#define NOVA_LCD_W 240
#define NOVA_LCD_H 240

// SPI bus + GC9A01 panel bring-up. Leaves LVGL out of it - display_core.c
// hands the resulting handles to esp_lvgl_port itself.
esp_err_t nova_panel_init(const nova_display_pins_t *pins,
                           esp_lcd_panel_handle_t *panel,
                           esp_lcd_panel_io_handle_t *io);
