
// File sumovore.c by Dan Peirce B.Sc.
 
// Kwantlen Polytechnic University 
// apsc1299

// rev. May 15&16, 2013 New function reset_codes() and many new comments for
//                      reset functins and LVD fucntions.
// rev. May 14, 2011 turned watchdog timer off
// rev. June 18, 2010 added LED error codes to traps in reset functions
// rev. April 30, 2010 to add reliability features (wdt,bor,lvd and stack overflow)
// rev. Nov. 17 2009 to make use of new LED macro's,
//        to incorparate #pragma lines and
//        to point to osc.h in Functions folder
// rev. june 2009 for dynamic braking
// rev. May 22, 2009 to refect changes for BB2
// rev. March 13, 2007
// rev. March 2, 2007
#pragma config WDT = OFF      // rev. May 14, 2011 **** watchdog timer off *****
                            // reset if the watchdog timer times out
#pragma config WDTPS = 8     // rev. April 30, 2010
#pragma config BOREN = ON // hardware enable BOR 
                            // rev. April 30, 2010
#pragma config BORV = 0   // BOR voltage set between
                          // 4.36 and 4.82
                          // rev. April 30, 2010
#pragma config STVREN = ON  // reset on a stack overflow
                            // rev. April 30, 2010

#pragma config OSC = INTIO67  // allows osc1 (pin 13) and osc2 (pin 14) to be used as inputs
                              // note there is a crystal attached to these pins on the 
                              // brainboard
#pragma config MCLRE = OFF
#pragma config LVP = OFF
// #pragma config lines must come before #include "sumovore.h" as sumovore.h redefines OFF!!!

#include <xc.h>
#include <stdio.h>
#include <reset.h>
#include "..\Common\osc.h"
#include "sumovore.h"


void openPORTCforPWM(void);
void openPORTCforUSART(void); 
void openPORTA(void);
void openPORTB(void);
void openPORTD(void);
void openPORTE(void);

void reset_codes(void);
void PORtask(void);
void BORtask(void);
void RESETtask(void);
void WDTtask(void);
void STKFULtask(void);
void openLVD(void);
void gtrap(void);

// union sensor_union SeeLine = 0;  // see note below April 3, 2014
union sensor_union SeeLine;  // rev. April 3, 2014 for XC8 new compiler did not allow old initialization
unsigned int threshold;    // value compared to adc result


void initialization(void)
{
    SeeLine.B = 0;          // rev. April 3, 2014 for XC8
    RCONbits.IPEN = 1;      // rev. April 30, 2010
    INTCONbits.GIEH = 0;    // rev. April 30, 2010
    INTCONbits.GIEL = 0;    // rev. April 30, 2010
    
    set_osc_32MHz();  // to change the internal oscillator frequency (see osc.h osc.c)
    openPORTCforUSART();

    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH,
             16 );            // for 19200 bit per second
                               // (32000000/115200/16)-1 = 16
                  // actual buad rate is 32000000/(16*(16+1)) = 117647 baud (note a 2% error in frequency)
      // see http://en.wikibooks.org/wiki/Serial_Programming/Typical_RS232_Hardware_Configuration#Oscillator_.26_Magic_Quartz_Crystal_Values


    openPORTD(); 
    PORTD = 0;  // TURN ALL LED'S OFF 
    
    reset_codes();   // determine and display code for cause of MCU reset. 

    openPORTCforPWM();

    openPORTA();
    openPORTB();    
    openPORTE();
   
     
    openLVD(); 
    
    

    OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_6_TAD , ADC_CH1 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, AN0_AN4);
// AN0-AN4 is defined in sumovore.h the others are defined in adc.h (C18 library) 

    RmotorGoFwd = NO;  // NO is defined as 0b0 in sumovore.h
    RmotorGoFwdCmp = NO;
    LmotorGoFwd = NO;
    LmotorGoFwdCmp = NO;
//  PWMperiod = [(period)+1]x 4 x Tosc x TMR2
//  period	Tosc    	TMR2Pre		pwm_period		freq
//  255	    3.13E-08	16  		5.12E-04		1.95E+03

    OpenTimer2(TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);  // TMR2 prescale is 16
    OpenPWM1(199);           // TPWM = (199+1)*4*(31.25 ns)*16
                             //      = 0.400 ms   or 2500 Hz
    OpenPWM2(199);
    SetDCPWM1(0);            // TDC  = 64*(31.25 ns)*16
                             //      = 0.032 ms
                             //      = 0% * TPWM  (800 will give 100%)
    SetDCPWM2(0);
   // threshold = THRESHOLD_DEFAULT; 

}

//***********************************************************************************
//                          openPORTCforUSART()
//***********************************************************************************
void openPORTCforUSART(void)
{
  TRISCbits.TRISC6 = 0;  // set TX (RC6) as output 
  TRISCbits.TRISC7 = 1;  // and RX (RC7) as input
}

//***********************************************************************************
//                          openPORTCforPWM()
//***********************************************************************************
void openPORTCforPWM(void)
{
    TRISCbits.TRISC0 = 0; // Direction Left M
    TRISCbits.TRISC1 = 0; // Enable Left M
    TRISCbits.TRISC2 = 0; // Enable Right M 
    TRISCbits.TRISC3 = 0; // I2C SCL
    TRISCbits.TRISC4 = 0; // I2C SDA
    TRISCbits.TRISC5 = 0; // Direction Right M
    // TRISC6 and TRISC 7 initialized in openPORTCforUSART()
}

//***********************************************************************************
//                          openPORTA()
//***********************************************************************************
void openPORTA(void)
{
    TRISA = 0B11101111; // RA0/AN0, RA1/AN1, RA2/AN2, RA3/AN3, RA5/AN4 SET AS INPUTS
                        // RA4 not used set as output
                        // bits RA6 and RA7 are left as inputs (crystal still attached 
                        //  on sumovore)
}

//***********************************************************************************
//                          openPORTB()
//***********************************************************************************
void openPORTB(void)
{
    TRISB = 0B11000000; // PORTB mostly not used
                        // reserve pins 39 (RB6/PGC) and 40 (RB7/PGD)
                        // as inputs to avoid conflict if ISP and PICkit2
}

//***********************************************************************************
//                          openPORTD()
//***********************************************************************************
void openPORTD(void)
{
    TRISD = 0b01100000; // RD7 not connected
    // RD6 is IR Right, RD5 is IR Left, RD4 is LED5, RD3 is LED4
    // RD2 is LED3, RD1 is LED2 and RD0 is LED1
}

//***********************************************************************************
//                          openPORTE()
//***********************************************************************************
void openPORTE(void)
{
    TRISE = 0b000; // all outputs 
                   // E0 and E1 are now used for motor direction and
                   // dynamic braking
                   // E2 is not used    
}



void set_motor_speed(enum motor_selection the_motor, enum motor_speed_setting motor_speed, int speed_modifier)
{
    const static int motor_speeds[] = { -800, -725, -650, 0, 650, 725, 800};
    int duty_cycle;
    enum e_direction {reverse,forward} dir_modifier= forward;

    duty_cycle = motor_speeds[ motor_speed ] + speed_modifier;
    if ( duty_cycle < 0 ) 
    {
        dir_modifier = reverse;
        duty_cycle = -1 * duty_cycle;
    }
    if ( duty_cycle > 800 ) duty_cycle = 800;

    if (the_motor == left)
    {
        SetDCPWM2((unsigned int) duty_cycle );
        if ( dir_modifier == reverse ) LmotorGoFwd = NO;
        else LmotorGoFwd = YES;
        LmotorGoFwdCmp = !LmotorGoFwd;
    }
    else
    {
        SetDCPWM1((unsigned int) duty_cycle );
        if ( dir_modifier == reverse ) RmotorGoFwd = NO;
        else RmotorGoFwd = YES;
        RmotorGoFwdCmp = !RmotorGoFwd;   
    }
}

void motors_brake_all( void )  // created june 26, 2009
{
    SetDCPWM1(800u ); // enable motors 100% for braking 
    SetDCPWM2(800u ); //
    LmotorGoFwdCmp = NO; // ground all direction lines
    LmotorGoFwd = NO;  // motor terminals will have dead short
    RmotorGoFwdCmp = NO;
    RmotorGoFwd = NO;
 
}

unsigned int adc(unsigned char channel)
{
    SetChanADC( channel );
    ConvertADC();
    while( BusyADC() );

    return ReadADC();    
}

// ****************************************************************
//                          check_sensors
// ****************************************************************
void check_sensors(void)
{
        SeeLine.b.Left = ( adc(RLS_LeftCH0) > threshold );      // adc() prototye in sumovore.h
        SeeLine.b.CntLeft = ( adc(RLS_CntLeftCH1) > threshold );   // adc() code in sumovore.c
        SeeLine.b.Center = ( adc(RLS_CenterCH2) > threshold );    //  ledx turns on when corresponding 
        SeeLine.b.CntRight = ( adc(RLS_CntRightCH3) > threshold );  //    reflective sensore sees a line
        SeeLine.b.Right = ( adc(RLS_RightCH4) > threshold );
}
// ******************************************************************


// ****************************************************************
//                          set_leds
// ****************************************************************
void set_leds(void)
{
        setLED1(SeeLine.b.Left);
        setLED2(SeeLine.b.CntLeft);
        setLED3(SeeLine.b.Center);
        setLED4 (SeeLine.b.CntRight);
        setLED5(SeeLine.b.Right);
}
// ****************************************************************

// ****************************************************************
// determine and display code for cause of MCU reset. 
// LED
// 3  4  5
// 0  0  0  Power on Reset  <POR>       This is the normal result of of flicking toggling the power switch
// 0  0  1  Brown Out Reset   <BOR>     PIC detected a voltage below about 4.6 volts that did not return above 4.6 volts for more than 200 micro seconds
// 0  1  0  Low Voltage Detect   <LVD>  PIC detected a voltage below 4.59 volts (actually not a reset but an interrupt)
// 0  1  1  <not defined> should not get this
// 1  0  0  WatchDog Timer reset  <WDT> These resets have been disabled 
//                                              in the #pragma config WDT = OFF directive (near top of this file) 
//                                              this code should not occure unless OFF is changed back to ON
// 1  0  1  Stack Overflow   <SCKFUL>   This is usually an indication of unitended recursion (program bug)
// 1  1  1  RESET task <reset>  (say a software reset -- this one has not been tested)
//
// LED 1 and 2 flash alternetly

void reset_codes(void)
{
    if( isPOR() ) PORtask();        // rev. April 30, 2010 
                                    //   Note that isPOR() is described on Page 145 of the MPlab C18 Library manual
                                    // This indicates power dropped to zero (e.g. power switch toggled)
    else if (isBOR() ) BORtask();   // this indicates a reset has occured due to low voltage for more than 200 micro seconds
                                    //   Note that isBOR() is described on page 144 of the MPlab C18 Library manual
    else if(isWDTTO() ) WDTtask();  // This indicates the watchdog timer has timed out and caused a reset
                                    //   Note that using it requires careful program design that is beyond the scope of apsc1299.
                                    //         isWDTTO() is descriped on page 145 of the MPlab C18 Library manual
                                    //
    else if(STKPTRbits.STKFUL) STKFULtask(); // The PIC has a 32 level hardware stack. 
                                             //  Once the limit is exceded a reset should occur. Typically caused by unitended
                                             //  recursion.

    else RESETtask();              // This never comes up but it would indicate a software reset
}
// *****************************************************************

// **PORtask()**
// control comes here if the power switch is toggled on and the PIC supply voltage is normal
//   If a <POR> is indicated at any other time it probably indicates an open circuit occured.
//   An open circuit could be caused by that a battery out of possition, the power switch or some loose connection.
// Unlike the other reset events control stays in this function for only a couple of seconds

void PORtask(void)  // rev. June 18, 2010
{
    unsigned long count1=0, count2=0;
    StatusReset();       // sets flags /POR and /BOR see page 146 of the MPlab C18 Library manual
                         
    printf("<POR>");
    setLED1(1);
    setLED2(0);
    setLED3(0);
    setLED4(0);
    setLED5(0);
    while(count1<10u)       //trap here for a couple of seconds and flash LEDs 1 and 2 alternately
    {
        CLRWDT();               // for watchdog timer (the watchdog timer is disabled for simple curve follower but enabled for
        if (count2==30000u)     //   the robot diagnostic program -- both use essentially the same sumovore.c file )
        {                       // clrWdt() is not described in the C18 Library manual. It is a macro defined in p18f4525.h 
            count2=0;           //   as inline assembly language code #define CLRWDT() {_asm clrwdt _endasm}
            count1++;           //  if/when the watchdog timer were/is enabled long duration while loops would require the WDT to 
            setLED1(1);         //    be cleared periodically in the loop to avoid a watchtimer time out and system reset
            setLED2(0);
        }  
        else if (count2==15000u) 
        {
            setLED1(0);
            setLED2(1);
        }  
        count2++;   
    }   
}    

// ** BORtask()**
// Control comes here only if a Brown Out Reset has occured since the last Power On Reset
//  Note that a brown out reset occurs if the supply voltage drops below 4.36 volts and 
//     does not return above 4.6 volts for more than 200 micro seconds (200 micro seconds is the
//     minimum specified in the datasheet and the exact time requried to trigger the reset varies from PIC to PIC). 
// The PIC is held in reset until the voltage is again above 4.6 volts (actual voltage can varry between the limits of 4.36 to 4.82). 
// No code is executed while the PIC is held in reset.
//  Unlike the POR the BOR is not a normal condition. 
//  This function indicates the BOR code and gets trapped in an endless loop since all values stored 
//  in volatile memory are possibly unreliable.
//  Repeated frequent BOR's indicate low batteries but one should check for a misaligned battery and check the battery voltage.
//  An individual single BOR could possibly be the result of a battery shifting in the holder.
void BORtask(void)  // rev. June 18, 2010
{
    StatusReset();       // sets flags /POR and /BOR
                         //  comment corrected Feb. 25, 2011
    printf("<BOR>"); 
    setLED1(0);
    setLED2(0);
    setLED3(0);
    setLED4(0);
    setLED5(1);
    gtrap();    // trap code here until POR
}

// ** WDTtask()**
// Control comes here only if a Watchdog timer time out occured which results in a reset.
// This feature of the MCU can be used to detect improper program execution that could be caused by a 
//  programming bug or some unknown cause. This feature is disabled at the top of this file but the code remains in case 
// someone wants to try it out. Also, it is enabled for the diagnostic program used for testing the robots (that program
// makes use of the same version of sumovore.c as the simple curve follower project except that the watchdog timer is enabled
// with #pragma config WDT = ON in the diagnostic project).
// Using the watchdog timer is beyond the scope of APSC1299.
void WDTtask(void)  // rev. April 30, 2010
{
    printf("<WDT TO>");
    setLED1(1);
    setLED2(0);
    setLED3(1);
    setLED4(0);
    setLED5(0);
    gtrap();    // trap code here until POR
}

// **STKFULtask()**
// The PIC has a 32 level hardware stack. The C compiler sets up this stack for
//  use in fucntion calls. Normally 32 levels is more than sufficient for our PIC
//  programs and this reset is probably an indication of unintended recursion (this
//  has been the case in all instances Dan is aware of since we enabled this in 
//  2010. It is just possible that someone could use so many levels of function calls
//  that the PIC's limit would be exceeded and this error would occur. Whatever the cause
//  once the limit is exceded a reset should occur.
void STKFULtask(void)    // rev. April 30, 2010
{
                           // An error on the hardware stack
    STKPTRbits.STKFUL = 0; //  caused a reset!
    printf("<STKFUL>");    //  continue 
    setLED1(1);
    setLED2(0);
    setLED3(1);
    setLED4(0);
    setLED5(1);
    gtrap();    // trap code here until POR    
}    

// **RESETtask()**
// The PIC does have a software reset instruction. It is not expected to be used in these programs.
// This function would indicate if a software reset did occur.
void RESETtask(void)    // rev. April 30, 2010
{

    printf(" <reset> ");     
    setLED1(1);
    setLED2(0);
    setLED3(1);
    setLED4(1);
    setLED5(1);
    gtrap();    // trap code here until POR
}   

// **openLVD()**
// this function sets up the low voltage
//  detect feature of the PIC 
//   sets HLVDIP to a high priority interrupt.
//  The use of interrupts is beyond the scope of APSC1299.
// Unlike any of the resets an interrupt results in an interrupt handling routine to be called. In general
//   an interrupt handling routine executes quickly and returns. 
// The interrupt routine for the LVD is defined in interrupts.c and it resets all the IO ports as inputs. 
//    This will reduce the load on the batteries in an attempt to avoid a BOR. It also sets a flag lvd_flag.
//    lvd_flag is checked in main() each time through the while loop to see if a low voltage has occured.
//  
void openLVD(void)
{
    RCONbits.IPEN = 1;
    HLVDCONbits.HLVDEN = 1; // HLVDEN enabled

    IPR2bits.HLVDIP = 1; // sets HLVD to high priority
    HLVDCONbits.VDIRMAG = 0; // interrupt occures if voltage below
                             //  trip point
    HLVDCONbits.HLVDL3 = 1; // set to about 4.59 volts
    HLVDCONbits.HLVDL2 = 1; 
    HLVDCONbits.HLVDL1 = 1;
    HLVDCONbits.HLVDL0 = 0;                                
    PIR2bits.HLVDIF = 0; // ensure interrupt is clear                           
    PIE2bits.HLVDIE = 1; // enables HLVD interrupt
    INTCONbits.GIEH = 1; // globle enable of interrupts
}    

// **LVtrap()**
// As described in the comments for openLVD() a low voltage detection causes an interrupt and the interrupt
//   service routine sets a flag lvd_flag. This is detected in main() and once the lvd_flag is detected as set
//   control comes here. This function has been setup much like the functions that handle different reset conditions.
void LVtrap(void)
{
    printf("\\<LVD>");
    openPORTD();  // set as outputs for LED's (required because the interrupt service routine set all ports as inputs)
    setLED1(1);
    setLED2(0);
    setLED3(0);
    setLED4(1);
    setLED5(0);
    gtrap();    // trap code here until POR  
}

// **gtrap**
// called by reset functions (except POR) and by LVtrap(). Flashes LED1 and LED2 alternetly forever.
// Once here a reset is required to get out of this function.
void gtrap(void)
{
    unsigned long count=0;
    
    while(1)       //trap here
    {
        CLRWDT(); 
        if (count==30000u) 
        {
            count=0;
            setLED1(1);
            setLED2(0);
        }  
        else if (count==15000u) 
        {
            setLED1(0);
            setLED2(1);
        }  
        count++;   
    }
}
