#include <xc.h>
#include "interrupts.h"



// void interrupt high_isr(void);

static unsigned char lvd_flag=0; 
 // defined as static so it can only be see in this file;
 // functions in other files cannot change lvd_flag.
 // When another function requires the value of lvd_flag
 // the lvd_flag_set() function defined in this file can be 
 // used to see if the flag is set.
 // This hiding of a variable, encapsulating it in a
 // c module is the method used in C to encapsulate variables 
 // (and so reduce program bugs).
 // In C++ one can create a class to encapsulate a variable but
 //  C does not support classes.
 // lvd_flag is zero when initiallized. It will become 1 if
 // a low voltage interrupt occures. It will be reset to 0 only
 // when reinitiallized by a reset of the PIC.


void interrupt high_isr(void)
{
    PIR2bits.HLVDIF = 0; // ensure interupt is clear  
    TRISA = 0xff; // PORTs set to high impedence
    TRISB = 0xff; // to reduce current requirements
    TRISC = 0xff;
    TRISD = 0xff;
    TRISE = 0x03;
    lvd_flag =1; // this sets the lvd flag
                 // lvd_flag_set() will return this
        // variables value so that the status of
        // lvd_flag can be found in main().
    PIE2bits.HLVDIE = 0; // disables HLVD interupt
            // When a low voltage condition exists
            // the interrupt will be called only once
            //  This is set to a 1 by openLVD() in HLVD.c.
    HLVDCONbits.HLVDEN = 0; // HLVDEN disabled to reduce power
}

// the actual lvd_flag has scope only in this file.
// This function allows other code to see the value of
//  the flag but not to change it.
// In C functions have external scope by default.
unsigned char lvd_flag_set(void)
{
    return lvd_flag;
}       
