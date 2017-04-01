//0. INFORMATION & NOTES_________________________________________________________________________________________________________________

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 4			/
	// Exercise No. 2		/
	/////////////////////////

	//Port A: Analogue inputs from the rotary potentiometer (RA0), the LM335Z temperature sensor (RA1) and the Light Dependent Resistor (RA2). Digital output to control the LED transistor Q3 (RA4). 5V closed, 0V open. 
	//Port B: Push-button PB2 digital input (RB0).
	//Port C: Toggle switch digital inputs (RC2, RC3, RC4 and RC5). Least significant.
	//Port F: Digital outputs to control 8 LEDs and 7seg displays (RF0 ? RF7). Write 0V to illuminate. 
	//Port H: Digital outputs to control 7-segment transistors Q1 and Q2 (RH0 and RH1). 0V closed, 5V open. Toggle switch digital inputs (RH4, RH5, RH6 and RH7). Most significant.
	//Port J: Push-button PB1 digital input (RJ5). Digital output for controlling the sounder (RJ6).
	//7seg - 0:132, 1:245, 2:76, 3:100, 4:53, 5:38, 6:6, 7:244, 8:4, 9:36, A:20, B:7, C:142, D:132, E:14, F:30

//1. PREPROCESSOR DIRECTIVES____________________________________________________________________________________________________________
    #include "xc_config_settings.h"
    #include "delays.h"
    #include "pconfig.h"
    #include "adc.h"                                                            //remember to enable File/Project Properties/Linker/Link in periferal
    #include "stdint.h"
    #include "timers.h"
	#include "usart.h"

//2. DEFINITIONS________________________________________________________________________________________________________________________
    //#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTION PROTOTYPES_______________________________________________________________________________________________________________
	void io_config(void);
	void usart_config(void);
	void inter_config(void);
	void interrupt isr(void);

//4. MAIN______________________________________________________________________________________________________________________________
	void main(){
		io_config(); //configures io
		usart_config(); //configures usart 
		inter_config(); //configures interrupts
		while(1);
	}

//5. FUNCTIONS_________________________________________________________________________________________________________________________
	void io_config(void){ //setup io
		TRISF = 0b00000000;
		TRISA = 0b11101111;
		LATA = 0b00010000;
	}

	void usart_config(void) {
		Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW,15); //baud rate 10k, receive interrupts on, asynchronous, 8bit
	}  

	void inter_config(void){
		INTCONbits.GIE = 1; // from example
		INTCONbits.PEIE = 1; // from example
		PIE1bits.RC1IE = 1; // from example
	}

    void interrupt isr(void){
		if (PIR1bits.RC1IF == 1) {
			PIR1bits.RC1IF = 0; //clears the interrupt flag
		}
        LATF = Read1USART(); //displays usart value on LEDS
	}
	
//6. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged	


