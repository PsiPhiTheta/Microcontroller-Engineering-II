//0. INFORMATION & NOTES_________________________________________________________________________________________________________________

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 3			/
	// Exercise No. 1		/
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

//2. DEFINITIONS________________________________________________________________________________________________________________________
	#define LAB_switchnum 1                                                     //change during lab, 0 = LSB S1, 1 = MSB S2
    #define LAB_LEDnum 1                                                        //change during lab, 0 = LSB L1, 1 = MSB L2
    #define LAB_7segnum 1                                                       //change during lab, 0 = U1, 1 = U2
    #define LAB_time 1000                                                       //change during lab, = time in ms
    #define LAB_PBi 0                                                           //change during lab, PBi = PB1
    #define LAB_PBj 1                                                           //change during lab, PBi = PB2
    //#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTION PROTOTYPES_______________________________________________________________________________________________________________
    void test_routine(void);
    void configure_PB2_interrupt(void);
    void enable_global_interrupts(void);
    unsigned char PB2_pressed(void);
    void interrupt isr(void);
    unsigned char read_switches_Sj(void);
    void display_on_LEDs(unsigned char val);

//4. MAIN______________________________________________________________________________________________________________________________
    void main(void) {
       
        // setup IO
        ADCON1 = 0x0F; // enable digital I/O

        LATF = 0x00; // Turn all LEDs off    
        TRISF = 0x00; // Port F - LEDs/7seg display all outputs
        
        LATH = 0b11111111; // Turn off 7segs
        TRISC = 0b11111111; // Port C bits all inputs - C2-C5 are switches 0-3
        TRISH = 0b11111100; // Port H bits - H4-H7 are switches 4-7
        
        LATA = 0b00010000; // write to turn on LED transistor 
        TRISA = 0b11101111; // transistor enable as output
                
        // test
        test_routine();
        
        // run
        OpenADC(ADC_FOSC_16 & ADC_0_TAD, ADC_CH0 & ADC_INT_ON & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0x0E);
        
        enable_global_interrupts();
        configure_PB2_interrupt();
        
        while(1)
            ;
    }

//5. FUNCTIONS_________________________________________________________________________________________________________________________
    void test_routine(){
        LATF = 0x00; // switch all leds off
        Delay10KTCYx(250); //1s delay
        LATF = 0xFF; // switch all leds on
        Delay10KTCYx(250); //1s delay
        LATF = 0x00; // switch all leds off
    }
    
    void configure_PB2_interrupt(void) {
        OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_OFF); 
    }
    
    void enable_global_interrupts(void) {
        INTCON = 0xC0; //set high PEIE and GIE bits of INTCON
    }

    unsigned char PB2_pressed(void) {
        return (!PORTBbits.RB0);
    }
    
    void interrupt isr (void) {
        unsigned char sw_val = read_switches_Sj();
        while (PB2_pressed()) {
            display_on_LEDs (sw_val);
        }
        LATF = 0x00; // switch all LEDs off
        INTCON = INTCON & 0xFE; //clear interrupt flag
    }

    unsigned char read_switches_Sj() {
        if (LAB_switchnum) { //MSB
            unsigned char x;
            TRISH=0xF0;
            x = PORTH >> 4;
            return x;
        }
        else { //LSB
            unsigned char x;
            TRISC=0xFF;
            x = PORTC >> 2;
            return x;
        }
    }
    
    void display_on_LEDs(unsigned char val) {
        if (LAB_LEDnum) { //MSB
            LATF = val << 4;
        }
        else { //LSB
            LATF = val;
        }
    }

//6. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged	

