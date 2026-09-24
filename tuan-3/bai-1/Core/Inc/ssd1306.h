#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f1xx_hal.h"

#define SSD1306_WIDTH       128U
#define SSD1306_HEIGHT       64U
#define SSD1306_I2C_ADDR   (0x3CU << 1)

typedef enum
{
  SSD1306_COLOR_BLACK = 0x00,
  SSD1306_COLOR_WHITE = 0x01
} SSD1306_Color;

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef SSD1306_UpdateScreen(void);
HAL_StatusTypeDef SSD1306_SetContrast(uint8_t contrast);
void SSD1306_Fill(SSD1306_Color color);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_SetTextSize(uint8_t size);
char SSD1306_WriteChar(char character);
char *SSD1306_WriteString(char *string);

#endif /* SSD1306_H */