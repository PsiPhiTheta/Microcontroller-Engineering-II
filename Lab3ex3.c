//0. INFORMATION & NOTES_________________________________________________________________________________________________________________

	/////////////////////////
	// Lab assessment ME II	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	// Lab No. 3			/
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

//2. DEFINITIONS________________________________________________________________________________________________________________________
	#define LAB_7segnum 0                                                       //change during lab, 0 = U1, 1 = U2
    #define LAB_PBij 0                                                          //change during lab, 0 = PB1, 1 = PB2
    #define LAB_time 1                                                          //change during lab, = time in s
    //#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTION PROTOTYPES_______________________________________________________________________________________________________________
    void test_routine(void);
    void configure_PB2_interrupt(void);
    void enable_global_interrupts(void);
    void interrupt isr(void);
    void timer1_count (void); 
    unsigned char PB1_pressed (void);
    unsigned char PB2_pressed(void);
    void display_on_7seg(unsigned char val);
    unsigned char int_to_SSD (unsigned char val);

//4. MAIN______________________________________________________________________________________________________________________________
    unsigned char SSDnum;
    unsigned char start;
    
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
        OpenTimer1 (TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_INT & T1_PS_1_8 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);

        WriteTimer1(65000);
        
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
        LATA = 0b00000000; //disable LEDs
    }
    
    void configure_PB2_interrupt(void) {
        OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_OFF); 
    }
    
    void enable_global_interrupts(void) {
        INTCON = 0xC0; //set high PEIE and GIE bits of INTCON
    }

    void interrupt isr (void) {                                                 
        if (LAB_PBij == 1) { //chooses which PB to start and stop               
            if (PB2_pressed()) {                                                
                start = 1;     //start = true                                       
            }
            if (PB1_pressed()) {
                start = 0;    //start = false
            }
        }
        else {
            if (PB1_pressed()) {                                                       
                start = 1;     //start = true                                               
            }
            if (PB2_pressed()) {
                start = 0;    //stop = false
            }
        }
        
        if (start != 0) {  //checks if start true                                                   
            if (PIR1bits.TMR1IF){           //from example program 4                                    
                PIR1bits.TMR1IF = 0;        //from example program 4                                       
                timer1_count();             //from example program 4                                       
            }
            else if (PIR1bits.ADIF){                                            
                PIR1bits.ADIF = 0;          //from example program 4                                       
                display_on_7seg (SSDnum); 
                SSDnum++;
                if (SSDnum == 16) {
                    SSDnum = 0;
                }
            }
        }
        else {
            LATF = 0xFF; // switch all 7segs off
        }
        
        INTCON = INTCON & 0xFE; //clear interrupt flag
    }

    void timer1_count (void){                                                   
        static unsigned int loop_num = 0; //initiate loop number                
        WriteTimer1(65000);  //from program 4                                                     
        loop_num++;
        if (loop_num == (400*LAB_time)) //delay by 1s * LAB_time
        {
            loop_num = 0;
            ConvertADC();                                                      
        }
    }
    
    unsigned char PB1_pressed(void) {
        return (!PORTJbits.RJ5);
    }
    
    unsigned char PB2_pressed(void) {
        return (!PORTBbits.RB0);
    }
    
    void display_on_7seg(unsigned char val) {
        val = int_to_SSD(val);
        if (LAB_7segnum == 1) {
            LATH = 0b11111101; //U2
            LATF = val;
        }
        else {
            LATH = 0b11111110; //U1
            LATF = val;
        }
    }
    
    unsigned char int_to_SSD (unsigned char val){
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

//6. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged	

