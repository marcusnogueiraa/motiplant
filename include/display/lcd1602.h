#ifndef LCD1602_H
#define LCD1602_H

#include <stdint.h>
#include <stdbool.h>

#define SLAVE_ADDRESS_LCD 0x27
#define I2C_DEVICE_FILE_PATH "/dev/i2c-2"

#define LCD_CLEAR_DISPLAY 0x01
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_FUNCTION_SET 0x20

#define LCD_RS         (1 << 0)
#define LCD_EN         (1 << 2)
#define LCD_BACK_LIGHT (1 << 3)

extern int lcd_fd;

short lcd1602_write(uint8_t address, uint8_t *pData, uint8_t len);
short lcd1602_init(void);
void lcd1602_sendCommand(char command);
void lcd1602_sendData(uint8_t data);
void lcd1602_sendString(char *str);
void lcd1602_setCursorPosition(bool row, int column);
void lcd1602_clear(void);
void lcd1602_close(void);
short lcd1602_open(void);

#endif
