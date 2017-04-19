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
//variables for speed modifier of the motors to allow for straighter movement
    int fast_right_wheel=-37;
    int medium_left_wheel=17;
    int slow_left_wheel=21;

    void motor_control(void)
    {
         // Check the values of all the sensors to determine what the robot should be doing.
         switch(SeeLine.B)
         {
       //If multiple sensors on the left side are detected move into these cases for right hand turns
             case 0b11000u:
             case 0b11100u:
                 //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(16, 20000);
                 //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
                 if(SeeLine.B == 0b00000u)turn_left2centre();
                 break;
      //If multiple sensors on the left side are detected move into these cases for right hand turns
             case 0b00011u:
             case 0b00111u:
                 //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(16, 20000);   
                 //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
           
            case 0b00100u:
            case 0b00010u:
            case 0b01000u:
            case 0b00001u:
            case 0b10000u:
                           //no breaks all above readings end up here
                           follow_simple_curves();
                           break;

    //if no leds are lit up brake all the motors.
            case 0b00000u:
                           motors_brake_all();
                           break;
            default:       break;
          } 
    }
    //function that can be called wth differnt scalers and write time values that causes the robot to move forward until a certain time has passed or all white is deteccted.
    void check_for_whitespace(char prescaler, int write_time)
    {
        switch(prescaler)
        {
            case 1:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
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
                   straight_fwd_fast(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 break;                 
        }
    }
    
//Code provided by Coombes. Adjusts the robot motors to operate at different speeds so it can adjust on a slightly curved path
    void follow_simple_curves(void)
    {
         if ( SeeLine.b.Center ) straight_fwd_fast();
         else if (SeeLine.b.Left) spin_left_fast();
         else if (SeeLine.b.CntLeft) turn_left_medium();
         else if (SeeLine.b.CntRight) turn_right_medium();
         else if (SeeLine.b.Right) spin_right_fast();
    }
    
    void turn_left2centre(void)
    {
                 while(SeeLine.B != 0b00100u)
                 {
                     spin_left_fast();
                     check_sensors();
                     set_leds();
                 }
    }
    void turn_right2centre(void)
    {
                 while(SeeLine.B != 0b00100u)
                 {
                     spin_right_fast();
                     check_sensors();
                     set_leds();
                 }
    }


  
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
