#ifndef LCD_H    /* Guard against multiple inclusion */
#define LCD_H
#include <stdint.h>
enum LCD_Pins {
    RS = 9, // A
    EN = 8, // A
	D0 = 12, // B
	D1 = 13, // B
	D2 = 10, // A // doesnt work switching to PA10 channel A
	D3 = 15, // B
    D4 = 6, // C
    D5 = 7, // C
    D6 = 8, // C
    D7 = 9, // C
    DMask = 0 // ALL
};
enum LCD_Pins_channel {
	CA = 0,
	CB = 1,
	CC = 2,
	all = 3
};

enum LCD_CMD {
    Clear = 0x01, //0001
    Home = 0x02, //0010
    LCDOn = 0x0C, //1100
    EMS = 0x06, //0111
    DDRM = 0x80 //10000000
};

void delay_ms(int t);
void delay_us3(int t);
void LCD_HIGH(int pin, int channel);
void LCD_LOW(int pin, int channel);
void LCD_EN_PULSE();
void LCD_OUT(uint8_t HALF);
void LCD_CMD(uint8_t CMD);
void LCD_DATA(uint8_t d);
void LCD_INIT();
void LCD_CLEAR();
void LCD_WRITE_CHAR(char c);
void LCD_WRITE_STR(char s[], int size);
#endif 
