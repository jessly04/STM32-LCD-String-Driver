#include "LCD.h"
#include "main.h"

// Since the LCD works on 5V and the SAM only works on 3.3V I had to step up the output
// I only had a ULN2003 on hand which inverts the signals. So all my signals are inverted
// the LCD you have may work on 3.3V the one i stole was broken and so i used my own
// but it uses the same driver as new haven so this should work
// just flip the macros below CLR -> SET and vise versa

int SetDef[16] = {
	 GPIO_BSRR_BS_0,
	 GPIO_BSRR_BS_1,
	 GPIO_BSRR_BS_2,
	 GPIO_BSRR_BS_3,
	 GPIO_BSRR_BS_4,
	 GPIO_BSRR_BS_5,
	 GPIO_BSRR_BS_6,
	 GPIO_BSRR_BS_7,
	 GPIO_BSRR_BS_8,
	 GPIO_BSRR_BS_9,
	 GPIO_BSRR_BS_10,
	 GPIO_BSRR_BS_11,
	 GPIO_BSRR_BS_12,
	 GPIO_BSRR_BS_13,
	 GPIO_BSRR_BS_14,
	 GPIO_BSRR_BS_15
};

int ClrDef[16] = {
	 GPIO_BSRR_BR_0,
	 GPIO_BSRR_BR_1,
	 GPIO_BSRR_BR_2,
	 GPIO_BSRR_BR_3,
	 GPIO_BSRR_BR_4,
	 GPIO_BSRR_BR_5,
	 GPIO_BSRR_BR_6,
	 GPIO_BSRR_BR_7,
	 GPIO_BSRR_BR_8,
	 GPIO_BSRR_BR_9,
	 GPIO_BSRR_BR_10,
	 GPIO_BSRR_BR_11,
	 GPIO_BSRR_BR_12,
	 GPIO_BSRR_BR_13,
	 GPIO_BSRR_BR_14,
	 GPIO_BSRR_BR_15
};

void delay_ms(int t){
	for(int i=0; i < t*1000; i++);
}

void delay_us3(int t){
	for(int i=0; i < t*500; i++);
}

void LCD_LOW(int pin, int channel){
	switch(channel){
	case CA:
		GPIOA->BSRR |= SetDef[pin];
		break;
	case CB:
		GPIOB->BSRR |= SetDef[pin];
		break;
	case CC:
		GPIOC->BSRR |= SetDef[pin];
		break;
	case all:
		GPIOB->BSRR |= SetDef[D0];
		GPIOB->BSRR |= SetDef[D1];
		GPIOA->BSRR |= SetDef[D2];
		GPIOB->BSRR |= SetDef[D3];

		GPIOC->BSRR |= SetDef[D4];
		GPIOC->BSRR |= SetDef[D5];
		GPIOC->BSRR |= SetDef[D6];
		GPIOC->BSRR |= SetDef[D7];
	}

}

void LCD_HIGH(int pin, int channel){
	switch(channel){
	case CA:
		GPIOA->BSRR |= ClrDef[pin];
		break;
	case CB:
		GPIOB->BSRR |= ClrDef[pin];
		break;
	case CC:
		GPIOC->BSRR |= ClrDef[pin];
		break;
	case all:
		GPIOB->BSRR |= ClrDef[D0];
		GPIOB->BSRR |= ClrDef[D1];
		GPIOA->BSRR |= ClrDef[D2];
		GPIOB->BSRR |= ClrDef[D3];

		GPIOC->BSRR |= ClrDef[D4];
		GPIOC->BSRR |= ClrDef[D5];
		GPIOC->BSRR |= ClrDef[D6];
		GPIOC->BSRR |= ClrDef[D7];
		break;
	}
}


// Enable pulse for the LCD display
void LCD_EN_PULSE(){
    // delay_us3(1);
	HAL_Delay(1);
    LCD_HIGH(EN, CA);
    // needs to be at least 450ns // best i can do is 3000ns
    // delay_us3(10);
    HAL_Delay(1);
    LCD_LOW(EN, CA);
}

// I use 8 bit integer here because the commands are only 2 hex
void LCD_OUT(uint8_t HALF){
    // clear all the Data out 
    LCD_LOW(DMask, all);
    // set them based on the command
    if(HALF & 0x1){LCD_HIGH(D0, CB);};
    if(HALF & 0x2){LCD_HIGH(D1, CB);};
    if(HALF & 0x4){LCD_HIGH(D2, CA);};
    if(HALF & 0x8){LCD_HIGH(D3, CB);};

    if(HALF & 0x10){LCD_HIGH(D4, CC);};
    if(HALF & 0x20){LCD_HIGH(D5, CC);};
    if(HALF & 0x40){LCD_HIGH(D6, CC);};
    if(HALF & 0x80){LCD_HIGH(D7, CC);};
    
    //delay_us3(1);
    HAL_Delay(5);
    // Write to LCD register
    LCD_EN_PULSE();
}
// As for the order of data transfer, the four high order bits are transferred before the four low order bits
void LCD_CMD(uint8_t CMD){
    // RS = 0 means CMD register, RS=1 means data register
    LCD_LOW(RS, CA);
    LCD_OUT(CMD); // last 4 bits
    
   // clear display and return home take a stupid long time
   // so delay when they come on
    if((CMD == Clear) || (CMD == Home)){HAL_Delay(3);} //im putting 5 because the delay function i used an estimate lol
    //else {delay_us3(200);} // the rest should be fine
    else{HAL_Delay(1);}
    
}

void LCD_DATA(uint8_t d){
    LCD_HIGH(RS, CA); // when RS is 1, its in data mode
    
    LCD_OUT(d); // last 4 bits
    // delay a bit
    HAL_Delay(1);
            
}

void LCD_INIT(){
    // make sure all outputs are 0
    LCD_LOW(RS, CA);
    LCD_LOW(EN, CA);
	LCD_LOW(DMask, all);
    
    // give the LCD time to power on  more than 15ms minimum
    HAL_Delay(40);
    // these commands come straight from the datasheet
    // function set DL = LOW (4 bit mode), N = High (2 line), F = low (5x8 pixel format)
    
//    // the wakey wakey protocal
//    LCD_CMD(0x38);
//    HAL_Delay(5);
//    LCD_CMD(0x38);
//    HAL_Delay(5);
//
//    // display ON
//    LCD_CMD(LCDOn);
//    HAL_Delay(5);
//
//
//    // clear display
//    LCD_CMD(Clear);
//    HAL_Delay(5);
//
//    // Entry Mode Set
//    LCD_CMD(EMS);
//
//
//    HAL_Delay(1);
    
    LCD_CLEAR();
    HAL_Delay(5);
    LCD_CMD(0b00111000);
    HAL_Delay(1);
    LCD_CMD(0b00001110);
    HAL_Delay(1);
    LCD_CMD(0b00000110);
    HAL_Delay(1);
    
}

// clears display
void LCD_CLEAR(){
    LCD_CMD(Clear);
}

void LCD_WRITE_CHAR(char c){
    // have to convert the char to uint8_t
    // LCD characters are in ASC2 so you can use char data type
    LCD_DATA((uint8_t)c);
}

// helper function to write strings
void LCD_WRITE_STR(char s[], int size){
    for(int i = 0; i < size; i++){
        LCD_DATA((uint8_t)s[i]);
    }
}
