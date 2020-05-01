/*! \file SerialCom.c
    \brief definicije za UART
    
 Definicije funkcija za Uart definisane u h fajlu
*/
#include "SerialCom.h"
 


/// temp za prijem promenljiva
unsigned char tempRX='\0';
volatile bool SerialReady=false;
bool S=false;
volatile char Serial[20]={'\0'};
unsigned int BrojUStringu=0,i=0;

void initUART1(void)
{
    /// ovim odredjujemo baudrate
    U1BRG=UBRG1_VALUE;

    /// biramo koje pinove koristimo za komunikaciju osnovne ili alternativne
    U1MODEbits.ALTIO=1;

    /// omogucavamo rx1 interupt
    IEC0bits.U1RXIE=1;

    U1STA&=0xfffc;

    /// ukljucujemo ovaj modul
    U1MODEbits.UARTEN=1;

    /// ukljucujemo predaju
    U1STAbits.UTXEN=1;
}

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void) 
{
    
    tempRX = U1RXREG;
 
    Serial[BrojUStringu]=tempRX;
    
    if(Serial[BrojUStringu]=='/' && Serial[BrojUStringu-1]=='/')
    {
            Serial[BrojUStringu]='\0';
            Serial[BrojUStringu-1]='\0';
            for(i=BrojUStringu;i<20;i++) Serial[BrojUStringu]='\0';
            SerialReady=true;
            BrojUStringu=0;
    }
    else
    { BrojUStringu++; SerialReady=false;}

    IFS0bits.U1RXIF = 0;
    
        
} 

bool SerialReadyFunc(void)
{
    S=SerialReady;
    SerialReady=false;
    return S;
}



void RS232_putst(register const char *str)
{
    while ((*str)!=0)
    {
        WriteUART1(*str);      
        str++;
    }

}

void WriteUART1(uint8_t data)
{
    /// da li je prethodno slanje zavrseno
	  while(!U1STAbits.TRMT);

      ///da li ima deveti bit
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART2dec2string(uint8_t data)
{
	unsigned char temp;

	temp=data/1000;
	WriteUART1(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART1(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART1(temp+'0');
	data=data-temp*10;
	WriteUART1(data+'0');
}
