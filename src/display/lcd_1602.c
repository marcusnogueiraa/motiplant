#include "display/lcd1602.h"
#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int lcd_fd;

short lcd1602_write(uint8_t address, uint8_t *pData, uint8_t len) {
    int ret;
    char buf[len + 1];
    buf[0] = address;
    for (int i = 1; i < (len + 1); i++) {
        buf[i] = *(pData + (i - 1));
    }

    ret = write(lcd_fd, buf, (len + 1));
    if (ret <= 0) {
        perror("Erro na escrita no LCD");
        return -1;
    }

    return 0;
}

void lcd1602_sendCommand(char command) {
    uint8_t commandBuf[4];
    commandBuf[0] = (command & 0xF0) | LCD_EN | LCD_BACK_LIGHT;
    commandBuf[1] = (command & 0xF0) | LCD_BACK_LIGHT;
    commandBuf[2] = ((command << 4) & 0xF0) | LCD_EN | LCD_BACK_LIGHT;
    commandBuf[3] = ((command << 4) & 0xF0) | LCD_BACK_LIGHT;
    lcd1602_write(SLAVE_ADDRESS_LCD, commandBuf, 4);
}

void lcd1602_sendData(uint8_t data) {
    uint8_t dataBuf[4];
    dataBuf[0] = (data & 0xF0) | LCD_EN | LCD_BACK_LIGHT | LCD_RS;
    dataBuf[1] = (data & 0xF0) | LCD_BACK_LIGHT | LCD_RS;
    dataBuf[2] = ((data << 4) & 0xF0) | LCD_EN | LCD_BACK_LIGHT | LCD_RS;
    dataBuf[3] = ((data << 4) & 0xF0) | LCD_BACK_LIGHT | LCD_RS;

    lcd1602_write(SLAVE_ADDRESS_LCD, dataBuf, 4);
}

void lcd1602_clear(void) {
    lcd1602_sendCommand(LCD_CLEAR_DISPLAY);
    usleep(2000);
}

void lcd1602_setCursorPosition(bool row, int column) {
    if (row) {
        column |= 0xC0;
    } else {
        column |= 0x80;
    }
    lcd1602_sendCommand(column);
}

short lcd1602_init(void) {
    lcd1602_sendCommand(LCD_FUNCTION_SET | 0x08);
    usleep(1000);
    lcd1602_sendCommand(LCD_DISPLAY_CONTROL);
    usleep(1000);
    lcd1602_sendCommand(LCD_CLEAR_DISPLAY);
    usleep(2000);
    lcd1602_sendCommand(LCD_ENTRY_MODE_SET | 0x02);
    usleep(1000);
    lcd1602_sendCommand(LCD_DISPLAY_CONTROL | 0x04);
    return 0;
}

void lcd1602_sendString(char *str) {
    while (*str) {
        lcd1602_sendData(*str++);
    }
}

short lcd1602_open(void) {
    lcd_fd = open(I2C_DEVICE_FILE_PATH, O_RDWR);
    if (lcd_fd < 0) {
        perror("Erro ao abrir o dispositivo I2C");
        return -1;
    }

    if (ioctl(lcd_fd, I2C_SLAVE, SLAVE_ADDRESS_LCD) < 0) {
        perror("Erro ao configurar o endereço do escravo");
        close(lcd_fd);
        return -1;
    }

    return 0;
}

void lcd1602_close(void) {
    if (lcd_fd >= 0) {
        close(lcd_fd);
    }
}
