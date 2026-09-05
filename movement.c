/*
 *  movement.c
 *
 *  Created on: Feb 6, 2026
 *  Author: Kareem Elkassem
 *  This file handles the movement functions for the car.
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern unsigned int segment_count;
extern unsigned int cycle_time;
extern unsigned int right_motor_count;
extern unsigned int left_motor_count;
extern unsigned int time_change;
unsigned int speed;
unsigned char pwm_active;
unsigned int pwm_speed;


void Forward_Off(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_REVERSE_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void Rforward(void){
    P6OUT |= R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~R_REVERSE;

}
void Lforward(void){
    P6OUT |= L_FORWARD;
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~R_REVERSE;

}
void Rreverse(void){
    P6OUT |= R_REVERSE;
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~L_FORWARD;

}
void Lreverse(void){
    P6OUT |= L_REVERSE;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~R_REVERSE;


}
void Arch(void){
    LEFT_FORWARD_SPEED = RIGHTARCH;
    RIGHT_FORWARD_SPEED = LEFTARCH;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void BLForward_Move(void){
    LEFT_FORWARD_SPEED = SLOWLEFT;
    RIGHT_FORWARD_SPEED = SLOWRIGHT;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void BLForward_Move2(void){
    LEFT_FORWARD_SPEED = SLOW;
    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void KForward_Move(void){
    LEFT_FORWARD_SPEED = KSLOWLEFT;
    RIGHT_FORWARD_SPEED = KSLOW;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void Forward_Move(void){
    LEFT_FORWARD_SPEED = FASTLEFT;
    RIGHT_FORWARD_SPEED = FAST;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void Reverse_Move(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = FAST;
    RIGHT_REVERSE_SPEED = FAST;
}
void Reverse_Move2(){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = SLOW;
    RIGHT_REVERSE_SPEED = SLOW;

}

void CW_Move(void){
   LEFT_FORWARD_SPEED = TURNSLOW;
   RIGHT_REVERSE_SPEED = TURNSLOW;
   LEFT_REVERSE_SPEED = WHEEL_OFF;
   RIGHT_FORWARD_SPEED = WHEEL_OFF;
}

void CW_Move2(void){
    LEFT_FORWARD_SPEED = IOTTURN;
    RIGHT_REVERSE_SPEED = IOTTURN;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;

}
void CCW_Move2(void){
   RIGHT_FORWARD_SPEED = IOTTURN;
   LEFT_REVERSE_SPEED = IOTTURN;
   LEFT_FORWARD_SPEED = WHEEL_OFF;
   RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void CCW_Move(void){
   RIGHT_FORWARD_SPEED = TURNSLOW;
   LEFT_REVERSE_SPEED = TURNSLOW;
   LEFT_FORWARD_SPEED = WHEEL_OFF;
   RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void Slight_Right(void){
    LEFT_FORWARD_SPEED = SLOW;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void Slight_Left(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = SLOW;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}





