/*
 * shapes.c
 *
 *  Created on: Feb 13, 2026
 *  Author: Kareem Elkassem
 *  This file contains the functions for shape movements
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"



// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

unsigned int wheel_move;
char forward;

unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
unsigned int delay_start = 0;
unsigned int right_motor_count = 0;
unsigned int left_motor_count = 0;
unsigned int segment_count = 0;
unsigned int direction = 0;
unsigned int sw1_position;
unsigned int sw2_position;
unsigned int okay_to_look_at_switch1;
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;
unsigned int okay_to_look_at_switch2;
// unsigned char event;
unsigned char state;
 unsigned int figure8count;
 unsigned int phase;
 unsigned int sidecount;
 unsigned int counter;
 unsigned int timecount;




// RUN STRAIGHT
//void Run_Straight(void){
//  switch(state){
//  case  WAIT:
//  wait_case();
//  // Begin
//  break;                           //
//  case  START:                       // Begin
//  start_case();
//  break;                           //
//  case  RUN:                         // Run
//  run_case();
//  break;                           //
//  case  END:                         // End
//  end_case();
//  break;                           //
//  default: break;
//  }
//  }
//// IDLE CASE FOR ALL SHAPES
//
////void idle_case(void){
////    if(event == CIRCLE || event == FIGUREEIGHT|| event == TRIANGLE){
////            state = WAIT;
////        }
//// }
//
//// WAIT CASE FOR ALL SHAPES
////void wait_case(void){
////  if(time_change){
////  time_change = 0;
////  if(delay_start++ >= WAITING2START){
////  delay_start = 0;
////  state = START;
////  }
////  }
////  }
//////START CASE FOR ALL SHAPES
////void start_case(void){
////  cycle_time = 0;
////  right_motor_count = 0;
////  left_motor_count = 0;
////  Forward_Move();
////  segment_count = 0;
////  state = RUN;
////  direction = 0;
////  }
//
//// RUN CASE FOR SHAPES
//void run_circle_case(void){ // FOR GOING IN A CIRCLE
//    if(time_change){
//      time_change = 0;
//
//      // Turn both motors ON every cycle
//
//      // Different timing creates curve
//      if(right_motor_count++ >= RIGHT_COUNT_TIME){
//        P6OUT &= ~R_FORWARD;
//      }
//
//      if(left_motor_count++ >= LEFT_COUNT_TIME){
//        P6OUT &= ~L_FORWARD;
//      }
//
//      if(cycle_time++ >= WHEEL_COUNT_TIME){
//        cycle_time = 0;
//        right_motor_count = 0;
//        left_motor_count = 0;
//        segment_count++;
//        Forward_Move();
//      }
//
//      // Complete two circles
//      if(segment_count >= CIRCLE_DISTANCE * 2){
//        state = END;
//      }
//    }
//  }
//
//void run_figure8_case(void){
//    if(time_change){
//        time_change = 0;
//
//    if(direction == 0){
//        if(right_motor_count++ >= RIGHT_COUNT_TIME) P6OUT &= ~R_FORWARD;
//        if(left_motor_count++ >= LEFT_COUNT_TIME) P6OUT &= ~L_FORWARD;
//    }else{
//        if(right_motor_count++ >= LEFT_COUNT_TIME) P6OUT &= ~R_FORWARD;
//        if(left_motor_count++ >= RIGHT_COUNT_TIME) P6OUT &= ~L_FORWARD;
//    }
//    if(cycle_time++ >= WHEEL_COUNT_TIME){
//            cycle_time = 0;
//            right_motor_count = 0;
//            left_motor_count = 0;
//            segment_count++;
//            Forward_Move();
//    }
//    if(segment_count >= (CIRCLE_DISTANCE-2 ) && direction == 0){
//            direction = 1;          // switch curve direction
//            segment_count = 0;      // reset counter for second circle
//    }
//    else if(segment_count >= (CIRCLE_DISTANCE +6 ) && direction == 1){
//        figure8count++;
//    segment_count=0;
//    direction=0;
//    if(figure8count >=2){
//        state = END;
//    }
//    else{
//        Forward_Move();
//    }
//    }
//    }
//}
//void run_triangle_case(void){
//    if(time_change){
//        time_change = 0;
//
//        if(phase==0){ // MOVING STRAIGHT
//          P6OUT |= R_FORWARD;
//          P6OUT |= L_FORWARD;
//        if(right_motor_count++ >= (TRIANGLE_COUNT_TIME_RIGHT)) P6OUT &= ~R_FORWARD;
//        if(left_motor_count++ >= (TRIANGLE_COUNT_TIME_LEFT)) P6OUT &= ~L_FORWARD;
//        if(cycle_time++ >= SIDE_DISTANCE){
//            cycle_time = 0;
//            right_motor_count = 0;
//             left_motor_count = 0;
//             phase = 1;
//             Forward_Move();
//
//        }
//        }
//        if(phase == 1){  // TURNING
//            P6OUT ^= GRN_LED;
//            if(right_motor_count++ >= RIGHT_COUNT_TIME+18)
//                    P6OUT &= ~R_FORWARD;
//
//                if(left_motor_count++ >= LEFT_COUNT_TIME -5)
//                    P6OUT &= ~L_FORWARD;
//
//                if(cycle_time++ >= TURN_TIME){
//
//                    cycle_time = 0;
//                    right_motor_count = 0;
//                    left_motor_count = 0;
//
//                    sidecount++;
//
//                    if(sidecount >= 6){
//                        phase = 0;
//                        sidecount=0;
//                        state = END;
//                    }
//                    else{
//                        phase = 0;
//                        Forward_Move();
//                    }
//                }
//            }
//
//      }
//    }


//void run_case(void){ // FOR GOING STRAIGHT
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= TRAVEL_DISTANCE){
//            if(right_motor_count++ >= RIGHT_COUNT_TIME){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= LEFT_COUNT_TIME){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time++ >= WHEEL_COUNT_TIME){
//                cycle_time = 0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//        }else{
//            state = END;
//        }
//    }
//      }
//  //END CASE FOR ALL SHAPES
//void end_case(void){
//  Forward_Off();
//  segment_count = 0;
//  state = IDLE;
//  event = NONE;
//  }
//
//
////STATE MACHINES FOR CIRCLE FIGURE 8 AND TRIANGLE
//void Run_Circle(void){
//      switch(state){
//      case IDLE:
//          idle_case();
//          break;
//      case WAIT:
//          wait_case();
//          break;
//      case START:
//          start_case();
//          break;
//      case RUN:
//          run_circle_case();
//          break;
//      case END:
//          end_case();
//          break;
//      default: break;
//          }
// }
//void Run_Figure8(void){
//    switch(state){
//      case IDLE:
//       idle_case();
//        break;
//        case WAIT:
//        wait_case();
//        break;
//        case START:
//        start_case();
//        break;
//        case RUN:
//        run_figure8_case();
//        break;
//        case END:
//         end_case();
//         break;
//         default: break;
//         }
//}
//void Run_Triangle(void){
//    switch(state){
//      case IDLE:
//      idle_case();
//      break;
//      case WAIT:
//      wait_case();
//      break;
//      case START:
//      start_case();
//      break;
//      case RUN:
//      run_triangle_case();
//      break;
//      case END:
//      end_case();
//      break;
//      default: break;
//             }
//
//}
//
//// SWITCHES
//void Switches_Process(void){
//    Switch1_Process();
//    Switch2_Process();
//}
//
//void Switch1_Process(void){
//    if (!(P4IN & SW1)&& okay_to_look_at_switch1) {
//        // SW1 is physically pressed
//
//
//                sw1_position = PRESSED;      // 0
//                if(counter == 0){
////                        event = CIRCLE;
//                    }
//                    else if(counter == 1){
////                        event = FIGUREEIGHT;
//                    }
//                    else if(counter == 2){
//                        event = TRIANGLE;
//                    }
//
//                    counter++;           // move to next shape
//
//                    if(counter > 2){     // reset after triangle
//                        counter = 0;
//                    }
//             // trigger the circle event
//                state = IDLE;
//
//                // Start debounce lockout
//                okay_to_look_at_switch1 = NOT_OKAY;   // 0
//                count_debounce_SW1 = 0;
//
//        }
//
//        // ----------- DEBOUNCE HANDLING -----------
//        if (okay_to_look_at_switch1 == NOT_OKAY) {
//
//            if (count_debounce_SW1++ >= DEBOUNCE_TIME) {
//
//                // After debounce time, allow new presses
//                okay_to_look_at_switch1 = OKAY;       // 1
//
//                // Update release state
//                if (P4IN & SW1) {
//                    sw1_position = RELEASED;          // 1
//                }
//            }
//        }
// }
//void Switch2_Process(void){
//    if (!(P2IN & SW2) && okay_to_look_at_switch2){
//    sw2_position = PRESSED;
//
//    okay_to_look_at_switch2 = NOT_OKAY;   // 0
//    count_debounce_SW2 = 0;
//    }
//
//
//
//    if (okay_to_look_at_switch2 == NOT_OKAY) {
//
//                if (count_debounce_SW2++ >= DEBOUNCE_TIME) {
//
//                    // After debounce time, allow new presses
//                    okay_to_look_at_switch2 = OKAY;       // 1
//
//                    // Update release state
//                    if (P2IN & SW2) {
//                        sw2_position = RELEASED;          // 1
//                    }
//                }
//            }
//
//}
//
//
//
//
//
