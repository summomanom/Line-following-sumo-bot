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
                 //Open up a timer to allow the robot to move forward for enough time to pas over a 90 degree intersection 
                 OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_16);
                 TMR0IF = 0;
                 WriteTimer0(20000);
                 //if the timer ends or the robot senses all white we break out of the while loop to stop moving forward
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   straight_fwd_fast();  
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 //If the robot ends on all white we know we just passed a 90 degree turn and must turn
                 if(SeeLine.B == 0b00000u)turn_left2centre();
                 break;
             //If multiple sensors on the right side are detected move into these cases for right hand turns
             case 0b00011u:
             case 0b00111u:
                 //Open up a timer to allow the robot to move forward for enough time to pas over a 90 degree intersection 
                 OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_16);
                 TMR0IF = 0;
                 WriteTimer0(20000);
                 //if the timer ends or the robot senses all white we break out of the while loop to stop moving forward
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u)
                 {
                   straight_fwd_fast(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                   //If the robot ends on all white we know we just passed a 90 degree turn and must turn
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
           
          //if a single led lights up use the follow simple curves code provided by coombes   
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
