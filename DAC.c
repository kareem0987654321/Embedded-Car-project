/*
 * DAC.c
 *
 *  Created on: Mar 20, 2026
 *  Author: Kareem Elkassem
 *  This file has the initialization for the DAC.
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

unsigned int DAC_data;

void Init_DAC(void){
SAC3DAC  = DACSREF_0;
SAC3DAC |= DACLSEL_0;
SAC3OA  = NMUXEN;
SAC3OA |= PMUXEN;
SAC3OA |= PSEL_1;
SAC3OA |= NSEL_1;
SAC3OA |= OAPM;
SAC3PGA = MSEL_1;
SAC3OA |= SACEN;
SAC3OA |= OAEN;
DAC_data = DAC_Begin;
// Select VCC as DAC reference
// DAC latch loads when DACDAT written
// SAC Negative input MUX control
// SAC Positive input MUX control
// 12-bit reference DAC source selected
// Select negative pin input
// Select low speed and low power mode
// Set OA as buffer mode
// Enable SAC
// Enable OA
// Starting Low value for DAC output [2v]
SAC3DAT = DAC_data;
TB0CTL |=  TBIE;
P2OUT   |=  DAC_ENB;
// Initial DAC data
// Timer B0 overflow interrupt enable

SAC3DAC |= DACEN;
// Set RED_LED On indicates the OVERFLOW timer start
// Enable DAc
}




