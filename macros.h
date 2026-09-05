/*
 * macros.h
 *
 *  Created on: Feb 5, 2026
 *  Author: Kareem Elkassem
 *  This file handle the macros used for the car.
 *  Built with Code Composer Version: CCS12.4.0.00007_win64
 */

#ifndef MACROS_H_
#define MACROS_H_

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define NONE                  ('N')
#define STRAIGHT              ('L')
#define TRIANGLE              ('T')
#define CAPTURE                ('C')
#define FIGUREEIGHT           ('F')
#define IDLE                  ('I')
#define WAIT                  ('W')
#define BLACK_DETECT          ('B')
#define TURN                  ('A')
#define DONE                   ('D')
#define FOLLOW                  ('F')
#define START                 ('S')
#define RUN                   ('R')
#define EXIT                  ('E')
#define FINAL                   ('Z')
#define WHEEL_COUNT_TIME       (50)
#define RIGHT_COUNT_TIME        (30)
#define TRIANGLE_COUNT_TIME_LEFT     (70)
#define TRIANGLE_COUNT_TIME_RIGHT (70)
#define LEFT_COUNT_TIME         (6)
#define TURN_TIME               (150)
#define SIDE_DISTANCE            (100)
#define CIRCLE_DISTANCE         (25)
#define TRAVEL_DISTANCE         (2)
#define WAITING2START          (100)
#define PRESSED                 (0)
#define RELEASED                (1)
#define OKAY                    (1)
#define NOT_OKAY                (0)
#define DEBOUNCE_RESTART        (0)
#define DEBOUNCE_TIME           (10000)
#define TB0CCR0_INTERVAL        (25000)// 200 ms
#define TB0CCR1_INTERVAL        (25000)// 200 ms
#define TB0CCR2_INTERVAL        (25000)// 200 ms
#define P2PUD                   (P2OUT)
#define P4PUD                   (P4OUT)
#define FIFTY_MS_COUNT           (10)
#define DEBOUNCE_THRESHOLD        (1)
#define PWM_PERIOD          (TB3CCR0)
#define LEFT_FORWARD_SPEED  (TB3CCR3)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED  (TB3CCR5)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LCD_BACKLITE_DIMING (TB3CCR1)
// PWM SPEEDS
#define WHEEL_OFF     (0)
#define SLOW        (11500)
#define KSLOW        (15000)
#define KSLOWLEFT  (16000)
#define TURNSLOW    (13000)
#define IOTTURN        (17000)
#define SLOWLEFT       (12000)
#define SLOWRIGHT       (11500)
#define RIGHTARCH (13000)
#define LEFTARCH  (16500)
#define FAST     (32000)
#define FASTLEFT     (47000)
//PWM SPEEDS
#define BEGINNING  (0)
#define SMALL_RING_SIZE (16)
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)
#define DAC_IDLE  (0)
#define DAC_RUNNING (1)
#define DAC_DONE (2)
#define SECRET_PIN       "6365"
#define PIN_LENGTH       4
#define CMD_DIR_INDEX    5    // position of direction char in "^1234D####"
#define CMD_DUR_INDEX    6    // start of duration digits


#define DAC_Begin           (2725) // 2v
#define DAC_Limit           (1500) //
#define DAC_Adjust          (1505) //

#endif /* MACROS_H_ */
