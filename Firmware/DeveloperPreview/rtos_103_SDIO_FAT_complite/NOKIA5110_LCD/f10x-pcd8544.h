/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "global_variables.h"

// ���������� ������ LCD_CS
#define LCD_CS1   GPIO_SetBits  (LCD_PORT, SCE)
#define LCD_CS0   GPIO_ResetBits(LCD_PORT, SCE)
// ���������� ������ LCD_RST
#define LCD_RST1  GPIO_SetBits  (LCD_PORT, RST)
#define LCD_RST0  GPIO_ResetBits(LCD_PORT, RST)
// ���������� ������ LCD_DC
#define LCD_DC1   GPIO_SetBits  (LCD_PORT, DC)
#define LCD_DC0   GPIO_ResetBits(LCD_PORT, DC)
// ���������� ������ LCD_SCK
#define LCD_SCK1   GPIO_SetBits  (LCD_PORT, SCLK)
#define LCD_SCK0   GPIO_ResetBits(LCD_PORT, SCLK)
// ���������� ������ LCD_MOSI
#define LCD_MOSI1   GPIO_SetBits  (LCD_PORT, MOSI)
#define LCD_MOSI0   GPIO_ResetBits(LCD_PORT, MOSI)

// ���� ����� � ������� ��������� �������
#define SCLK GPIO_Pin_3 //PB3
#define MOSI GPIO_Pin_5	//PB5
#define DC   GPIO_Pin_7	//PB7
#define RST  GPIO_Pin_8	//PB8
#define SCE  GPIO_Pin_9 //PB9

#define LCD_PORT GPIOB	// ���� � �������� ��������� �������

#define LCD_PH RCC_APB2Periph_GPIOB // ���� � ������� ���������� �������

void lcd8544_init(void);  // �������������� �������

void lcd8544_refresh(void); // ���������� ������ �� ������

void lcd8544_putpix(unsigned char x, unsigned char y, unsigned char mode); // ����� �������

void lcd8544_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // ����� �����

void lcd8544_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // �������������

void lcd8544_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode); //  ����� �������

void lcd8544_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode); // ����� ����������� �����

void lcd8544_putstr(unsigned char x, unsigned char y, const unsigned char str[], unsigned char mode); // ����� ������

void init_spi3(void);//��������� SPI3

void SPI3Send(uint16_t data);

void startSPI3(void);

void lcd8544_dma_refresh(void);

void drawCircle(int16_t x0, int16_t y0, int16_t r, unsigned char mode);

void drawFastVLine(int16_t x, int16_t y, int16_t h, unsigned char mode);

void drawFastHLine(int16_t x, int16_t y, int16_t w, unsigned char mode);

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, unsigned char mode) ;


void drawBitmap(int16_t x, int16_t y,
			      const char *bitmap, uint8_t w, uint8_t h,
			      unsigned char mode);

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		unsigned char mode);


void drawDigit(int16_t x, int16_t y, char digit, unsigned char mode);

void drawSmallDigit(int16_t x, int16_t y, char digit, unsigned char mode);

void lcd_rounds_update(void);

void lcd_clips_update(void);

void lcd_battary_voltage_update(uint16_t voltage);

void lcd_fire_mode_update(tfire_mode fire_mode);

void lcd_health_update(void);

void lcd_time_update(void);




//void lcd8544_draw_raw(unsigned char x, unsigned char y, unsigned char width, uint16_t size);
