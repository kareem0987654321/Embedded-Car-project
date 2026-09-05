/*
 * txrx.c
 *
 *  Created on: Mar 25, 2026
 *  Author: Kareem Elkassem
 *  This file handles the serial communication with the PC and the IOT module.
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 *
 */
#include  "msp430.h"
#include  <string.h>
#include <stdlib.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
extern unsigned int direct_iot;
extern char process_buffer[25];                 // Size for appropriate Command Length
extern char pb_index;                           // Index for process_buffer

volatile char iot_TX_buf[32];
volatile char USB_Ring_Rx[16];
extern volatile unsigned int iot_rx_wr;
volatile unsigned int iot_tx;
volatile unsigned int usb_rx_wr;
extern volatile unsigned int iot_rx_rd;
volatile unsigned int iot_rx_wr;
extern volatile char IOT_Ring_Rx[50];
char IOT_Data[4][11];
char display_line[4][11];
unsigned int line;
unsigned int character;
unsigned int nextline =1;
unsigned int test_Value;
unsigned int IOT_parse;
unsigned int boot_state;
unsigned int ip_address_found;
unsigned int display_changed;
volatile unsigned int rx_ready;
extern volatile unsigned int send_message;
volatile unsigned int msg_received;
const char ip_mac[] = "AT+CIPSTAMAC?\r\n";
const char AT[] = "AT\r\n";
char ip_address[16];
unsigned int iot_index;
volatile unsigned int pc_ready;
char cmd_buffer[11];
unsigned int cmd_mode;
unsigned int cmd_index;
unsigned int iot_index;
volatile int move_active;
volatile char move_direction = 'F';
volatile unsigned int move_start;
volatile unsigned int move_duration;
extern unsigned int timecount;
volatile unsigned int update_display;
extern unsigned char event;
extern unsigned int exitflag;
static unsigned int ip_collecting;


//initialization for eUSCI_A0
void Init_Serial_UCA0(char speed){
    UCA0CTLW0 = 0;
     UCA0CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
     UCA0CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
     UCA0CTLW0 &= ~UCMSB;                 // MSB, LSB select
     UCA0CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
     UCA0CTLW0 &= ~UCPEN;                 // No Parity
     UCA0CTLW0 &= ~UCSYNC;
     UCA0CTLW0 &= ~UC7BIT;
     UCA0CTLW0 |=  UCMODE_0;

     UCA0BRW = 4;                        // 115,200 baud
     UCA0MCTLW = 0x5551;
     UCA0CTLW0 &= ~UCSWRST ;              // release from reset
     UCA0TXBUF = 0x00;                    // Prime the Pump
     UCA0IE |= UCRXIE;                    // Enable RX interrupt
}

//initialization for eUSCI_A1
void Init_Serial_UCA1(char speed){
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    update_display = 1;
    display_changed = 1;
    UCA1CTLW0 = 0;
     UCA1CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
     UCA1CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
     UCA1CTLW0 &= ~UCMSB;                 // MSB, LSB select
     UCA1CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
     UCA1CTLW0 &= ~UCPEN;                 // No Parity
     UCA1CTLW0 &= ~UCSYNC;
     UCA1CTLW0 &= ~UC7BIT;
     UCA1CTLW0 |=  UCMODE_0;

         UCA1BRW = 4;                        // 115,200 baud
         UCA1MCTLW = 0x5551;

//     UCA1BRW = 4;                        // 115,200 baud
//     UCA1MCTLW = 0x5551;
     UCA1CTLW0 &= ~UCSWRST ;              // release from reset
     UCA1TXBUF = 0x00;                    // Prime the Pump
     UCA1IE |= UCRXIE;                    // Enable RX interrupt
}

// interrupt for UCA0
#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){    //This interrupt is the interrupt relating to serial communication port UCA0
  //-----------------------------------------------------------------------------
  // Interrupt name: eUSCI_A0_ISR
  // Description: This interrupt transmits and receives through UCA0
  //-----------------------------------------------------------------------------
  char iot_receive;
  switch(__even_in_range(UCA0IV,0x08)){
  case 0: break;                 //Vector 0 - no interrupt
  case 2:{                                     // Vector 2 – Rx0IFG
    iot_receive = UCA0RXBUF;
    IOT_Ring_Rx[iot_rx_wr++] = iot_receive;    // Add to Ring Buffer
    if(iot_rx_wr >= sizeof(IOT_Ring_Rx)){
      iot_rx_wr = BEGINNING;
    }
    if(pc_ready){
    UCA1TXBUF = iot_receive;
    }
    }break;
  case 4:{                                     // Vector 4 – Tx0IFG
    UCA0TXBUF = iot_TX_buf[iot_tx];

    if(iot_TX_buf[iot_tx] == 0x00){
      UCA0IE &= ~UCTXIE;
    }

    iot_TX_buf[iot_tx++] = 0;
  }break;
  default: break;
  }
}

// interrupt for UCA1
#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){            //This interrupt is the interrupt relating to serial communication port UCA1
  //-----------------------------------------------------------------------------
  // Interrupt name: eUSCI_A1_ISR
  // Description: This interrupt transmits and receives through UCA1
  //-----------------------------------------------------------------------------
  char usb_value;
  switch(__even_in_range(UCA1IV,0x08)){
    case 0: break;                    //Vector 0 - no interrupt
    case 2:{                                  // Vector 2 - Rx1IFG
      usb_value = UCA1RXBUF;
      if(!pc_ready){
          pc_ready = TRUE;
      }
      USB_Ring_Rx[usb_rx_wr++] = usb_value;   // Add to Ring Buffer
      if(usb_rx_wr >= sizeof(USB_Ring_Rx)){
        usb_rx_wr = BEGINNING;
      }
    }break;
  case 4:{                                    // Vector 4 - TX1IFG
      UCA1TXBUF = process_buffer[pb_index];  // Transmit Current Indexed value
      process_buffer[pb_index++] = NULL;
       // Null Location of Transmitted value
      if(process_buffer[pb_index] == NULL){  // Is the next pb_index location NULL - End of Command
      UCA1IE &= ~UCTXIE;
      }
  }break;
  default:
    break;
  }
  //-----------------------------------------------------------------------------
}

// Transmit Function for USCI_A0
void USCI_A0_transmit(void){
// Contents must be in process_buffer
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
pb_index = 0;                          // Set Array index to first location
UCA0IE |= UCTXIE;                      // Enable TX interrupt
}

void USCI_A1_transmit(void){
// Contents must be in process_buffer
// End of Transmission is identified by NULL character in process_buffer
// process_buffer includes Carriage Return and Line Feed
pb_index = 0;                          // Set Array index to first location
UCA1IE |= UCTXIE;                      // Enable TX interrupt
}
//--------------------------send string to iot
void IOT_Send(const char *cmd) {
    int i = 0;
    while(cmd[i] != '\0' && i < (sizeof(iot_TX_buf) - 1)) {
        iot_TX_buf[i] = cmd[i];
        i++;
    }
    iot_TX_buf[i] = 0x00;      // Null terminator marks end of message for ISR
    iot_tx = 0;                 // Reset TX index to start of buffer
    UCA0IE |= UCTXIE;           // Enable TX interrupt to start sending
}

//------------------------ parse +ipd commands
void Parse_IPD(char *msg) {
    char *data = strchr(msg, ':');    // Find the ':' separator
    if(data == NULL) return;

    data++;    // Advance past ':' to the actual payload

    /* Strip trailing \r and \n if present */
    int len = strlen(data);
    if(len > 0 && (data[len-1] == '\n' || data[len-1] == '\r')) {
        data[--len] = '\0';
    }
    if(len > 0 && (data[len-1] == '\n' || data[len-1] == '\r')) {
        data[--len] = '\0';
    }

    if(data[0] == '^') {
        /* It's a movement command - send to Process_Command */
        Process_Command(data);
    } else {
        /* Non-command payload - display on LCD line 3 for debugging */
        strncpy(display_line[3], data, 10);
        display_line[3][10] = '\0';
        display_changed = 1;
        update_display = 1;
    }
}

//------------------------ process pc commands
void Process_Command(char *cmd) {
    if(cmd == NULL)     return;
    if(cmd[0] != '^')  return;    // Must start with '^'

    /* Validate PIN (indices 1 through PIN_LENGTH) */
    if(strncmp(&cmd[1], SECRET_PIN, PIN_LENGTH) != 0) {
        strcpy(display_line[3], "BAD PIN   ");
        display_changed = TRUE;
        return;
    }

    /* Extract direction character */
    char dir = cmd[CMD_DIR_INDEX];

    /* K command triggering line follow state machine */
    if(dir == 'K'){
        timecount = 0;
        event = WAIT;
        return;
    }
    if(dir == 'E'){
        timecount = 0;
        Forward_Off();
        event = EXIT;
        P6OUT ^= GRN_LED;
        P2OUT &= ~IR_LED;
        lcd_BIG_mid();
        strcpy(display_line[1], "  BL EXIT ");
               display_changed = 1;
               update_display = 1;
        return;
    }
    if(dir == 'A'){
        timecount = 0;
        event = START;
        return;
    }

    if(dir == 'I'){
        timecount = 0;
        Forward_Off();
        P2OUT &= ~IR_LED;
        LCD_BACKLITE_DIMING = 0;
        return;
    }

    if(dir == 'H'){
        timecount = 0;
        Forward_Off();
        LCD_BACKLITE_DIMING = 20000;
     return;
    }

    if(dir >= 'S' && dir <= 'Z'){
            int spot = dir - 'S' + 1;   // S=1, T=2, U=3, V=4, W=5, X=6, Y=7, Z=8
            lcd_BIG_mid();
            switch(spot){
                case 1: strcpy(display_line[1], " ARRIVE 1 "); break;
                case 2: strcpy(display_line[1], " ARRIVE 2 "); break;
                case 3: strcpy(display_line[1], " ARRIVE 3 "); break;
                case 4: strcpy(display_line[1], " ARRIVE 4 "); break;
                case 5: strcpy(display_line[1], " ARRIVE 5 "); break;
                case 6: strcpy(display_line[1], " ARRIVE 6 "); break;
                case 7: strcpy(display_line[1], " ARRIVE 7 "); break;
                case 8: strcpy(display_line[1], " ARRIVE 8 "); break;
            }
            display_changed = 1;
            update_display = 1;
            return;   // 0000 duration ignored, just displays
        }

    if(dir != 'F' && dir != 'B' && dir != 'L' && dir != 'R') {
        return;    // Unknown direction - ignore
    }

    /* Extract duration (in 200ms ticks) */
    unsigned int duration = (unsigned int)atoi(&cmd[CMD_DUR_INDEX]);
    if(duration == 0) return;     // Zero duration - ignore

    /* Arm the movement */
    move_direction = dir;
    move_duration  = duration;
    move_start     = timecount;   // Record when movement begins (200ms ticks)
    move_active    = TRUE;

    /* Display command on LCD using big middle format (spec step 58) */
    lcd_BIG_mid();
    switch(dir) {
        case 'F': strcpy(display_line[1], " FORWARD  "); break;
        case 'B': strcpy(display_line[1], "BACKWARD  "); break;
        case 'L': strcpy(display_line[1], "  LEFT    "); break;
        case 'R': strcpy(display_line[1], "  RIGHT   "); break;
    }
    display_changed = 1;
    update_display = 1;
}


void IOT_Process(void) {
    unsigned int iot_rx_wr_temp;
    int i;
    static char ipd_buf[32];
    static unsigned int ipd_index = 0;
    static unsigned int ipd_active = 0;

    iot_rx_wr_temp = iot_rx_wr;

    if(iot_rx_wr_temp != iot_rx_rd) {

        char ch = IOT_Ring_Rx[iot_rx_rd++];
        if(iot_rx_rd >= sizeof(IOT_Ring_Rx)) {
            iot_rx_rd = BEGINNING;
        }

        if(ch == 0x0A) {
            if(ipd_active) {
                ipd_buf[ipd_index] = '\0';
                Parse_IPD(ipd_buf);
                ipd_active = 0;
                ipd_index  = 0;
            }

            if(ip_address_found) {
                ip_address[iot_index] = '\0';
                ip_address_found = 0;

                char part1[11] = {0};
                char part2[11] = {0};
                int  dot_count = 0;
                int  i1 = 0, i2 = 0;

                for(i = 0; ip_address[i] != '\0' && i < 15; i++) {
                    if(ip_address[i] == '.') dot_count++;
                    if(dot_count < 2) {
                        if(i1 < 10) part1[i1++] = ip_address[i];
                    } else {
                        if(i2 < 10) part2[i2++] = ip_address[i];
                    }
                }
                part1[i1] = '\0';
                part2[i2] = '\0';

                strcpy(display_line[0], "IP Address");
                strncpy(display_line[2], part1, 10);
                strncpy(display_line[3], part2, 10);
                display_changed = TRUE;
                update_display  = TRUE;
            }

            character = 0;
            line++;
            if(line >= 4) line = 0;
            nextline = line + 1;
            if(nextline >= 4) nextline = 0;

        } else {
            if(character < 9) {
                IOT_Data[line][character] = ch;
            }

            /* Confirm +IPD at position 3 before activating capture */
            if(character == 3 &&
               IOT_Data[line][0] == '+' &&
               IOT_Data[line][1] == 'I' &&
               IOT_Data[line][2] == 'P' &&
               ch == 'D') {
                ipd_active    = 1;
                ipd_index     = 0;
                ipd_buf[0]    = '+';
                ipd_buf[1]    = 'I';
                ipd_buf[2]    = 'P';
                ipd_buf[3]    = 'D';
                ipd_index     = 4;
            }

            if(ipd_active && character > 3) {
                if(ipd_index < 31) {
                    ipd_buf[ipd_index++] = ch;
                }
            }

            switch(character) {
                case 10:
                    if(ch == 'I') {
                        ip_address_found = 1;
                        iot_index = 0;
                        for(i = 0; i < sizeof(ip_address); i++) {
                            ip_address[i] = 0;
                        }
                    }
                    break;
                default: break;
            }
            if(ip_address_found) {
                if(ch == '"' && !ip_collecting) {
                    // Opening quote - start collecting on NEXT character
                    ip_collecting = 1;
                }
                else if(ch == '"' && ip_collecting) {
                    // Closing quote - stop
                    ip_collecting = 0;
                    ip_address[iot_index] = '\0';
                }
                else if(ip_collecting && ch != '\r' && ch != '\n') {
                    if(iot_index < 15) {
                        ip_address[iot_index++] = ch;
                    }
                }
            }

            character++;
        }
    }
}
