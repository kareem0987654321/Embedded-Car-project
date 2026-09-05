//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Kareem Elkassem
//  January 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
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

extern unsigned char event;
extern unsigned char state;
extern unsigned int counter;
extern unsigned int sidecount;
extern unsigned int phase;
extern unsigned int figure8count;
extern unsigned int timecount;
extern unsigned char switch_number;
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int ADC_Thumb;
extern unsigned char ADC_Channel;
extern unsigned int speed;
extern volatile unsigned int send_message;
volatile unsigned int update_baud = 1;
char i;
char NCSU_1[]= "  NCSU  #1\r\n";
extern char baud_460[] = "  460,800 ";
extern char baud_115[] = "  115,200 ";
unsigned int direct_iot;
volatile char IOT_Ring_Rx[50];
volatile unsigned int splash_done;
volatile unsigned int iot_rx_wr;
char process_buffer[25];
char pb_index;
extern volatile char USB_Ring_Rx[16];
extern volatile unsigned int usb_rx_wr;
volatile unsigned int usb_rx_rd;
volatile unsigned int iot_rx_rd;
 int j;
 int idx;
 char c;
 extern volatile unsigned int msg_received;
 char received_command[11]={0};
 char line2_command[11] = {0};
 extern volatile char iot_TX_buf[16];
 extern volatile unsigned int iot_tx;
 extern volatile int move_active;
 extern volatile char move_direction;
 extern volatile unsigned int move_start;
 extern volatile unsigned int move_duration;
 extern unsigned int boot_state;
 extern volatile unsigned int pc_ready;
 extern volatile unsigned int cmd_mode;
 extern volatile unsigned int cmd_index;
 extern char cmd_buffer[11];
 unsigned int boot_timer;
 unsigned int boot_done;
 unsigned int keepalive_timer;

//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings


  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();
  Init_ADC();
  Init_DAC();
  Init_Serial_UCA0(speed);
  Init_Serial_UCA1(speed);

  wheel_move = 0;
  forward = TRUE;

  P2OUT  |= IR_LED;


//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------

  while(ALWAYS) {                       // Can the Operating system run
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

    if(Last_Time_Sequence != Time_Sequence){
    Last_Time_Sequence = Time_Sequence;
    cycle_time++;
    time_change = 1;
    }

    while(iot_rx_wr != iot_rx_rd){
        IOT_Process();
    }
    if(!boot_done){                     // booting up the IOT module
    while(boot_state < 5){
        IOT_Process();
      switch(boot_state){
      case 0:
          if (timecount >= 2){
              P3OUT |= IOT_EN_CPU;
              P6OUT |= GRN_LED;
              boot_state = 1;
             }
          break;
      case 1:
          if((timecount - boot_timer) >10){
          IOT_Send("AT\r\n");
          boot_timer = timecount;
          boot_state = 2;
          }
          break;
      case 2:
         if((timecount - boot_timer) > 5){
          IOT_Send("AT+CIPMUX=1\r\n");
          boot_timer = timecount;
          boot_state = 3;
      }
          break;
      case 3:
          if((timecount-boot_timer) > 5){
          IOT_Send("AT+CIPSERVER=1,6365\r\n");
          boot_timer = timecount;
          boot_state=4;
          }
          break;
      case 4:
          if((timecount - boot_timer) > 5){
          IOT_Send("AT+CIFSR\r\n");
          boot_timer = timecount;
          boot_state = 5;
          }
          break;

      default:
          break;
     }
    }
    boot_timer = timecount;
    while((timecount - boot_timer) < 10){   // wait up to 2 seconds
        IOT_Process();
    }
    P6OUT &= ~GRN_LED;
    boot_done = 1;
    }


    // ------------------------------------handle movements
    if(move_active){
           if((timecount - move_start) < move_duration){
               // Keep moving
               switch(move_direction){
                   case 'F': Forward_Move(); break;
                   case 'B': Reverse_Move(); break;
                   case 'L': CW_Move2(); break;
                   case 'R': CCW_Move2(); break;
                   default: break;
               }
           } else {
               move_active = FALSE;  // Stop moving
               Forward_Off();
               strcpy(display_line[1], "   STOP   "); // Update LCD
               display_changed = 1;
               update_display = 1;
           }
       }



  // ------------------ PC-> IOT
    if(usb_rx_wr != usb_rx_rd) {
                char usb_char = USB_Ring_Rx[usb_rx_rd++];

                if(usb_rx_rd >= sizeof(USB_Ring_Rx)) {
                    usb_rx_rd = 0;
                }

                /* First received char sets pc_ready - handled in UCA1 ISR */

                if(usb_char == '^') {
                    /* Start of a FRAM-only command */
                    cmd_mode = TRUE;
                    cmd_index = 0;
                    cmd_buffer[cmd_index++] = usb_char;
                }
                else if(cmd_mode) {
                    if(usb_char == 0x0D) {
                        /* Carriage return = end of command */
                        cmd_buffer[cmd_index] = '\0';
                        cmd_mode  = FALSE;
                        cmd_index = 0;
                        Process_Command(cmd_buffer);   // Execute the captured command
                    } else {
                        if(cmd_index < 10) {
                            cmd_buffer[cmd_index++] = usb_char;
                        }
                    }
                }
                else {
                    /* Not a FRAM command - pass character through to IOT via UCA0 */
                    while(!(UCA0IFG & UCTXIFG));   // Wait for TX buffer ready
                    UCA0TXBUF = usb_char;
                }
            }


switch(event){
case IDLE:
idle_case();
break;
case WAIT:
wait_case();
break;
case FINAL:
final_move();
break;
case START:
start_case();
break;
case BLACK_DETECT:
blackline_case();
break;
case TURN:
turn_case();
break;
case DONE:
done_case();
break;
case FOLLOW:
follow_line();
break;
case EXIT:
exit_case();
break;
default:
break;
}
  }

}

