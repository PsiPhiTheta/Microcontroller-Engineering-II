//0. INFORMATION & NOTES

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 1			/
	// Exercise No. 3		/
	/////////////////////////

	//Port A: Analogue inputs from the rotary potentiometer (RA0), the LM335Z temperature sensor (RA1) and the Light Dependent Resistor (RA2). Digital output to control the LED transistor Q3 (RA4). 5V closed, 0V open. 
	//Port B: Push-button PB1 digital input (RB0).
	//Port C: Toggle switch digital inputs (RC2, RC3, RC4 and RC5). Least significant.
	//Port F: Digital outputs to control 8 LEDs and 7seg displays (RF0 ? RF7). Write 0V to illuminate. 
	//Port H: Digital outputs to control 7-segment transistors Q1 and Q2 (RH0 and RH1). 0V closed, 5V open. Toggle switch digital inputs (RH4, RH5, RH6 and RH7). Most significant.
	//Port J: Push-button PB1 digital input (RJ5). Digital output for controlling the sounder (RJ6).
	//7seg - 0:132, 1:245, 2:76, 3:100, 4:53, 5:38, 6:6, 7:244, 8:4, 9:36, A:20, B:7, C:142, D:132, E:14, F:30

//1. Preprocessor directives
	#include "xc_config_settings.h"
	#include "delays.h"

//2. DEFINITIONS	
	//#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885

//TO CHANGE DURING LAB 
    #define UM 0b11111110                                                       //change to 0b11111101 for U2
    #define HEX0 4                                                              //8 - change to any symbol
    #define HEX1 36                                                             //9 - change to any symbol
    #define HEX2 20                                                             //A - change to any symbol
    #define HEX3 7                                                              //B - change to any symbol
   
//3. FUNCTIONS
unsigned char read_S1(void) {
    unsigned char x;
    TRISC=0xFF;
    x = PORTC >> 2;
    return x;
}

unsigned char read_S2(void) {
    unsigned char x;
    TRISH=0xF0;
    x = PORTH >> 4;
    return x;
}

void display_on_U1 (unsigned int value){
	LATH = UM;
	LATF = value;
}

//4. MAIN
	
void main (void) {
    
    unsigned char switch_bank, x, y, z; //variable declarations 
    
    // Configuration of I/O
    ADCON1 = 0x0F; // enable digital I/O
    
    TRISC = 0b00111100; // Port C bits all inputs - C2-C5 are switches 0-3
    TRISH = 0b11110000; // Port H bits all inputs - H4-H7 are switches 4-7
    
    TRISF = 0x00; // Port F output - LEDs/7seg display
    
    TRISA = 0x00; //LED transistor enable
    LATA = 0b00010000; //LED transistor enable
    
    // Read & Display 
    int patterns[4] = {HEX0, HEX1, HEX2, HEX3}; 
    
    while (1){
        switch_bank = read_S1();                                                //change this to s2 if required
    
        switch(switch_bank){
            case 1:
                display_on_U1(patterns[0]); 
                break;
            case 2:
                display_on_U1(patterns[1]); 
                break;
            case 4:
                display_on_U1(patterns[2]); 
                break;
            case 8:
                display_on_U1(patterns[3]); 
                break;
            default:
                LATH = 0b11111110;
                LATF = 0b11111111;
        }
    } 
}
    
//6. DEBUG STATUS debugged
