#include "ST7735.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* ============================================================
 * FONT 5x7
 * ============================================================ */
static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // 0x20 space
    {0x00,0x00,0x5F,0x00,0x00}, // !
    {0x00,0x07,0x00,0x07,0x00}, // "
    {0x14,0x7F,0x14,0x7F,0x14}, // #
    {0x24,0x2A,0x7F,0x2A,0x12}, // $
    {0x23,0x13,0x08,0x64,0x62}, // %
    {0x36,0x49,0x55,0x22,0x50}, // &
    {0x00,0x05,0x03,0x00,0x00}, // '
    {0x00,0x1C,0x22,0x41,0x00}, // (
    {0x00,0x41,0x22,0x1C,0x00}, // )
    {0x14,0x08,0x3E,0x08,0x14}, // *
    {0x08,0x08,0x3E,0x08,0x08}, // +
    {0x00,0x50,0x30,0x00,0x00}, // ,
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x60,0x60,0x00,0x00}, // .
    {0x20,0x10,0x08,0x04,0x02}, // /
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x56,0x36,0x00,0x00}, // ;
    {0x08,0x14,0x22,0x41,0x00}, // <
    {0x14,0x14,0x14,0x14,0x14}, // =
    {0x00,0x41,0x22,0x14,0x08}, // >
    {0x02,0x01,0x51,0x09,0x06}, // ?
    {0x32,0x49,0x79,0x41,0x3E}, // @
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x3F,0x40,0x38,0x40,0x3F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x00,0x7F,0x41,0x41,0x00}, // [
    {0x02,0x04,0x08,0x10,0x20}, // backslash
    {0x00,0x41,0x41,0x7F,0x00}, // ]
    {0x04,0x02,0x01,0x02,0x04}, // ^
    {0x40,0x40,0x40,0x40,0x40}, // _
    {0x00,0x01,0x02,0x04,0x00}, // `
    {0x20,0x54,0x54,0x54,0x78}, // a
    {0x7F,0x48,0x44,0x44,0x38}, // b
    {0x38,0x44,0x44,0x44,0x20}, // c
    {0x38,0x44,0x44,0x48,0x7F}, // d
    {0x38,0x54,0x54,0x54,0x18}, // e
    {0x08,0x7E,0x09,0x01,0x02}, // f
    {0x0C,0x52,0x52,0x52,0x3E}, // g
    {0x7F,0x08,0x04,0x04,0x78}, // h
    {0x00,0x44,0x7D,0x40,0x00}, // i
    {0x20,0x40,0x44,0x3D,0x00}, // j
    {0x7F,0x10,0x28,0x44,0x00}, // k
    {0x00,0x41,0x7F,0x40,0x00}, // l
    {0x7C,0x04,0x18,0x04,0x78}, // m
    {0x7C,0x08,0x04,0x04,0x78}, // n
    {0x38,0x44,0x44,0x44,0x38}, // o
    {0x7C,0x14,0x14,0x14,0x08}, // p
    {0x08,0x14,0x14,0x18,0x7C}, // q
    {0x7C,0x08,0x04,0x04,0x08}, // r
    {0x48,0x54,0x54,0x54,0x20}, // s
    {0x04,0x3F,0x44,0x40,0x20}, // t
    {0x3C,0x40,0x40,0x20,0x7C}, // u
    {0x1C,0x20,0x40,0x20,0x1C}, // v
    {0x3C,0x40,0x30,0x40,0x3C}, // w
    {0x44,0x28,0x10,0x28,0x44}, // x
    {0x0C,0x50,0x50,0x50,0x3C}, // y
    {0x44,0x64,0x54,0x4C,0x44}, // z
};

/* ============================================================
 * LOW LEVEL SPI & GPIO
 * ============================================================ */
static inline void CS_LOW(void)  { HAL_GPIO_WritePin(TFT_CS_PORT,  TFT_CS_PIN,  GPIO_PIN_RESET); }
static inline void CS_HIGH(void) { HAL_GPIO_WritePin(TFT_CS_PORT,  TFT_CS_PIN,  GPIO_PIN_SET);   }
static inline void DC_LOW(void)  { HAL_GPIO_WritePin(TFT_DC_PORT,  TFT_DC_PIN,  GPIO_PIN_RESET); }
static inline void DC_HIGH(void) { HAL_GPIO_WritePin(TFT_DC_PORT,  TFT_DC_PIN,  GPIO_PIN_SET);   }
static inline void RST_LOW(void) { HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_RESET); }
static inline void RST_HIGH(void){ HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_SET);   }

static void SPI_Write(uint8_t data) {
    HAL_SPI_Transmit(&TFT_SPI, &data, 1, HAL_MAX_DELAY);
}

static void SPI_WriteBuf(uint8_t *buf, uint32_t len) {
    HAL_SPI_Transmit(&TFT_SPI, buf, len, HAL_MAX_DELAY);
}

static void WriteCmd(uint8_t cmd) {
    DC_LOW(); CS_LOW();
    SPI_Write(cmd);
    CS_HIGH();
}

static void WriteData(uint8_t data) {
    DC_HIGH(); CS_LOW();
    SPI_Write(data);
    CS_HIGH();
}

static void WriteData16(uint16_t data) {
    DC_HIGH(); CS_LOW();
    uint8_t buf[2] = { data >> 8, data & 0xFF };
    SPI_WriteBuf(buf, 2);
    CS_HIGH();
}

/* ============================================================
 * ST7735 INIT SEQUENCE
 * ============================================================ */
void ST7735_Init(void) {
    // Hardware reset
    RST_LOW(); HAL_Delay(10);
    RST_HIGH(); HAL_Delay(120);

    WriteCmd(0x01); HAL_Delay(150);  // SW reset
    WriteCmd(0x11); HAL_Delay(500);  // Sleep out

    // Frame rate
    WriteCmd(0xB1);
    WriteData(0x01); WriteData(0x2C); WriteData(0x2D);

    WriteCmd(0xB2);
    WriteData(0x01); WriteData(0x2C); WriteData(0x2D);

    WriteCmd(0xB3);
    WriteData(0x01); WriteData(0x2C); WriteData(0x2D);
    WriteData(0x01); WriteData(0x2C); WriteData(0x2D);

    WriteCmd(0xB4); WriteData(0x07);  // Column inversion

    // Power sequence
    WriteCmd(0xC0);
    WriteData(0xA2); WriteData(0x02); WriteData(0x84);
    WriteCmd(0xC1); WriteData(0xC5);
    WriteCmd(0xC2); WriteData(0x0A); WriteData(0x00);
    WriteCmd(0xC3); WriteData(0x8A); WriteData(0x2A);
    WriteCmd(0xC4); WriteData(0x8A); WriteData(0xEE);
    WriteCmd(0xC5); WriteData(0x0E); // VCOM

    WriteCmd(0x36); WriteData(0xC8);  // MX, MY, RGB mode

    // Color mode 16bit
    WriteCmd(0x3A); WriteData(0x05);

    // Gamma
    WriteCmd(0xE0);
    WriteData(0x02); WriteData(0x1C); WriteData(0x07);
    WriteData(0x12); WriteData(0x37); WriteData(0x32);
    WriteData(0x29); WriteData(0x2D); WriteData(0x29);
    WriteData(0x25); WriteData(0x2B); WriteData(0x39);
    WriteData(0x00); WriteData(0x01); WriteData(0x03); WriteData(0x10);

    WriteCmd(0xE1);
    WriteData(0x03); WriteData(0x1D); WriteData(0x07);
    WriteData(0x06); WriteData(0x2E); WriteData(0x2C);
    WriteData(0x29); WriteData(0x2D); WriteData(0x2E);
    WriteData(0x2E); WriteData(0x37); WriteData(0x3F);
    WriteData(0x00); WriteData(0x00); WriteData(0x02); WriteData(0x10);

    WriteCmd(0x13); HAL_Delay(10);   // Normal display
    WriteCmd(0x29); HAL_Delay(100);  // Display on

    ST7735_BacklightOn();
    ST7735_FillScreen(BLACK);
}

void ST7735_BacklightOn(void)  { HAL_GPIO_WritePin(TFT_LED_PORT, TFT_LED_PIN, GPIO_PIN_SET); }
void ST7735_BacklightOff(void) { HAL_GPIO_WritePin(TFT_LED_PORT, TFT_LED_PIN, GPIO_PIN_RESET); }

/* ============================================================
 * SET ADDRESS WINDOW
 * ============================================================ */
static void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    WriteCmd(0x2A);
    WriteData(0x00); WriteData(x0);
    WriteData(0x00); WriteData(x1);

    WriteCmd(0x2B);
    WriteData(0x00); WriteData(y0);
    WriteData(0x00); WriteData(y1);

    WriteCmd(0x2C);
}

/* ============================================================
 * DRAWING FUNCTIONS
 * ============================================================ */
void ST7735_FillScreen(uint16_t color) {
    ST7735_FillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, color);
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
    SetAddrWindow(x, y, x, y);
    WriteData16(color);
}

void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
    if (x + w > TFT_WIDTH)  w = TFT_WIDTH  - x;
    if (y + h > TFT_HEIGHT) h = TFT_HEIGHT - y;

    SetAddrWindow(x, y, x + w - 1, y + h - 1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    DC_HIGH(); CS_LOW();
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        SPI_Write(hi);
        SPI_Write(lo);
    }
    CS_HIGH();
}

void ST7735_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ST7735_FillRect(x,         y,         w, 1, color);
    ST7735_FillRect(x,         y + h - 1, w, 1, color);
    ST7735_FillRect(x,         y,         1, h, color);
    ST7735_FillRect(x + w - 1, y,         1, h, color);
}

void ST7735_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs((int16_t)x1 - x0);
    int16_t dy = abs((int16_t)y1 - y0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx - dy;

    while (1) {
        ST7735_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

void ST7735_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r, ddF_x = 1, ddF_y = -2 * r;
    int16_t x = 0, y = r;
    ST7735_DrawPixel(x0, y0 + r, color);
    ST7735_DrawPixel(x0, y0 - r, color);
    ST7735_DrawPixel(x0 + r, y0, color);
    ST7735_DrawPixel(x0 - r, y0, color);
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        ST7735_DrawPixel(x0+x, y0+y, color); ST7735_DrawPixel(x0-x, y0+y, color);
        ST7735_DrawPixel(x0+x, y0-y, color); ST7735_DrawPixel(x0-x, y0-y, color);
        ST7735_DrawPixel(x0+y, y0+x, color); ST7735_DrawPixel(x0-y, y0+x, color);
        ST7735_DrawPixel(x0+y, y0-x, color); ST7735_DrawPixel(x0-y, y0-x, color);
    }
}

void ST7735_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    ST7735_FillRect(x0 - r, y0, 2*r+1, 1, color);
    int16_t f = 1-r, ddF_x = 1, ddF_y = -2*r, x = 0, y = r;
    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x;
        ST7735_FillRect(x0-x, y0+y, 2*x+1, 1, color);
        ST7735_FillRect(x0-x, y0-y, 2*x+1, 1, color);
        ST7735_FillRect(x0-y, y0+x, 2*y+1, 1, color);
        ST7735_FillRect(x0-y, y0-x, 2*y+1, 1, color);
    }
}

/* ============================================================
 * TEXT FUNCTIONS
 * ============================================================ */
void ST7735_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 0x20 || c > 0x7A) c = ' ';
    const uint8_t *glyph = font5x7[c - 0x20];

    for (uint8_t col = 0; col < 5; col++) {
        uint8_t line = glyph[col];
        for (uint8_t row = 0; row < 7; row++) {
            uint16_t px = x + col * size;
            uint16_t py = y + row * size;
            if (line & (1 << row))
                ST7735_FillRect(px, py, size, size, color);
            else if (bg != color)
                ST7735_FillRect(px, py, size, size, bg);
        }
    }
    // spacing
    if (bg != color)
        ST7735_FillRect(x + 5*size, y, size, 7*size, bg);
}

void ST7735_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    while (*str) {
        ST7735_DrawChar(x, y, *str++, color, bg, size);
        x += 6 * size;
        if (x + 6*size > TFT_WIDTH) { x = 0; y += 8*size; }
    }
}

void ST7735_DrawInt(uint16_t x, uint16_t y, int32_t val, uint16_t color, uint16_t bg, uint8_t size) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", (long)val);
    ST7735_DrawString(x, y, buf, color, bg, size);
}

void ST7735_DrawFloat(uint16_t x, uint16_t y, float val, uint8_t decimals, uint16_t color, uint16_t bg, uint8_t size) {
    char buf[20];
    char fmt[8];
    snprintf(fmt, sizeof(fmt), "%%.%df", decimals);
    snprintf(buf, sizeof(buf), fmt, val);
    ST7735_DrawString(x, y, buf, color, bg, size);
}
