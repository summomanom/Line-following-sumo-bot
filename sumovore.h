// ********************************************************************
// this file sumovore.h is for use with the solarbotics sumovore
//   and the PIC18F4525 and microchip 18C compiler 


//  By Dan Peirce B.Sc.
//  For Kwantlen Polytechnic University 
//  rev. May 14, 2011 reversed order of bits in struct sensors
//  rev. May 14, 2011 senser_union B is changed to 5 bit bitfield
//  rev. March 4, 2011 declaration of threshold added
//  rev. Nov. 17 new function like macro's defined for LED's
//  rev. May 22, 2009 for brainboard 2
//  some comments revised March 5, 2007
//  rev. March 13, 2007
//  rev. March 2, 2007

// *******************************************************************

// rev. April 3, 2014 end of macro changed to =!a from =~a to avoid warning from new XC8 compiler
#define setLED1(a) PORTDbits.RD0=!a  // When a = ON or OFF, 
#define setLED2(a) PORTDbits.RD1=!a  //  setLEDn(ON) turns on LEDn
#define setLED3(a) PORTDbits.RD2=!a  //  setLEDn(OFF) turns off LEDn
#define setLED4(a) PORTDbits.RD3=!a  // a could also be any char or integer
#define setLED5(a) PORTDbits.RD4=!a  // but only the least significant
                                     // bit will be used.

#define set_all_LEDs(a)  PORTD=(((~a)&0b00011111)|(PORTD&0b11100000))
     // in set_all_LEDs(a) a can be any 5 bit value or variable
     // if a has more than 5 bit the most significant bits will
     // be ignored

#define    LeftIR  !PORTDbits.RD5   // RD5 is pin 28 
#define    RightIR !PORTDbits.RD6   // RD6 is pin 29
            // Note that the IR detectors output low when an object
            //     is detected so the not "!" of the pin is used
            //  in the define to maintain positive logic

#define    EnableRmotor  PORTCbits.RC2    // pin_c2  -- Enable Right motor  
#define    EnableLmotor  PORTCbits.RC1    // pin_c1  -- Enable Left motor   
#define    RmotorGoFwd   PORTCbits.RC5    // rev for bb2  -- Right motor forward 
#define    RmotorGoFwdCmp PORTEbits.RE0   // rev for bb2  -- Right motor forward complement
#define    LmotorGoFwd   PORTCbits.RC0    // rev for bb2  -- Left motor forward  
#define    LmotorGoFwdCmp PORTEbits.RE1   // rev for bb2  -- Left motor forward complement

#define    RLS_LeftCH0      ADC_CH0    // AN0  (left reflective line sensor)
#define    RLS_CntLeftCH1   ADC_CH1    // AN1  (center left reflective line sensor)
#define    RLS_CenterCH2    ADC_CH2    // AN2  (center reflective line sensor)
#define    RLS_CntRightCH3  ADC_CH3    // AN3  (center right reflective line sensor)
                    // Pin RA4 No connection 
#define    RLS_RightCH4     ADC_CH4    // AN4  (right reflective line sensor)
                                       // note PortA is only 6 bits!

#define YES       0b1        // used to turn on an individual bit        
#define NO        0b0        // used to turn off an individual bit
#define ON        0b1
#define OFF       0b0

#define AN0_AN4    0B1010  // used for configuration of PCFG3:PCFG0
                           // A/D port Configuration Control Bits
                           // these determine which pins are analog inputs
                           // see page 224 of PIC18F4525 datasheet

#define THRESHOLD_DEFAULT 512u

extern unsigned int threshold;    // this declaration makes it possible to change threshold in 
                                // other files that include sumovore.h (like main.c)
                                // added March 4, 2011
                               // note that "extern" is used because this variable is defined in
                               // sumovore.c and in c "extern makes this line a declaration rather 
                               // than a definition!
                               // This has been added so that students can change the contents of 
                               // threshold without making changes to either sumovore.c or sumovore.h
                               // which should be discouraged

struct sensors
{
    unsigned    Right:1;   // bit order rev May 14, 2011
    unsigned CntRight:1;
    unsigned   Center:1;
    unsigned  CntLeft:1;
    unsigned     Left:1;
    unsigned :3;
};

union sensor_union         // revised May 14, 2011
{                          // so that B is restricted to 5 valid bits
    struct
    {
        unsigned B:5;
        unsigned :3;
    };
    struct sensors b;
};

void initialization(void);  // defined in sumovore.c

unsigned int adc(unsigned char channel);  // defined in sumovore.c

enum motor_speed_setting { rev_fast, rev_medium, rev_slow, stop, slow, medium, fast };

enum motor_selection { left, right };

void set_motor_speed(enum motor_selection the_motor, enum motor_speed_setting motor_speed, int speed_modifier);
                 // defined in sumovore.c
void motors_brake_all( void );
void set_leds(void);
void check_sensors(void);
void LVtrap(void);

extern union sensor_union SeeLine;  // defn. is in sumovore.c

