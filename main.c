#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// -----------------------------------------------------------------------------
// Candidate Assignment: AVR LED Mode Controller
// -----------------------------------------------------------------------------
// Objective
// Implement a small AVR C program that changes LED blinking behavior based on a
// switch input. Use the AVR GCC toolchain to build and Wokwi VSCode Extension
// to simulate the result.
//
// What is Wokwi?
// - Wokwi is a hardware simulator that lets you run embedded firmware without
//   a physical board.
// - Example: https://wokwi.com/projects/461302817329780737
// - In this assignment, you will compile your AVR program, then load the
//   generated .elf or .hex into Wokwi to verify LED behavior with the switch.
// - This means your submission can be validated consistently even without
//   real hardware.
//
// Target Platform
// - MCU: ATmega328P
// - Language: C
// - Toolchain: AVR GCC
// - Simulator: Wokwi VSCode Extension
//
// Functional Requirements
// 1. Switch input controls blinking mode.
// 2. If the switch is LEFT: blink RED LED every 500 ms.
// 3. If the switch is RIGHT: blink GREEN and BLUE LEDs every 500 ms.
// 4. Use an interrupt to detect switch changes and update the active mode.
//
// Note:
// 1. Reference build artifacts .elf and .hex are provided.
// 2. Complete main.c and simulate by editing wokwi.toml to point to your generated .elf or .hex file.
//
// Pin Mapping
// - Switch: pin 5
// - RED LED: pin 13
// - GREEN LED: pin 12
// - BLUE LED: pin 11
//
//
// Minimum Deliverables
// 1. Git repository with working implementation in this file.
//    - repo should include a README with build instructions and any notes.
//    - diagram.json should be included for Wokwi simulation.
//    - .elf or .hex file generated from your build process.
//    - source file used to build the .elf or .hex.
// 2. Behavior aligned with reference implementation according to requirements.
// 3. Build notes at the top of the file, including compiler flags and commands.
// 4. Testability support with test-build and release-build configurations.
// 5. Comments for any assumptions or tradeoffs.
//
// Build and Simulation (high level)
// 1. Build with AVR GCC and generate .elf or .hex.
// 2. Update wokwi.toml to point to your generated artifact.
// 3. Open the Wokwi diagram in VSCode and start the simulation.
// 4. Toggle the virtual switch and verify both required blinking modes.
//
// Evaluation Notes
// - The checklist above is a baseline.
// - You may add improvements or optimizations as long as core behavior remains
//   correct and requirements are met.
// - comment any assumptions and tradeoffs in your implementation
// - testability and maintainability will be considered in evaluation
//
// Helpful Resources
// - AVR GCC toolchain:
//   https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers
// - Wokwi VSCode getting started:
//   https://docs.wokwi.com/vscode/getting-started
// - ATmega328P quick reference:
//


volatile int gLedMode = 0;

#define SWITCH_PIN PD5

#define RED_PIN PB5
#define GREEN_PIN PB4
#define BLUE_PIN PB3

#define SWITCH_LEFT 1
#define SWITCH_RIGHT 0


void CheckMode() {
	int switchMode = PIND & (1 << PD5);		// Check switch position
	if ( switchMode == 0 ) {				// Set LED mode
		gLedMode = SWITCH_RIGHT;
	} else {
		gLedMode = SWITCH_LEFT;
	}
}

// ---- ISR Code ----
ISR(PCINT2_vect)
{
	CheckMode();
}


int main(void)
{
    // ---- Initialization code ----
	// Initialization LED
	DDRB = 0x0;								// 	clear port
	DDRB |= (1 << RED_PIN);					//	P13 Red
	DDRB |= (1 << GREEN_PIN);				//	P12 Green
	DDRB |= (1 << BLUE_PIN);				//  P11 Blue

	// Initialization interrupt
	DDRD &= ~(1 << SWITCH_PIN);				//	P13 Switch
	PORTD |= (1 << PD5);					//	set input for switch
	EICRA |= (1 << ISC10);					//	any logical change on INT1 generates an interrupt request
	EIMSK |= (1 << INT1);					//  enable int1 mask
	PCICR |= (1 << PCIE2);					//	set pin change for switch
	PCMSK2 |= (1 << PCINT21);				//	set pin change for switch

	CheckMode();

    sei();

    while (1)
    {
        //---- application code ----
    	if ( gLedMode == SWITCH_LEFT ) {

    		// Toggle RED
    		PORTB ^= (1 << RED_PIN);			// toggle red
    		PORTB &= ~(1 << GREEN_PIN);			// clear Green
    		PORTB &= ~(1 << BLUE_PIN);			// clear Blue

    	} else {
    		// Toggle GREEN and BLUE
    		PORTB &= ~(1 << RED_PIN);			// clear Red
    		PORTB ^= (1 << GREEN_PIN);			// toggle Green
    		PORTB ^= (1 << BLUE_PIN);			// toggle Blue
    	}

		// Delay for 500ms
		_delay_ms(500);

    }

    return 0;
}
