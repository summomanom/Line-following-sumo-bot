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
             //right 90 degrees
             case 0b00111u:
             case 0b01111u:
             case 0b00011u:
   
             {  
                 check_sensors();    
                 set_leds(); 
                 while(SeeLine.B == 0b00111u || SeeLine.B == 0b01111u || SeeLine.B == 0b00011u)
                 {
                     while(SeeLine.B != 0b00000u)
                     {
                     straight_fwd_fast();
                     check_sensors();    
                     set_leds(); 
                     }
                     check_sensors();
                     set_leds();
                 }
                 
                     while(SeeLine.B == 0b00000u || SeeLine.B ==0b00001u|| SeeLine.B ==0b00101u ||
                             SeeLine.B == 0b00100u||SeeLine.B == 0b00110u||SeeLine.B == 0b00011u)
                     {
                         
                         spin_right_fast();
                         check_sensors();    
                         set_leds(); 
                     }
             }
                 
               
            break;
            //left 90 degrees
             case 0b11100u:
             case 0b11000u:
             case 0b11110u:
             { 
                check_sensors();    
                set_leds(); 
                 while(SeeLine.B == 0b11100u || SeeLine.B == 0b11110u || SeeLine.B == 0b11000u)
                 {
                     while(SeeLine.B != 0b00000u)
                     {
                     straight_fwd_fast();
                     check_sensors();    
                     set_leds(); 
                     }
                     check_sensors();
                     set_leds();
                 }
                 
                     while(SeeLine.B == 0b00000u ||SeeLine.B == 0b00100u || SeeLine.B ==0b10000u||
                              SeeLine.B == 0b10100u ||SeeLine.B == 0b01100u||SeeLine.B == 0b11000u )
                         
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
         if ( SeeLine.b.Center ) straight_fwd_fast();
         else if (SeeLine.b.Left) spin_left_fast();
         else if (SeeLine.b.CntLeft) turn_left_medium();
         else if (SeeLine.b.CntRight) turn_right_medium();
         else if (SeeLine.b.Right) spin_right_fast();
    }


    //Spins in one spot to the left
    void spin_left_fast(void)
    {
      set_motor_speed(left, rev_fast, 0); 
      set_motor_speed(right, fast, 0); 
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
      set_motor_speed(right, fast, 0); 
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
