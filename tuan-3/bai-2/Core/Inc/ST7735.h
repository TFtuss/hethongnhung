#ifndef ST7735_H
#define ST7735_H

#include "main.h"
#include <stdint.h>
#include <string.h>

/* ============================================================
 * PIN CONFIGURATION - Chỉnh theo project của bạn
 * ============================================================ */
#define TFT_CS_PORT     GPIOB
#define TFT_CS_PIN      GPIO_PIN_6

#define TFT_DC_PORT     GPIOB       // A0/DC pin
#define TFT_DC_PIN      GPIO_PIN_7

#define TFT_RST_PORT    GPIOB
#define TFT_RST_PIN     GPIO_PIN_8

#define TFT_LED_PORT    GPIOB       // Backlight
#define TFT_LED_PIN     GPIO_PIN_9

/* SCK và SDA dùng SPI hardware (SPI1: PA5=SCK, PA7=MOSI) */
extern SPI_HandleTypeDef hspi1;
#define TFT_SPI         hspi1

/* ============================================================
 * DISPLAY SIZE
 * ============================================================ */
#define TFT_WIDTH       128
#define TFT_HEIGHT      160

/* ============================================================
 * COLORS (RGB565)
 * ============================================================ */

#define BLACK           0x0000
#define WHITE           0xFFFF
#define RED             0x001F  // Đã đảo từ 0xF800
#define GREEN           0x07E0  // Giữ nguyên (vì Green nằm ở giữa)
#define BLUE            0xF800  // Đã đảo từ 0x001F
#define YELLOW          0x07FF  // Đã đảo từ 0xFFE0
#define CYAN            0xFFE0  // Đã đảo từ 0x07FF
#define MAGENTA         0xF81F  // Giữ nguyên (vì R và B đều max)
#define ORANGE          0x053F  // Tính toán lại cho BGR
#define GRAY            0x8410  // Giữ nguyên
#define DARKGREEN       0x03E0  // Giữ nguyên
#define LIGHTBLUE       0xFB00  // Tính toán lại cho BGR

/* ============================================================
 * FONT SIZE
 * ============================================================ */
#define FONT_SIZE_1     1
#define FONT_SIZE_2     2
#define FONT_SIZE_3     3

/* ============================================================
 * FUNCTION PROTOTYPES
 * ============================================================ */
// Init
void ST7735_Init(void);
void ST7735_BacklightOn(void);
void ST7735_BacklightOff(void);

// Basic drawing
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ST7735_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void ST7735_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

// Text
void ST7735_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
void ST7735_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);
void ST7735_DrawInt(uint16_t x, uint16_t y, int32_t val, uint16_t color, uint16_t bg, uint8_t size);
void ST7735_DrawFloat(uint16_t x, uint16_t y, float val, uint8_t decimals, uint16_t color, uint16_t bg, uint8_t size);

#endif /* ST7735_H */
