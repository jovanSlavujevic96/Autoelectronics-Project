#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h> 
#include <p30fxxxx.h>
 
 /// Niz char u kom se smesta vrednost sa UART-a.
extern volatile char Serial[20];

/// Ako su podaci na uartu spremni za citanje setuje se ova promenljiva 
extern volatile _Bool SerialReady; 

/// Potrebno za MCP biblioteku, Da li je extended ili ne. 
extern volatile uint8_t m_nExtFlg;

/// ID poruke Extended (29 bit) or Standard (11 bit)
extern volatile uint32_t m_nID;  

 /// Broj karaktera poruke
extern volatile uint8_t m_nDlc;   

 /// Niz u kome se smestaju vrednosti poruke
extern volatile uint8_t m_nDta[8];  

/// Da li je Remote poruka
extern volatile uint8_t m_nRtr; 

  /// Za filtere - ne koristi se!
extern volatile uint8_t m_nfilhit; 

/// CS pin - Ne koristi se !
extern volatile uint8_t MCPCS;  

 /// Mod rada MCP cipa
extern volatile uint8_t mcpMode;    

/// frekvencija oscilatora za baudrate,postaviti 2 puta manju od frekvencije kvarca, ako se pri programiranju postavi HS/2 w/PLL 4 
#define FCY (uint32_t)8000000  

#endif
