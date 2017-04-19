#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"

    void follow_simple_curves(void);
    void spin_left_fast(void);
    void spin_left_medium(void);
    void spin_left_slow(void);
    void turn_left_fast(void);
    void turn_left_medium(void);
    void turn_left_slow(void);
    void straight_fwd_fast(void);
    void straight_fwd_medium(void);
    void straight_fwd_slow(void);
    void turn_right_fast(void);
    void turn_right_medium(void);
    void turn_right_slow(void);
    void spin_right_fast(void);
    void spin_right_medium(void);
    void spin_right_slow(void);
    void turn_left2centre(void);
    void turn_right2centre(void);
    void check_for_whitespace(char prescaler, int write_time);
     //variables for speed modifier
    int fast_right_wheel=-37;
    int medium_left_wheel=17;
    int slow_left_wheel=21;

    void motor_control(void)
    {
         // very simple motor control
         switch(SeeLine.B)
         {
               //if one side gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b11000u:
             case 0b11100u:
                 check_for_whitespace(16, 20000);
                 if(SeeLine.B == 0b00000u)turn_left2centre();
                 break;
             
                 //if one side gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b00011u:
             case 0b00111u:
                 check_for_whitespace(16, 20000);   
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
                 
                 //if the edge and a center gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b00101u:
             case 0b01001u:
                 check_for_whitespace(32, 0);   
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
             
                 //if the edge and a center gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b10100u:
             case 0b10010u:
                 check_for_whitespace(32, 0);   
                 if(SeeLine.B == 0b00000u)turn_left2centre();
                 break;
           
            case 0b00100u:
            case 0b00010u:
            case 0b01000u:
            case 0b00001u:
            case 0b10000u:
                           //no breaks all above readings end up here
                           follow_simple_curves();
                           break;


            case 0b00000u:
            {
                //small timer to make sure it is a gap
               OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_16);
                 TMR0IF = 0;
                 WriteTimer0(35536);
                 while(TMR0IF == 0 )
                 {
                   follow_simple_curves();  
                   check_sensors();
                     set_leds();
                 }
               //move forward 8cm
               if (SeeLine.B == 0b00000u)
               {
               OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_32);
                 TMR0IF = 0;
                 WriteTimer0(25536);
                 while(TMR0IF == 0&&SeeLine.B == 0b00000u)
                 {
                   straight_fwd_fast();  
                   check_sensors();
                     set_leds();
                 }
               }
               //once reached 8cm turn around
               if (SeeLine.B == 0b00000u)
                 {
                 OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64);
                 TMR0IF = 0;
                 WriteTimer0(9286);
                 while(TMR0IF == 0&&SeeLine.B == 0b00000u)
                 {
                   spin_right_fast();  
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
               }
               
            }
                           
                           break;
            default:       break;
          } 
    }
    //moves forward until it hits white space
    void check_for_whitespace(char prescaler, int write_time)
    {
        //switch statement for each prescalar and timer value
        switch(prescaler)
        {
            case 1:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
            case 4:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_4);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
                 
                 case 8:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_8);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
                 
                 case 16:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_16);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
 
                 case 32:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_32);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
                 case 64:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
                 case 128:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_128);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;
                 case 256:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_256);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;                 
        }
    }
    

    void follow_simple_curves(void)
    {
         if ( SeeLine.b.Center ) straight_fwd_fast();
         else if (SeeLine.b.Left) spin_left_fast();
         else if (SeeLine.b.CntLeft) turn_left_medium();
         else if (SeeLine.b.CntRight) turn_right_medium();
         else if (SeeLine.b.Right) spin_right_fast();
    }
    //turn left until center LED is lit up
    void turn_left2centre(void)
    {
                 while(SeeLine.B != 0b00100u)
                 {
                     spin_left_fast();
                     check_sensors();
                     set_leds();
                 }
    }
    //turn right until center LED is lit up
    void turn_right2centre(void)
    {
                 while(SeeLine.B != 0b00100u)
                 {
                     spin_right_fast();
                     check_sensors();
                     set_leds();
                 }
    }


    //Spins in one spot to the left
    void spin_left_fast(void)
    {
      set_motor_speed(left, rev_fast, 0); 
      set_motor_speed(right, fast, fast_right_wheel); 
    }
    void spin_left_medium(void)
    {
      set_motor_speed(left, rev_medium, 0); 
      set_motor_speed(right, medium, 0); 
    }
    void spin_left_slow(void)
    {
      set_motor_speed(left, rev_slow, 0); 
      set_motor_speed(right, slow, 0); 
    }
    
    ///turns on one wheel to the left
    void turn_left_fast(void)
    {
      set_motor_speed(left, stop, 0); 
      set_motor_speed(right, fast, fast_right_wheel); 
    }
    void turn_left_medium(void)
    {
      set_motor_speed(left, stop, 0); 
      set_motor_speed(right, medium, 0); 
    }
    void turn_left_slow(void)
    {
      set_motor_speed(left, stop, 0); 
      set_motor_speed(right, slow, 0); 
    }
    
    //move in a straight line
    void straight_fwd_fast(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, fast, fast_right_wheel); 
    }
    void straight_fwd_medium(void)
    {
      set_motor_speed(left, medium, medium_left_wheel); 
      set_motor_speed(right, medium, 0); 
    }
    void straight_fwd_slow(void)
    {
      set_motor_speed(left, slow, slow_left_wheel); 
      set_motor_speed(right, slow, 0); 
    }
    //spin in one place to the right
    void spin_right_fast(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, rev_fast, 0); 
    }
    void spin_right_medium(void)
    {
      set_motor_speed(left, medium, 0); 
      set_motor_speed(right, rev_medium, 0); 
    }
    void spin_right_slow(void)
    {
      set_motor_speed(left, slow, 0); 
      set_motor_speed(right, rev_slow, 0); 
    }
    
    //spin on one wheel to the right
    void turn_right_fast(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, stop, 0); 
    }
    void turn_right_medium(void)
    {
      set_motor_speed(left, medium, 0); 
      set_motor_speed(right, stop, 0); 
    }
    void turn_right_slow(void)
    {
      set_motor_speed(left, slow, 0); 
      set_motor_speed(right, stop, 0); 
    }
