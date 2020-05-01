/*! \file SerialCom.h
    \brief Deklaracije za Uart
    
    fajl u kome se nalaze sve deklaracije funkcija za uart.
*/

#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "globals.h"

/// baud rate (frekvencija je 4 puta manja pa je i baud rate 4 puta manji)
#define UART1_BAUD 38400
/// racunanje registra
#define UBRG1_VALUE (FCY/UART1_BAUD)/16 - 1

/// Inicijalizacije uarta.
void initUART1(void);

/// Slanje jednog karaktera uartom
void WriteUART1(uint8_t data);

/// Konverter cetvorocifrenog int-a u string i automatsko slanje uartom
void WriteUART2dec2string(uint8_t data);

/// Slanje stringa uartom
void RS232_putst(register const char *str);

/// Vra?a vrednost true ako su spremni podaci sa uarta za citanje u suprotnom false
bool SerialReadyFunc(void);




#endif