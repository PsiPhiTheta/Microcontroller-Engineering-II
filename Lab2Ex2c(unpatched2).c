//0. INFORMATION & NOTES

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 2			/
	// Exercise No. 2c		/
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
    #define LAB_CHAR_1 132                                                      // modify this during lab
    #define LAB_CHAR_2 245                                                      // modify this during lab
    #define LAB_tm 1                                                            // modify this during lab (tm = 1/tp)
    #define LAB_Fmin 1                                                          // modify this during lab
    #define LAB_Fmax 75                                                         // modify this during lab
	//#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTIONS
void configure_ADC(void){
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_0_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 7); 
}

unsigned int get_ADC_value(void){
    ConvertADC();           // initiate conversion
	while (BusyADC());      // wait for conversion to complete
	return ReadADC()/4; 	// return scaled (2^10->2^8) converted value from pot
}

char U1_on (void){
    LATH = 0b11111110;
}

char U2_on (void) {
    LATH = 0b11111101;
}

unsigned char int_to_SSD (unsigned int val){
    switch(val) {
        case 0:
            return 132;
        case 1:
            return 245;
        case 2:
            return 76;
        case 3:
            return 100;
        case 4:
            return 53;
        case 5:
            return 38;
        case 6:
            return 6;
        case 7:
            return 244;
        case 8:
            return 4;
        case 9:
            return 36;
        case 10:
            return 20;
        case 11:
            return 7;
        case 12:
            return 142;
        case 13:
            return 132;
        case 14:
            return 14;
        case 15:
            return 30;
        default:
            return 0xFF;
    }
}

//4. MAIN
void main (void) {

    LATA = 0b00000000;              // LED transistor disable
    TRISA = 0b11101111;             // LED transistor set as output
    
    LATH = 0b00000000;              // both U1 and U2 enabled
    TRISH = 0b11111100;             // 7seg transistor set as output
           
    LATF = 0b00000000;              // switch on all LEDs
    TRISF = 0b00000000;             // all bits of Port F as outputs
    
    Delay10KTCYx(250);              // all LEDs on for 1 sec
    Delay10KTCYx(250);              // all LEDs on for 1 sec 
    Delay10KTCYx(250);              // all LEDs on for 1 sec 
    LATF = 0b11111111;              // shows program passed test1
    Delay10KTCYx(250);              // all LEDs on for 1 sec
    Delay10KTCYx(250);              // all LEDs on for 1 sec 
    Delay10KTCYx(250);              // all LEDs on for 1 sec
                                    // shows program about to start
    configure_ADC();    
    
    unsigned int x, y, z;
    float freq, delay;
    
     while (1) {
        
        x = get_ADC_value(); 
        y = x & 0b00001111;
        z = (x & 0b11110000) >> 4 ;
        
        freq = LAB_Fmin + (((LAB_Fmax) - (LAB_Fmin))*x) ;
        delay = freq; // convert hZ to delay accurately
        
        LATF = int_to_SSD(y); 
        LATH = 0b11111110; //enable U1
        for (unsigned int i = 255; i<(delay) ; i++){ //used to avoid overflow at 255
            Delay1TCYx(1); //?ms delay
        }
                
        LATF = int_to_SSD(z); 
        LATH = 0b11111101; //enable U2
        for (unsigned int j = 255; j<(delay) ; j++){
            Delay1TCYx(1); //?ms delay
        }
    }
}

//5. Debug status: debugged	
