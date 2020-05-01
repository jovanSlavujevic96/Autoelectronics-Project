#ifndef PIEZO_TAJMER_H
#define PIEZO_TAJMER_H

#include <outcompare.h>

/* List of SFRs for Timers */
/* This list contains the SFRs with default (POR) values to be used for configuring Timers */
/* The user can modify this based on the requirement */
#define TMRx_VALUE          0x0000
#define PRx_VALUE           0xFFFFU
#define TxCON_VALUE         0x0000

/* Timer1 Control Register (T1CON) Bit Defines */

#define T2_ON               0xffffu      /* Timer1 ON */
#define T2_OFF              0x7fff      /* Timer1 OFF */

#define T2_IDLE_CON         0xdfffu      /* operate during sleep */
#define T2_IDLE_STOP        0xffffu      /* stop operation during sleep */

#define T2_GATE_ON          0xffffu      /* Timer Gate time accumulation enabled */
#define T2_GATE_OFF         0xffbfu      /* Timer Gate time accumulation disabled */

#define T2_PS_1_1           0xffcfu      /* Prescaler 1:1 */
#define T2_PS_1_8           0xffdfu      /*           1:8 */
#define T2_PS_1_64          0xffefu      /*          1:64 */
#define T2_PS_1_256         0xffffu      /*         1:256 */

#define T2_SYNC_EXT_ON      0xffffu      /* Synch external clk input */
#define T2_SYNC_EXT_OFF     0xfffbu      /* Do not synch external clk input */

#define T2_SOURCE_EXT       0xffffu      /* External clock source */
#define T2_SOURCE_INT       0xfffdu     /* Internal clock source */

/* defines for Timer Interrupts */

#define T2_INT_PRIOR_7      0xffffu      /* 111 = Interrupt is priority 7 */
#define T2_INT_PRIOR_6      0xfffeu      /* 110 = Interrupt is priority 6 */
#define T2_INT_PRIOR_5      0xfffdu      /* 101 = Interrupt is priority 5 */
#define T2_INT_PRIOR_4      0xfffcu      /* 100 = Interrupt is priority 4 */
#define T2_INT_PRIOR_3      0xfffbu      /* 011 = Interrupt is priority 3 */
#define T2_INT_PRIOR_2      0xfffau      /* 010 = Interrupt is priority 2 */
#define T2_INT_PRIOR_1      0xfff9u      /* 001 = Interrupt is priority 1 */
#define T2_INT_PRIOR_0      0xfff8u      /* 000 = Interrupt is priority 0 */

#define T2_INT_ON           0xffffu      /* Interrupt Enable */
#define T2_INT_OFF          0xfff7u      /* Interrupt Disable */

/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 1*/
#define EnableIntT2                    _T2IE = 1
#define DisableIntT2                   _T2IE = 0
#define SetPriorityIntT2(priority)     _T2IP = priority

#endif
