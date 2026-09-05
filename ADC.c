/*
 * ADC.C
 *
 *  Created on: Feb 27, 2026
 *  Author: Kareem Elkassem
 *  This file handles the ADC readings and the state machine functions for the black line following
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 *
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"


unsigned char adc_char[6];   // 4 digits + '\0'
unsigned int display_changed;
unsigned int update_display;
unsigned  int hex_value;
unsigned int real_line;
char display_line[4][11];
unsigned int ADC_Left_Detect;
unsigned int ADC_Right_Detect;
unsigned int ADC_Thumb;
unsigned char ADC_Channel;
unsigned int i;
volatile unsigned int timecount;
unsigned char event;
extern unsigned int speed;
unsigned int exitstate;
volatile unsigned int white_left;
volatile unsigned int white_right;
volatile unsigned int black_left;
volatile unsigned int black_right;
unsigned int blfound;
unsigned int whitefound;
unsigned int finalturn;
unsigned int fix;
unsigned int exitflag;
unsigned int follow_state;
unsigned int travel_timecount;
unsigned int turnstate;

void HEXtoBCD(int hex_value){
int value = 0;
for(i=0; i < 4; i++) {
adc_char[i] = '0';
}
while (hex_value > 999){
hex_value = hex_value- 1000;
value = value + 1;
adc_char[0] = 0x30 + value;
}
value = 0;
while (hex_value > 99){
hex_value = hex_value- 100;
value = value + 1;
adc_char[1] = 0x30 + value;
}
value = 0;
while (hex_value > 9){
hex_value = hex_value- 10;
value = value + 1;
adc_char[2] = 0x30 + value;
}
adc_char[3] = 0x30 + hex_value;
}


void HEXtoBCDDEC(int hex_value){
int value = 0;
for(i=0; i < 5; i++) {
adc_char[i] = '0';
}
while (hex_value > 999){
hex_value = hex_value- 1000;
value = value + 1;
adc_char[0] = 0x30 + value;
}
value = 0;
while (hex_value > 99){
hex_value = hex_value- 100;
value = value + 1;
adc_char[1] = 0x30 + value;
}
value = 0;
while (hex_value > 9){
hex_value = hex_value- 10;
value = value + 1;
adc_char[2] = 0x30 + value;
}
adc_char[3] = '.';
adc_char[4] = 0x30 + hex_value;
}

//------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
//
//char line => Specifies the line 1 thru 4
//
//
//char location => Is the location 0 thru 9
//------------------------------------------------------------
void adc_line(char line, char location){
//------------------------------------------------------------
int i;
unsigned int real_line;
real_line = line - 1;
for(i=0; i < 5; i++) {
display_line[real_line][i+location] = adc_char[i];
}
}



void Init_ADC(void){                        // ADC initialization
 ADCMCTL0 |= ADCINCH_2;
// ADCCTL0 Register
ADCCTL0 = 0;
ADCCTL0 |= ADCSHT_2;
ADCCTL0 |= ADCMSC;
ADCCTL0 |= ADCON;
// ADCCTL1 Register
ADCCTL1 = 0;
ADCCTL1 |= ADCSHS_0;
ADCCTL1 |= ADCSHP;
ADCCTL1 &= ~ADCISSH;
ADCCTL1 |= ADCDIV_0;
ADCCTL1 |= ADCSSEL_0;
ADCCTL1 |= ADCCONSEQ_0;
ADCCTL2 = 0;
ADCCTL2 |= ADCPDIV0;
ADCCTL2 |= ADCRES_2;
ADCCTL2 &= ~ADCDF;
ADCCTL2 &= ~ADCSR;
// ADCMCTL0 Register
ADCMCTL0 |= ADCSREF_0;
ADCMCTL0 |= ADCINCH_2;
ADCIE |= ADCIE0;
ADCCTL0 |= ADCENC;
ADCCTL0 |= ADCSC;
}
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
case ADCIV_NONE:
break;
case ADCIV_ADCOVIFG:   // When a conversion result is written to the ADCMEM0
// before its previous conversion result was read.
break;
case ADCIV_ADCTOVIFG:   // ADC conversion-time overflow
break;
case ADCIV_ADCHIIFG:
// Window comparator interrupt flags
break;
case ADCIV_ADCLOIFG:
break;
case ADCIV_ADCINIFG:
break;
case ADCIV_ADCIFG:

   ADCCTL0 &= ~ADCENC;                         // Disable ENC bit.
   switch (ADC_Channel){
      case 0:                                // Channel A2 Interrupt
        ADC_Left_Detect = ADCMEM0;              // Move result into Global
        ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
        ADCMCTL0 &= ~ADCINCH_2;                 // Disable Last channel A2
        ADCMCTL0 |= ADCINCH_3;                  // Enable Next channel A3
        ADC_Channel = 1;
        break;
      case 1:
        ADC_Right_Detect = ADCMEM0;
        ADC_Right_Detect =  ADC_Right_Detect >> 2;
        ADCMCTL0 &= ~ADCINCH_3;                 // Disable Last channel A3
        ADCMCTL0 |= ADCINCH_5;                  // enable next channel A5
        ADC_Channel = 2;
        break;
      case 2:
        ADC_Thumb = ADCMEM0;
        ADC_Thumb = ADC_Thumb >> 2;
        ADCMCTL0 &= ~ADCINCH_5;                 // Disable Last channel A5
        ADCMCTL0 |= ADCINCH_2;                  // enable next channel A2
        ADC_Channel=0;
        break;
      default:
        break;
    }
    ADCCTL0 |= ADCENC;                // Enable Conversions
    ADCCTL0 |= ADCSC;
// Start next sample
break;
default:
break;
}
}

void idle_case(void){
    P2OUT |= IR_LED;
    Forward_Off();
 }
void wait_case(void){
    Forward_Off();
    P2OUT  &= ~IR_LED;
        if(timecount >= 5){  // If your timer ISR = 200ms
            timecount = 0;
            P2OUT  &= ~IR_LED;
            if(event == WAIT){
                event = FINAL ;
            }
        }

  }
void final_move(void){
    lcd_BIG_mid();
    strcpy(display_line[1], " ARRIVE 8 ");
    display_changed = 1;
    update_display = 1;
    if(finalturn == 0){
        KForward_Move();
        P6OUT &= ~GRN_LED;

        if (timecount >= 21){
            Forward_Off();
            timecount = 0;
            finalturn = 1;
        }
    }
    else if(finalturn == 1){
        CCW_Move();
        if(timecount >= 2){
            timecount = 0;
            Forward_Off();
            finalturn = 2;
            event = IDLE;
        }
    }

}
void start_case(void){
    lcd_BIG_mid();
    strcpy(display_line[1], " BL START ");
    display_changed = 1;
    update_display = 1;
if (whitefound == 0){
    P2OUT |= IR_LED;
    Arch();

    if(ADC_Right_Detect <= white_right){ // ADJUST THIS TO CHECK FOR WHITE
        P6OUT |= GRN_LED;
        Forward_Off();
        timecount = 0;
        whitefound = 1;
    }
  }
    if(whitefound == 1 && timecount>= 5){
        event = BLACK_DETECT;
        timecount = 0;
    }
}

void blackline_case(void){
    if(blfound == 0){
        BLForward_Move();
        if(ADC_Right_Detect >= black_right){   // black line detected
            blfound = 1;
            Forward_Off();
            timecount = 0;
        }
    }
    else{
            lcd_BIG_mid();
            strcpy(display_line[1], "INTERCEPT ");
            display_changed = 1;
            update_display = 1;
            if(timecount >= 50){   // 10 seconds then move to turn
                timecount = 0;
                event = TURN;
            }
        }
}

void turn_case(void){
    lcd_BIG_mid();
    strcpy(display_line[1], "  BL TURN ");
       display_changed = 1;
       update_display = 1;
    if(turnstate == 0){
    CW_Move();
    if(ADC_Right_Detect >= black_right && ADC_Left_Detect >= black_left){    // ADJUST TO IF RIGHT DETECT IS BLACK
            Forward_Off();
            timecount = 0;
            turnstate = 1;
        }
    }
    else if (turnstate==1){
        Forward_Off();
    if(timecount >= 50){ // WAIT 10 SECONDS TO MOVE ON
    timecount = 0;
    event = FOLLOW;
    }
    }
}

void follow_line(void){
    IOT_Process();
    if(fix == 1){
            if(timecount >= 4){
                Reverse_Move2();
            }
            if(ADC_Left_Detect >= black_left && ADC_Right_Detect >= black_right){
                Forward_Off();
                fix = 0;
                timecount = travel_timecount;
            }
            return; // don't run normal follow logic while recovering
        }

        // Normal follow logic only runs when fix == 0
        if(follow_state == 0){
            lcd_BIG_mid();
            strcpy(display_line[1], "BL_Travel ");
            display_changed = 1;
            update_display = 1;
            if(ADC_Left_Detect >= black_left && ADC_Right_Detect >= black_right){
                BLForward_Move2();
            }
            else if(ADC_Left_Detect < black_left && ADC_Right_Detect >= black_right){
                Slight_Right();
            }
            else if(ADC_Right_Detect < black_right && ADC_Left_Detect >= black_left){
                Slight_Left();
            }
            else if(ADC_Right_Detect < black_right-130 && ADC_Left_Detect < black_left-130){
                travel_timecount = timecount; // save progress before hijacking timecount
                Forward_Off();
                fix = 1;
                timecount = 0;
            }
            if(timecount >= 35){
                Forward_Off();
                timecount = 0;
                follow_state = 1;
            }
        }
        else if(follow_state == 1){
            lcd_BIG_mid();
            strcpy(display_line[1], "BL_Travel ");
            display_changed = 1;
            update_display = 1;
            Forward_Off();
            if(timecount >= 50){
                timecount = 0;
                follow_state = 2;
            }
        }
        else if(follow_state == 2){
            lcd_BIG_mid();
            strcpy(display_line[1], "BL CIRCLE ");
            display_changed = 1;
            update_display = 1;
            if(ADC_Left_Detect >= black_left && ADC_Right_Detect >= black_right){
                BLForward_Move2();
            }
            else if(ADC_Left_Detect < black_left && ADC_Right_Detect >= black_right){
                Slight_Right();
            }
            else if(ADC_Right_Detect < black_right && ADC_Left_Detect >= black_left){
                Slight_Left();
            }
            else if(ADC_Right_Detect < black_right-130 && ADC_Left_Detect < black_left-130) {
                // Both sensors off line — trigger recovery
                Forward_Off();
                fix = 1;
                timecount = 0;
            }
        }
    }


void exit_case(void){     // THIS WILL HAPPEN WHEN I SEND ^6365E0000
    if(timecount >= 50 && exitstate == 0){ // waiting 10 seconds to exit
        Forward_Move();
        timecount = 0;
        exitstate = 1;
    }
    if(timecount >= 10 && exitstate == 1){
        Forward_Off();
        event = DONE;
    }

}

void done_case(void){
   Forward_Off();
   lcd_BIG_mid();
   strcpy(display_line[1], " BL STOP  ");
   display_changed = 1;
   update_display = 1;
}
