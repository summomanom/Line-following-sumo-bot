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
    void ensure_whitespace(void);
            
   //variables for speed modifier of the motors to allow for straighter movement
    int fast_right_wheel=-37;
    int medium_left_wheel=17;
    int slow_left_wheel=21;
    
    int wait_for_8cm = 10000;  //for half battery
    //int wait_for_8cm = 45536;  // for full battery    
    int turn_around_time =9286; // for half battery    
    //int turn_around_time = 15286; // for full battery

    void motor_control(void)
    {
        // Check the values of all the sensors to determine what the robot should be doing.
         switch(SeeLine.B)
         {
          //If multiple sensors on the left side are detected move into these cases for right hand turns
             case 0b11000u:
             case 0b11100u:
         //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(32, 0);
          //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
                 if(SeeLine.B == 0b00000u)turn_left2centre();
                 break;
             
                 //if one side gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b00011u:
             case 0b00111u:
               //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(32, 0); 
              //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
                 
                 //if the edge and a center gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b00101u:
             case 0b01001u:
             case 0b01101u:
               //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(32, 0);   
                //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
                 if(SeeLine.B == 0b00000u)turn_right2centre();
                 break;
             
                 //if the edge and a center gets triggered continue forward. if white space is found turn tell centered on a line
             case 0b10100u:
             case 0b10110u:
             case 0b10010u:
            //Call check for whitespace to open up a small timer to allow the robot to cross an intersection. if the robot sees all white it will break out of the timer.
                 check_for_whitespace(32, 0);   
            //if the robot is on all white after the timer ends we know we passed a turn and must turn nutil centered on a line
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
    //if the robot detects all white we need to check if there is a gap or a need to turn around          
            case 0b00000u:
            {
                //small timer to make sure it is a gap
               ensure_whitespace();
               //move forward 8cm
               if (SeeLine.B == 0b00000u)
               {
               OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_32);
                 TMR0IF = 0;

                 WriteTimer0(wait_for_8cm);

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

                 WriteTimer0(turn_around_time);

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
             //if we detect all blak we need to check if it is an intersection or if it is a landing pad
            case 0b11111u:
            {
                //open a small timer to force the robot to follow curves 
                 OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_8);
                TMR0IF = 0;
                 WriteTimer0(0);
                 while(TMR0IF == 0)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                }
                ensure_whitespace();
                //after the short timer if the robot still ses all black we must stop on the landing pad 
                if (SeeLine.B == 0b11111u)
                     motors_brake_all();
                 
            }
            break;
            default:   
                follow_simple_curves();
                break;

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
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                
            case 4:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_4);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                 
                 case 8:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_8);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;

                 case 16:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_16);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                
                 case 32:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_32);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                
                 case 64:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                 case 128:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_128);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;
                 case 256:
                OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_256);
                TMR0IF = 0;
                 WriteTimer0(write_time);
                 while(TMR0IF == 0 && SeeLine.B != 0b00000u && SeeLine.B != 0b11111u)
                 {
                   follow_simple_curves(); 
                   check_sensors();
                     set_leds();
                 }
                 CloseTimer0();
                 ensure_whitespace();
                 break;                 
        }
    } 
    
//if the sensors go blamk we need to move forward to ensure that it is actually whitespace and not our sensors mising the line for a second
    void ensure_whitespace(void)
    {
        OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_8);
                 TMR0IF = 0;
                 WriteTimer0(45000);
                 while(TMR0IF == 0 )
                 {
                   follow_simple_curves();  
                   check_sensors();
                     set_leds();
                 }
    }        

//updated the follow curves code provided by coombes to allow for more adjustments along the path. to stop the robot from going too far in one direction
    void follow_simple_curves(void)
    {
         if ( SeeLine.B == 0b00100u || SeeLine.B == 0b10010u || SeeLine.B == 0b01001u) straight_fwd_fast();
         else if (SeeLine.B == 0b10000u) spin_left_fast();
         else if (SeeLine.B == 0b01000u || SeeLine.B == 0b11000u ) turn_left_medium();
         else if (SeeLine.B == 0b00010u || SeeLine.B == 0b00011u ) turn_right_medium();
         else if (SeeLine.B == 0b00001u) spin_right_fast();
         else if (SeeLine.B == 0b01100u) turn_left_medium();
         else if(SeeLine.B == 0b00110u) turn_right_medium();
         else if(SeeLine.B == 0b10100u ) turn_right_medium();
         else if(SeeLine.B == 0b00101u) turn_left_medium();
    }    

    void turn_left2centre(void)
    {
                 while(SeeLine.B != 0b00100u)
                 {
                     spin_left_medium();
                     check_sensors();
                     set_leds();
                 }                            
    }

    void turn_right2centre(void)
    {
                 while(SeeLine.B != 0b00100u){
                     spin_right_medium();
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
      set_motor_speed(left, slow, 0); 
      set_motor_speed(right, fast, fast_right_wheel); 
    }

    void turn_left_medium(void)
    {
      set_motor_speed(left, medium, 0); 
      set_motor_speed(right, fast, fast_right_wheel); 
    }

    void turn_left_slow(void)
    {
      set_motor_speed(left, slow, 0); 
      set_motor_speed(right, medium, 0); 
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
      set_motor_speed(right, slow, 0); 
    }

    void turn_right_medium(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, medium, 0); 
    }

    void turn_right_slow(void)
    {
      set_motor_speed(left, medium, 0); 
      set_motor_speed(right, slow, 0); 
    }
