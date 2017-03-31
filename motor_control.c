#include <stdio.h>
#include <p18f4525.h>
#include "sumovore.h"
#include "motor_control.h"
#include "interrupts.h"
#include "osc.h"
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
    
    //variables for speed modifier
    int fast_right_wheel=-37;
    int medium_left_wheel=17;
    int slow_left_wheel=21;

    void motor_control(void)
    {
         // very simple motor control
         switch(SeeLine.B)
         {

             //cases for tight angle turns to the right 
             case 0b00101u:
             case 0b01001u:
             case 0b01011u:
             case 0b00011u:
             {
                  OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_8);
                  WriteTimer0(0);
                 // continue forward as long as the edge / middle sensors are on 
                 while(SeeLine.B != 0b00000u && ReadTimer0() <= 15000u )
                 {
                     straight_fwd_medium();
                     check_sensors();    
                     set_leds(); 
                 }
                 //if the sensors change to show the two close ones we know we have a tight angle turn
                 if(SeeLine.B == 0b00000u )
                 {
                   //Now that we know that its a tight angle we want to continue until we see white space
                    
                     //after white space is seen begin to turn right until the robot is centered back onto a line
                     while(SeeLine.B != 0b00100u)
                     {
                         spin_right_medium(); 
                     check_sensors();    
                     set_leds(); 
                     }
                     
                 }
             }
                 break; 
                 
         
         
             //cases to check for small angles on the left of the robot
             case 0b10100u:
             case 0b10010u:
             {
                 //while the robot sees the far edge and center continue to go forward
                 while(SeeLine.B == 0b10100u || SeeLine.B == 0b10010u )
                 {
                     straight_fwd_medium();
                     check_sensors();    
                     set_leds(); 
                 }
                 //if the robot sees the center and closer led we know that we have found  a small angle turn 
                 if(SeeLine.B == 0b01100u || SeeLine.B == 0b11100u)
                 {
                     //Continue forward until we hit white space
                     while(SeeLine.B != 0b00000u)
                     {
                         straight_fwd_medium();
                     check_sensors();    
                     set_leds();
                     }
                     //once white space is found turn to the left until centered back on the line
                     while(SeeLine.B != 0b00100u)
                     {
                     spin_left_medium(); 
                     check_sensors();    
                     set_leds(); 
                     }
                     
                 }
             }
                 break; 
             
             case 0b00111u:
             case 0b01111u:
            
   
             {  
                 check_sensors();    
                 set_leds(); 
                 while(SeeLine.B == 0b00111u || SeeLine.B == 0b01111u || SeeLine.B == 0b00011u)
                 {
                     straight_fwd_medium();
                     check_sensors();    
                     set_leds(); 
                 }
                 
                     while(SeeLine.B == 0b00000u || SeeLine.B ==0b00001u||SeeLine.B == 0b00100u||SeeLine.B == 0b00110u||SeeLine.B == 0b00011u)
                     {
                         
                         spin_right_fast();
                         check_sensors();    
                         set_leds(); 
                     }
             }
                 
               
            break;
            //left 90 degrees
             case 0b11100u:
       
             case 0b11110u:
             { 
                check_sensors();    
                set_leds(); 
                 while(SeeLine.B == 0b11100u || SeeLine.B == 0b11110u || SeeLine.B == 0b11000u)
                 {
                     straight_fwd_medium();
                     check_sensors();    
                     set_leds(); 
                 }
                 
                     while(SeeLine.B == 0b00000u || SeeLine.B ==0b10000u||SeeLine.B == 0b00100u||SeeLine.B == 0b01100u||SeeLine.B == 0b11000u)
                         
                     {
                         
                         spin_left_fast();
                         check_sensors();    
                         set_leds(); 
                     }
             }
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
                            motors_brake_all();
                           break;
            default:       break;
          } 
    }

    void follow_simple_curves(void)
    {
         if ( SeeLine.b.Center ) straight_fwd_medium();
         else if (SeeLine.b.Left) spin_left_medium();
         else if (SeeLine.b.CntLeft) turn_left_medium();
         else if (SeeLine.b.CntRight) turn_right_medium();
         else if (SeeLine.b.Right) spin_right_medium();
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
