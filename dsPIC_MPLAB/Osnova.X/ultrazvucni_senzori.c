
#include "ultrazvucni_senzori.h"

//prvi senzor

uint8_t kljuc5=0;
uint64_t brojac_ulaz=0;

uint8_t kljuc52=0;
uint64_t brojac_ulaz2=0;

void Init_Digi(void)
{
    ADPCFGbits.PCFG1=1; // Digitalni pin , TRIG
    ADPCFGbits.PCFG2=1; // Digi pin , ECHO
    TRISBbits.TRISB1=0; // Trig izlazni pin
    TRISBbits.TRISB2=1; // ECHO ulazni pin 
}

void Init_T1(void)
{//Tajmer broji koliko dugo je Eho aktivan i generise 10uS impuls
    TMR1 = 0;
    PR1 = 0; //60 000// Interrapt na svakih 6 ms
    T1CONbits.TCS = 0;
     
    IFS0bits.T1IF = 0; // clear interrupt flag
	IEC0bits.T1IE = 1; // enable interrupt
    IPC0bits.T1IP=3;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    
    if(kljuc5==1){    
        brojac_ulaz++;// Ova promenljiva broji koliko puta je T1 odbrojio do 6ms
    } 
        
    if (kljuc5==0){   
        T1CONbits.TON=0; 
        LATBbits.LATB1=0; // Ugasi TRIG signal
        TMR1=0;
        PR1 = TMR1_period; //60 000
        kljuc5=1;

        while(PORTBbits.RB2==0);// Sacekaj da se upali EHO
        T1CONbits.TON=1; 
    }
    
    IFS0bits.T1IF = 0;    
}
 

uint16_t digi_ocitaj_prvi()
{
    uint16_t  digiCM=0; // Tacno 16 bita  
    uint16_t  vrednost_brojaca=0;

    TMR1=0; // Odakle krece da broji
    PR1=impuls; // generisanje impulsa od 10uS
    LATBbits.LATB1=1; // Upali TRIG signal
    T1CONbits.TON=1; // Upali brojac

    while(kljuc5==0); //

    while(PORTBbits.RB2==1);// Ovde se zastekaj i broji koliko je dugo EHO aktivan

    T1CONbits.TON=0; 

    vrednost_brojaca=TMR1;

    if(brojac_ulaz>3)
        digiCM=400;

    if(brojac_ulaz<=3)
        digiCM=(17000*(vrednost_brojaca*0.1+brojac_ulaz*6000))/1000000; // u CM

    kljuc5=0;
    brojac_ulaz=0;

    return digiCM;
}


//drugi senzor

void Init_Digi2(void)
{
    TRISDbits.TRISD1=0; // Trig izlazni pin
    TRISDbits.TRISD2=1; // ECHO ulazni pin B2 eho , B1 trig
}

 void Init_T5(void)
 {//Tajmer broji koliko dugo je Eho aktivan i generise 10uS impuls
    TMR5 = 0;
    PR5 = 0; //60 000// Interrapt na svakih 6 ms
    T5CONbits.TCS = 0;
     
    IFS1bits.T5IF = 0; // clear interrupt flag
	IEC1bits.T5IE = 1; // enable interrupt
    IPC5bits.T5IP= 3;
}
 
void __attribute__((interrupt, auto_psv)) _T5Interrupt(void)
{
    
    if(kljuc52==1)
        brojac_ulaz2++;// Ova promenljiva broji koliko puta je T1 odbrojio do 6ms
       
    if(kljuc52==0)
    {     
        T5CONbits.TON=0; 
        LATDbits.LATD1=0; // Ugasi TRIG signal
        TMR5=0;
        PR5 = TMR1_period; //60 000
        kljuc52=1;

        while(PORTDbits.RD2==0);// Sacekaj da se upali EHO
        T5CONbits.TON=1; 
    }
    IFS1bits.T5IF = 0; 
}
 
uint16_t digi_ocitaj_drugi()
{
    uint16_t digiCM2=0; // Tacno 16 bita 
    uint16_t vrednost_brojaca=0;    

    TMR5=0; // Odakle krece da broji
    PR5=impuls; // generisanje impulsa od 10uS
    LATDbits.LATD1=1; // Upali TRIG signal
    T5CONbits.TON=1; // Upali brojac

    while(kljuc52==0); //

    while(PORTDbits.RD2==1);// Ovde se zastekaj i broji koliko je dugo EHO aktivan

    T5CONbits.TON=0; 

    vrednost_brojaca=TMR5;

    if(brojac_ulaz2>3)
        digiCM2=400;

    if(brojac_ulaz2<=3)
        digiCM2=(17000*(vrednost_brojaca*0.1+brojac_ulaz2*6000))/1000000; // u CM

    kljuc52=0;
    brojac_ulaz2=0;

    return digiCM2;
}

void Init_senzori(void) 
{
    Init_Digi();
    Init_T1();
    Init_Digi2();
    Init_T5();
    TRISDbits.TRISD9=0; //LEDOVKA ZA PALJENJE/GASENJE PRI GENERISANJU ZVUCNOG SIGNALA //IZLAZ
}