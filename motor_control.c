#include <stdio.h>
#include <p18f4525.h>
#include "sumovore.h"
#include "motor_control.h"
#include "interrupts.h"
#include "osc.h"
    void follow_simple_curves(void);
    void spin_left(void);
    void turn_left(void);
    void straight_fwd(void);
    void turn_right(void);
    void spin_right(void);


    void motor_control(void)
    {
         // very simple motor control
         switch(SeeLine.B)
         {
             case 0b00111u:
             case 0b01111u:
             case 0b00011u:
   
             {  
                 check_sensors();    // from sumovore.c
                 set_leds(); 
                 while(SeeLine.B == 0b00111u || SeeLine.B == 0b01111u || SeeLine.B == 0b00011u)
                 {
                     straight_fwd();
                     check_sensors();    // from sumovore.c
                     set_leds(); 
                 }
                 
                     while(SeeLine.B == 0b00000u || SeeLine.B ==0b00001u||SeeLine.B == 0b00100u||SeeLine.B == 0b00110u||SeeLine.B == 0b00011u)
                     {
                         
                         spin_right();
                         check_sensors();    // from sumovore.c
                         set_leds(); 
                     }
         }
                 
               
            break;
            
             case 0b11100u:
             case 0b11000u:
             case 0b11110u:
             { 
              check_sensors();    // from sumovore.c
                 set_leds(); 
                 while(SeeLine.B == 0b11100u || SeeLine.B == 0b11110u || SeeLine.B == 0b11000u)
                 {
                     straight_fwd();
                     check_sensors();    // from sumovore.c
                     set_leds(); 
                 }
                 
                     while(SeeLine.B == 0b00000u || SeeLine.B ==0b10000u||SeeLine.B == 0b00100u||SeeLine.B == 0b01100u||SeeLine.B == 0b11000u)
                         
                     {
                         
                         spin_left();
                         check_sensors();    // from sumovore.c
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
         if ( SeeLine.b.Center ) straight_fwd();
         else if (SeeLine.b.Left) spin_left();
         else if (SeeLine.b.CntLeft) turn_left();
         else if (SeeLine.b.CntRight) turn_right();
        else if (SeeLine.b.Right) spin_right();
    }


    //Spins in one spot to the left
    void spin_left(void)
    {
      set_motor_speed(left, rev_fast, 0); 
      set_motor_speed(right, fast, 0); 
    }
    
    ///turns on one wheel to the left
    void turn_left(void)
    {
      set_motor_speed(left, stop, 0); 
      set_motor_speed(right, fast, 0); 
    }
    
    //move in a straight line
    void straight_fwd(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, fast, -37); 
    }
    //spin in one place to the right
    void spin_right(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, rev_fast, 0); 
    }
    
    //spin on one wheel to the right
    void turn_right(void)
    {
      set_motor_speed(left, fast, 0); 
      set_motor_speed(right, stop, 0); 
    }
    
    
