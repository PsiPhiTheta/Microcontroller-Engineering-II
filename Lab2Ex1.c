//0. INFORMATION & NOTES

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 2			/
	// Exercise No. 1		/
	/////////////////////////

	//Port A: Analogue inputs from the rotary potentiometer (RA0), the LM335Z temperature sensor (RA1) and the Light Dependent Resistor (RA2). Digital output to control the LED transistor Q3 (RA4). 5V closed, 0V open. 
	//Port B: Push-button PB2 digital input (RB0).
	//Port C: Toggle switch digital inputs (RC2, RC3, RC4 and RC5). Least significant.
	//Port F: Digital outputs to control 8 LEDs and 7seg displays (RF0 ? RF7). Write 0V to illuminate. 
	//Port H: Digital outputs to control 7-segment transistors Q1 and Q2 (RH0 and RH1). 0V closed, 5V open. Toggle switch digital inputs (RH4, RH5, RH6 and RH7). Most significant.
	//Port J: Push-button PB1 digital input (RJ5). Digital output for controlling the sounder (RJ6).
	//7seg - 0:132, 1:245, 2:76, 3:100, 4:53, 5:38, 6:6, 7:244, 8:4, 9:36, A:20, B:7, C:142, D:132, E:14, F:30

//1. Preprocessor directives
#include "xc_config_settings.h"
#include "delays.h"
#include "pconfig.h"
#include "adc.h" //remember to enable File/Project Properties/Linker/Link in periferal

//2. DEFINITIONS
    #define MIN_ADC 0
    #define MAX_ADC 255
    #define ON 0x11
    #define OFF 0x00
    #define ONE_SEC 250
	//#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTIONS
void configure_ADC(void){
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_0_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 7); 
}

//4. MAIN
void main (void) {

    unsigned int sample;
    unsigned char vpot, x;

    TRISF = 0x00;	// all bits of Port F as outputs
    TRISA = 0b11101111;
    LATA = 0xFF;

    LATF = ON;                        	// switch on all LEDs
    Delay10KTCYx(ONE_SEC);              // all LEDs on for 1 sec
    Delay10KTCYx(ONE_SEC);              // all LEDs on for 1 sec ? shows 
                                        // program has started and LEDs
                                        // operational
    LATF = OFF;

    configure_ADC();
      
     while (1) {
       ConvertADC();                   	// initiate conversion
		while (BusyADC());                       // wait for conversion to complete
		sample = ReadADC(); 	// read converted value from pot
		vpot = sample/4;            // scale sample to 8 bits
        
        LATF = vpot; 
    }

}

//5. Debug status: debugged	
