/*
 * ------------------------------------------------------------------------------
 */
//
//  Description: This file contains 6 ports with all pins
//
//  Kareem elkassem
//  February 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include "macros.h"
#include "ports.h"

void Init_Ports(void){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}
void Init_Port1(void){
    P1OUT = 0x00; // P1 set low
    P1DIR = 0x00; // set P1 direction to output

    P1SEL0 &= ~RED_LED; // GPI/O
    P1SEL1 &= ~RED_LED;
    P1OUT |= RED_LED;
    P1DIR |= RED_LED;

//    P1SEL0 &= ~V_A1_SEEED;//
//    P1SEL1 &= ~V_A1_SEEED;//
    P1OUT &= ~V_A1_SEEED;
    P1DIR &= ~V_A1_SEEED;
    P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED

//    P1SEL0 &= ~V_DETECT_L;//
//    P1SEL1 &= ~V_DETECT_L;//
    P1OUT &= ~V_DETECT_L;
    P1DIR &= ~V_DETECT_L;
    P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L

//    P1SEL0 &= ~V_DETECT_R;//
//    P1SEL1 &= ~V_DETECT_R;//
    P1OUT &= ~V_DETECT_R;
    P1DIR &= ~V_DETECT_R;
    P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R

//    P1SEL0 &= ~V_A4_SEEED;//
//    P1SEL1 &= ~V_A4_SEEED;//
    P1OUT &= ~V_A4_SEEED;
    P1DIR &= ~V_A4_SEEED;
    P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED

//    P1SEL0 &= ~V_THUMB;//
//    P1SEL1 &= ~V_THUMB;//
    P1OUT &= ~V_THUMB;
    P1DIR &= ~V_THUMB;
    P1SELC |= V_THUMB; //        ADC input for V_THUMB

    P1SEL0 |= UCA0RXD;//
    P1SEL1 &= ~UCA0RXD;
//    P1OUT &= ~UCA0RXD;
//    P1DIR &= ~UCA0RXD;

    P1SEL0 |= UCA0TXD;//
    P1SEL1 &= ~UCA0TXD;
//    P1OUT &= ~UCA0TXD;
//    P1DIR &= ~UCA0TXD;
}

void Init_Port2(void){
    // This port has pin 0-7
    // Configure Port 2-----------------------------------------------------------------------------
    P2OUT = 0x00;
    P2DIR = 0x00;
      // P2 set Low
      // Set P2 direction to output
    P2SEL0 &= ~SLOW_CLK;
    P2SEL1 &= ~SLOW_CLK;
    // SLOW_CLK GPIO operation
    P2OUT  &= ~SLOW_CLK;
      // SLOW_CLK GPIO operation
    P2DIR  |=  SLOW_CLK;
      // Initial Value = Low / Off
      // Direction = output
    P2SEL0 &= ~CHECK_BAT;
    P2SEL1 &= ~CHECK_BAT;
      // CHECK_BAT GPIO operation
    P2OUT  &= ~CHECK_BAT;
      // CHECK_BAT GPIO operation
    P2DIR  |=  CHECK_BAT;
      // Initial Value = Low / Off
      // Direction = output
    P2SEL0 &= ~IR_LED;
    P2SEL1 &= ~IR_LED;
      // P2_2 GPIO operation
    P2OUT  &= ~IR_LED;
    // P2_2 GPIO operation
    P2DIR  |=  IR_LED;
      // Initial Value = Low / Off
      // Direction = output
    P2SEL0 &= ~SW2;
    P2SEL1 &= ~SW2;
      // SW2 Operation
//    P2OUT  |=  SW2;
      // SW2 Operation
    P2DIR  &= ~SW2;
    P2PUD |= SW2;
    P2IES |= SW2;
    P2IFG &= ~SW2;
    P2IE |= SW2;
      // Configure pullup resistor
    P2REN  |=  SW2;
      // Direction = input
      // Enable pullup resistor
    P2SEL0 &= ~IOT_RUN_CPU;
    P2SEL1 &= ~IOT_RUN_CPU;
      // IOT_RUN_CPU GPIO operation
    P2OUT  &= ~IOT_RUN_CPU;
      // IOT_RUN_CPU GPIO operation
    P2DIR  |=  IOT_RUN_CPU;
      // Initial Value = Low / Off
      // Direction = output
    P2SEL0 &= ~DAC_ENB;
    P2SEL1 &= ~DAC_ENB;
      // DAC_ENB GPIO operation
    P2OUT  &= ~DAC_ENB;
      // DAC_ENB GPIO operation
    P2DIR  |=  DAC_ENB;
      // Initial Value = High
      // Direction = output
    P2SEL0 &= ~LFXOUT;
    P2SEL1 |= LFXOUT;
      // LFXOUT Clock operation
      // LFXOUT Clock operation
    P2SEL0 &= ~LFXIN;
    P2SEL1 |= LFXIN;
//
// LFXIN Clock operation
  // LFXIN Clock operation-----------------------------------------------------------------------------
}

void Init_Port3(void) {
    P3OUT = 0x00; // set P3 low
    P3DIR = 0x00; // set P3 direction to output

    P3SEL0 &= ~TEST_PROBE; // SET TO GPIO
    P3SEL1 &= ~TEST_PROBE;// SET TO GPIO
    P3OUT &= ~TEST_PROBE;
    P3DIR &= ~TEST_PROBE;

    P3SEL0 &= ~OA2O;//
    P3SEL1 &= ~OA2O;//
    P3OUT &= ~OA2O;
    P3DIR &= ~OA2O;

    P3SEL0 &= ~OA2N;//
    P3SEL1 &= ~OA2N;//
    P3OUT &= ~OA2N;
    P3DIR &= ~OA2N;

    P3SEL0 &= ~OA2P;//
    P3SEL1 &= ~OA2P;//
    P3OUT &= ~OA2P;
    P3DIR &= ~OA2P;

    P3SEL0 &= ~SMCLK; // SET TO GPIO
    P3SEL1 &= ~SMCLK; // SET TO GPIO
    P3OUT &= ~SMCLK;
    P3DIR &= ~SMCLK;

//    P3SEL0 &= ~DAC_CNTL; // set to GPIO
//    P3SEL1 &= ~DAC_CNTL; //set to GPIO
    P3OUT |= DAC_CNTL;
    P3DIR |= DAC_CNTL;
    P3SELC |= DAC_CNTL;

    P3SEL0 &= ~IOT_LINK_CPU;// SET TO GPIO
    P3SEL1 &= ~IOT_LINK_CPU; // SET TO GPIO
    P3OUT &= ~IOT_LINK_CPU;
    P3DIR &= ~IOT_LINK_CPU;

    P3SEL0 &= ~IOT_EN_CPU; // SE TO GPIO
    P3SEL1 &= ~IOT_EN_CPU;
    P3OUT &= ~IOT_EN_CPU;
    P3DIR |= IOT_EN_CPU;
}
void Init_Port4(void){
    //
      // Configure PORT 4-----------------------------------------------------------------------------
    P4OUT = 0x00;
    P4DIR = 0x00;
      // P4 set Low
      // Set P4 direction to output
    P4SEL0 &= ~RESET_LCD;
    P4SEL1 &= ~RESET_LCD;
      // RESET_LCD GPIO operation
    P4OUT  &= ~RESET_LCD;
      // RESET_LCD GPIO operation
    P4DIR  |=  RESET_LCD;
      // Initial Value = Low / Off
      // Direction = output
    P4SEL0 &= ~SW1;
    P4SEL1 &= ~SW1;
      // SW1 GPIO operation
    P4PUD  |=  SW1;
    // SW1 GPIO operation
    P4DIR  &= ~SW1;
      // Configure pullup resistor
    P4REN  |=  SW1;
    P4IES |= SW1;
    P4IFG &= ~SW1;
    P4IE |= SW1;
      // Direction = input
      // Enable pullup resistor
    P4SEL0 |=  UCA1TXD;
    P4SEL1 &= ~UCA1TXD;
      // USCI_A1 UART operation
      // USCI_A1 UART operation
    P4SEL0 |=  UCA1RXD;
    P4SEL1 &= ~UCA1RXD;
      // USCI_A1 UART operation
      // USCI_A1 UART operation
    P4SEL0 &= ~UCB1_CS_LCD;
    P4SEL1 &= ~UCB1_CS_LCD;
      // UCB1_CS_LCD GPIO operation
    P4OUT  |=  UCB1_CS_LCD;
      // UCB1_CS_LCD GPIO operation
    P4DIR  |=  UCB1_CS_LCD;
      // Set SPI_CS_LCD Off [High]
      // Set SPI_CS_LCD direction to output
    P4SEL0 |=  UCB1CLK;
    P4SEL1 &= ~UCB1CLK;
      // UCB1CLK SPI BUS operation
      // UCB1CLK SPI BUS operation
    P4SEL0 |=  UCB1SIMO;
    P4SEL1 &= ~UCB1SIMO;
      // UCB1SIMO SPI BUS operation
      // UCB1SIMO SPI BUS operation
    P4SEL0 |=  UCB1SOMI;
    P4SEL1 &= ~UCB1SOMI;
    //
      // UCB1SOMI SPI BUS operation
    // UCB1SOMI SPI BUS operation-----------------------------------------------------------------------------
}
void Init_Port5(void) {
    P5OUT = 0x00; // SET TO LOW
    P5DIR = 0x00; //DIRECTION TO OUT

//    P5SEL0 &= ~V_BAT; // SET TO GPIO
//    P5SEL1 &= ~V_BAT; // SET TO GIP OPERATION
    P5OUT &= ~V_BAT;
    P5DIR &= ~V_BAT;
    P5SELC |=  V_BAT;

//    P5SEL0 &= ~V_5_0;
//    P5SEL1 &= ~V_5_0;
    P5OUT &= ~V_5_0;
    P5DIR &= ~V_5_0;
    P5SELC |=  V_5_0;

//    P5SEL0 &= ~V_DAC;
//    P5SEL1 &= ~V_DAC;
    P5OUT &= ~V_DAC;
    P5DIR &= ~V_DAC;
    P5SELC |=  V_DAC;

//    P5SEL0 &= ~V_3_3;
//    P5SEL1 &= ~V_3_3;
    P5OUT &= ~V_3_3;
    P5DIR &= ~V_3_3;
    P5SELC |=  V_3_3;

    P5SEL0 &= ~IOT_BOOT; // SET TO GPIO OPERATION
    P5SEL1 &= ~IOT_BOOT; // SET TO GPIO OPERATION
    P5OUT |= IOT_BOOT;
    P5DIR |= IOT_BOOT;
}

void Init_Port6(void){
    P6OUT = 0x00;
    P6DIR = 0x00;

    P6SEL0 |= LCD_BACKLITE;
    P6SEL1 &= ~LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;

    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD;

    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD;

    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE;

    P6SEL0 |= L_REVERSE;
    P6SEL1 &= ~L_REVERSE;
    P6DIR |= L_REVERSE;

    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6OUT &= ~P6_5;
    P6DIR &= ~P6_5;

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6OUT &= ~GRN_LED;
    P6DIR |= GRN_LED;
}





