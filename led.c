/*
 * led.c
 *
 *  Created on: Feb 5, 2026
 *  Author: Kareem Elkassem
 *  This file contains the code for turning on or off the LED
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}




