/*
 * timers.c
 *
 *  Created on: Feb 24, 2026
 *  Author: Kareem Elkassem
 *  This file handles the timers/Interrupts for switches.
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */

#include <stdio.h>
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
volatile unsigned int DAC_data;
unsigned int Time_Sequence;
unsigned int time_change;
extern unsigned int update_display;
extern unsigned int display_changed;
extern char display_line[4][11];
volatile unsigned int timecount;
 unsigned int blink_count;
 volatile unsigned int sw1_debounce_count;
 volatile unsigned int sw2_debounce_count;
 volatile unsigned int sw1_debouncing;
 volatile unsigned int sw2_debouncing;
extern unsigned char event;
 unsigned char switch_number;
 extern unsigned char pwm_active;
 unsigned int pwm_phase;
 extern volatile unsigned int white_left;
 extern volatile unsigned int white_right;
 extern volatile unsigned int black_left;
 extern volatile unsigned int black_right;
 volatile unsigned int capture_step;
 extern unsigned int ADC_Left_Detect;
 extern unsigned int ADC_Right_Detect;
extern volatile char baud_mode;
extern volatile char update_baud;
volatile unsigned int send_message;

extern volatile unsigned int delay_start;
unsigned char speed;
volatile int dac_state = DAC_IDLE;

void Init_Timers(void){
    Init_Timer_B0();
    Init_Timer_B3();

}

void Init_Timer_B0(void){
//-----------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0 and B0_1 to B0_2 and OverFlow
// 8,000,000 / 8 / 8 / [1/time]
// 1,000,000 / 8 / [1/time]
// 125,000 / [1/time]
// 1/time => 1 / 8msec => 1 / 0.005 = 200
// 125,000 / 200 = 625 – This is Capture Compare Register Interval
// 8usec per clock tick
//-----------------------------------------------------------------------------
TB0CTL = TBSSEL__SMCLK;
TB0CTL |= MC__CONTINUOUS;
TB0CTL |= ID__8;
TB0CTL |= TBCLR;
TB0EX0 = TBIDEX__8;
// SMCLK
// continuous mode
// Divide clock by 8
// Clear Count
// Second Divider - Divide clock by 8
TB0CCR0 =TB0R + TB0CCR0_INTERVAL;
TB0CCTL0 &= ~CCIFG;
TB0CCTL0 |= CCIE;

TB0CCR1 =TB0R + TB0CCR1_INTERVAL;
TB0CCTL1 &= ~CCIFG;
TB0CCTL1 &= ~CCIE;

TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
TB0CCTL2 &= ~CCIFG;
TB0CCTL2 &= ~CCIE;

TB0CTL &= ~TBIE;
TB0CTL &= ~TBIFG;
}

// timer for 200 milliseconds
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    Time_Sequence++;
    timecount++;
//    P6OUT ^= LCD_BACKLITE;
    if(sw2_debouncing){
                sw2_debounce_count++;

                if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
                    sw2_debouncing = 0;
                    TB0CCTL2 &= ~CCIE;
                    P2IFG &= ~SW2;
                    P2IE |= SW2;

                    TB0CCTL0 |= CCIE;
                    TB0CCTL0 &= ~CCIFG;
                    TB0CCR0 =TB0R + TB0CCR0_INTERVAL;

                }else{
                    TB0CCR2 += TB0CCR2_INTERVAL;

                }
            }

    HEXtoBCDDEC(timecount << 1);
//    adc_line(3,0);
    display_changed = 1;
    update_display = 1;
    TB0CCR0 += TB0CCR0_INTERVAL;
    }
// switch 1 interrupt
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if(P4IFG & SW1){
        P4IFG &= ~SW1; // clear flag
        P4IE &= ~SW1; // disable switch interrupt
        sw1_debouncing = 1; // debounce active
        sw1_debounce_count = 0; // reset counter
        timecount =0;
        send_message = 1;
        P6OUT ^= GRN_LED;

//        event = 1;      // flag for display
//        switch_number = 1;
        TB0CCR1 =TB0R + TB0CCR1_INTERVAL;
        TB0CCTL1 &= ~CCIFG; //clear timer interrupt
        TB0CCTL1 |= CCIE; // enable ccr1 interrupt
    }
}
//switch 2 interrupt
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if(P2IFG & SW2){
        P2IFG &= ~SW2;
        P2IE &= ~SW2;
        P2OUT |= IR_LED;

        sw2_debouncing = 1;
        sw2_debounce_count = 0;
        switch(capture_step){
        case 0:
                white_left = ADC_Left_Detect;
                white_right = ADC_Right_Detect;
               capture_step = 1;
               break;
        case 1:
                black_left = ADC_Left_Detect;
                black_right = ADC_Right_Detect;
                     break;
        default:
        break;

        }
        TB0CCR2 =TB0R + TB0CCR2_INTERVAL;
        TB0CCTL2 &= ~CCIFG; // disable timer interrupt
        TB0CCTL2 |= CCIE; // enable ccr2 interrupt
    }
}
// debounce
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    switch(__even_in_range(TB0IV,14)){
    case 2:
//        TB0CCR1 += TB0CCR1_INTERVAL;


        if(sw1_debouncing){
            sw1_debounce_count++;
            if(sw1_debounce_count >= DEBOUNCE_THRESHOLD){
                sw1_debouncing = 0;
                TB0CCTL1 &= ~CCIE;
                P4IFG &= ~SW1;
                P4IE |= SW1;


                TB0CCR0 =TB0R + TB0CCR0_INTERVAL;
                TB0CCTL0 &= ~CCIFG;
                TB0CCTL0 |=CCIE;
            }else{
                TB0CCR1 += TB0CCR1_INTERVAL;
            }
        }
        break;
    case 4:
//        TB0CCR2 += TB0CCR2_INTERVAL;
        if(sw2_debouncing){
            sw2_debounce_count++;

            if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
                sw2_debouncing = 0;
                TB0CCTL2 &= ~CCIE;
                P2IFG &= ~SW2;
                P2IE |= SW2;

                TB0CCTL0 |= CCIE;
                TB0CCTL0 &= ~CCIFG;
                TB0CCR0 =TB0R + TB0CCR0_INTERVAL;

            }else{
                TB0CCR2 += TB0CCR2_INTERVAL;

            }
        }
        break;
    case 14:
    DAC_data = DAC_data - 100;
    SAC3DAT = DAC_data;
    if(DAC_data <= DAC_Limit){
    DAC_data = DAC_Adjust;
    SAC3DAT = DAC_data;
    TB0CTL &= ~TBIE;
    dac_state = DAC_DONE;
    }
 default:
     break;
    }
}

void Init_Timer_B3(void) {
//-----------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.1 L_FORWARD
// TB3.2 P6.2 R_FORWARD
// TB3.3 P6.3 L_REVERSE
// TB3.4 P6.4 R_REVERSE
// TB3.5 P6.5 LCD_BACKLITE
//-----------------------------------------------------------------------------
TB3CTL = TBSSEL__SMCLK;
TB3CTL |= MC__UP;
TB3CTL |= TBCLR;
// SMCLK
// Up Mode
// Clear TAR
TB3CCR0 = WHEEL_PERIOD;
TB3CCTL1 = OUTMOD_7;
LCD_BACKLITE_DIMING = PERCENT_80;
  // PWM Period
// CCR1 reset/set
//[Set this to 50005]
// P6.1 Left Forward PWM duty cycle
TB3CCTL2 = OUTMOD_7;
RIGHT_FORWARD_SPEED = WHEEL_OFF;
TB3CCTL3 = OUTMOD_7;
LEFT_FORWARD_SPEED = WHEEL_OFF;
TB3CCTL4 = OUTMOD_7;
RIGHT_REVERSE_SPEED = WHEEL_OFF;
TB3CCTL5 = OUTMOD_7;
LEFT_REVERSE_SPEED = WHEEL_OFF;
// CCR2 reset/set
// P6.2 Right Forward PWM duty cycle
// CCR3 reset/set
// P6.3 Left Reverse PWM duty cycle
// CCR4 reset/set
// P6.4 Right Reverse PWM duty cycle
// CCR5 reset/set
// P6.5 LCD_BACKLITE On Diming percent
//-----------------------------------------------------------------------------
}







