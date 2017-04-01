//0. INFORMATION & NOTES_________________________________________________________________________________________________________________

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 4			/
	// Exercise No. 3		/
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
    #define delay 1 															//change in lab
	#define baud_rate 7000														//change in lab
	#define n1  1																//change in lab
	#define n2  2																//change in lab
	#define n3  3																//change in lab
	#define n4  4																//change in lab
	#define n5  5																//change in lab
	#define n6  6																//change in lab
	#define n7  7																//change in lab
	#define n8  8																//change in lab
	#define n9  9																//change in lab
	#define n10  10																//change in lab
	#define n11  11																//change in lab
	#define n12  12																//change in lab
	#define n13  13																//change in lab
	#define n14  14																//change in lab
	#define n15  15																//change in lab
	#define n16  16																//change in lab
	//#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTION PROTOTYPES_______________________________________________________________________________________________________________
	void io_config(void);
	void usart_config(void);
	void inter_config(void);
	void interrupt isr(void);
	void Timer1(void);

//4. MAIN______________________________________________________________________________________________________________________________
	int timer_counter = 0;
	int count = 0;
	int x = 0;

	void main(){
	
	usart_config();
    
    inter_config();
     
    io_config();

    WriteTimer1(64755);

    while(1);
	}

//5. FUNCTIONS_________________________________________________________________________________________________________________________
	void io_config(void){ //setup io
		TRISF = 0b00000000;
		TRISA = 0b11101111;
		LATA = 0b00010000;
	}

	void usart_config(void) {
		OpenTimer1 (TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_INT & T1_PS_1_8 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
		Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, ((10000000/baud_rate/64)-1));
	}  

	void inter_config(void){
		INTCONbits.GIE = 1;
		INTCONbits.PEIE = 1;
		PIE1bits.RC1IE = 1;
	}

    void interrupt isr(void){
		if(PIR1bits.RC1IF == 1) {
			PIR1bits.RC1IF = 0;
			LATF = Read1USART();
		}
    
		if (PIR1bits.TMR1IF == 1) {
			PIR1bits.TMR1IF = 0;
			Timer1();
		}
	}

	
	void Timer1 (void) {
	
    WriteTimer1(64755);
    timer_counter++;
    if (timer_counter >= 400*delay){
        x++;
        timer_counter = 0;
        while(Busy1USART());
        switch (x){
            case 1: count = n1;
            break;
            case 2: count = n2;
            break;
            case 3: count = n3;
            break;
            case 4: count = n4;
            break;
            case 5: count = n5;
            break;
            case 6: count = n6;
            break;
            case 7: count = n7;
            break;
            case 8: count = n8;
            break;
            case 9: count = n9;
            break;
            case 10: count = n10;
            break;
            case 11: count = n11;
            break;
            case 12: count = n12;
            break;
            case 13: count = n13;
            break;
            case 14: count = n14;
            break;
            case 15: count = n15;
            break;
            case 16: count = n16;
            break;
            default: count = 0;
            break;
        }
        
        if (x == 16){
            x = 0;
		}
    }
    Write1USART(count);
}

	
//6. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged	










