#include "piezo_tajmer.h"

int ms=0, brojac_ms=0, stoperica=0, sekund =0;

void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{
    IPC1bits.T2IP=0b0;
    TMR2 =0;
    ms=1;//fleg za milisekundu ili prekid;potrebno ga je samo resetovati u funkciji

	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms 
    
    if (brojac_ms==1000)//sek
    {
      brojac_ms=0;
      sekund=1;//fleg za sekundu
    } 
	IFS0bits.T2IF = 0;  
}

