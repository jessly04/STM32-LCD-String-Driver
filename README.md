# STM32-LCD-String-Driver
Developed a custom peripheral driver for the STM32F030 R8T6 microcontroller. Building upon basic GPIO configurations, this project implements the LCD communication protocol to display text strings.
### Key Technical Features
- **8-Bit Parallel Interface:** Manages a full byte of data across multiple GPIO ports simultaneously.
- **String Handling:** Implemented a LCD_WRITE_STR function that enables the transmission of full character strings from the MCU's memory to the display.
- **Timing Pulse Logic:** Added an EN (Enable) pulse routine with microsecond-level precision to meet the LCD controller's data-setup and hold-time requirements.
- **Manual GPIO Setup:** Includes a GPIO_Init function that manually configures registers such as MODER (Mode Register) and OTYPER (Output Type Register) for high-speed push-pull output.
### Hardware
- **Microcontroller:** STM32F030R8T6
- **Display:** 16x2 Character LCD
- **Level Shifter:** ULN2003
- **Programmer:** ST-Link V2
### Challenge & Solution
The main problem we faced was ensuring that the STM32 didn't outrun the LCD since the STM32 is significantly faster than the LCD controller. To resolve this issue, we synchronized the RS (Register Select) pin timing with the data bus, triggered an Enable Pulse, and then implemented a short "busy-wait" delay to allow the LCD time to process the command before the next byte arrives.
### Project Structure
- LCD.c / LCD.h: The driver logic, includes the 8-bit output and string functions
- main.c: Initialization and application loop that demonstrates string rendering
- main.h: Standard HAL headers and system definitions
### Project Collaboration
This project was co-developed by Jessica Ly and Daniel Meija Mendez. We collaborated on the timing logic and the hardware-software integration.
