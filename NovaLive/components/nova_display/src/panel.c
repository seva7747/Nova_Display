#include "panel.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_gc9a01.h"

#define LCD_HOST    SPI2_HOST
#define LCD_PCLK_HZ (40 * 1000 * 1000)  // 80 MHz default is tight over dupont wires

esp_err_t nova_panel_init(const nova_display_pins_t *pins,
                          esp_lcd_panel_handle_t *panel,
                          esp_lcd_panel_io_handle_t *io)
{
    spi_bus_config_t bus = GC9A01_PANEL_BUS_SPI_CONFIG(pins->sclk, pins->mosi,
                                                        NOVA_LCD_W * 80 * sizeof(uint16_t));
    esp_err_t err = spi_bus_initialize(LCD_HOST, &bus, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        return err;
    }
    esp_lcd_panel_io_spi_config_t io_cfg = GC9A01_PANEL_IO_SPI_CONFIG(pins->cs, pins->dc, NULL, NULL);
    io_cfg.pclk_hz = LCD_PCLK_HZ;
    err = esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_cfg, io);
    if (err != ESP_OK) {
        return err;
    }
    esp_lcd_panel_dev_config_t dev = {
        .reset_gpio_num = pins->rst,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };
    err = esp_lcd_new_panel_gc9a01(*io, &dev, panel);
    if (err != ESP_OK) {
        return err;
    }
    esp_lcd_panel_reset(*panel);
    esp_lcd_panel_init(*panel);
    esp_lcd_panel_invert_color(*panel, true);  // this GC9A01 module needs INVON, or colors render as a negative
    esp_lcd_panel_disp_on_off(*panel, true);
    return ESP_OK;
}
