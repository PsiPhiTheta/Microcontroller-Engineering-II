//0. INFORMATION & NOTES

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 1			/
	// Exercise No. 2		/
	/////////////////////////

	//Port A: Analogue inputs from the rotary potentiometer (RA0), the LM335Z temperature sensor (RA1) and the Light Dependent Resistor (RA2). Digital output to control the LED transistor Q3 (RA4). 5V closed, 0V open. 
	//Port B: Push-button PB1 digital input (RB0).
	//Port C: Toggle switch digital inputs (RC2, RC3, RC4 and RC5). Least significant.
	//Port F: Digital outputs to control 8 LEDs and 7seg displays (RF0 ? RF7). Write 0V to illuminate. 
	//Port H: Digital outputs to control 7-segment transistors Q1 and Q2 (RH0 and RH1). 0V closed, 5V open. Toggle switch digital inputs (RH4, RH5, RH6 and RH7). Most significant.
	//Port J: Push-button PB1 digital input (RJ5). Digital output for controlling the sounder (RJ6).

//1. Preprocessor directives
	#include "xc_config_settings.h"
	#include "delays.h"

//2. DEFINITIONS	
	//#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTIONS
void display_on_U1 (unsigned int value){
	LATH = 0b11111110;
	LATF = value;
}
    

//4. MAIN
	
void main (void) {
    
    unsigned char x, y, z; //variable declarations 
    
    // Configuration of I/O
    ADCON1 = 0x0F; // enable digital I/O
    
    TRISC = 0b00111100; // Port C bits all inputs - C2-C5 are switches 0-3
    TRISH = 0b11110000; // Port H bits all inputs - H4-H7 are switches 4-7
    
    TRISF = 0x00; // Port F output - LEDs/7seg display
    
    TRISA = 0x00; //LED transistor enable
    LATA = 0b00010000; //LED transistor enable
    
    // Display 
    display_on_U1(245);
    
}
    
//6. DEBUG STATUS debugged
