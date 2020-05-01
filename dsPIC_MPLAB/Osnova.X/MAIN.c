#include "SerialCom.h"
#include "mcp_can.h"

#include "ultrazvucni_senzori.h"
#include "piezo_tajmer.h"

//deklaracije funkcija
void CAN_init(void);
void CAN_receive(void) ;
_Bool getMsgReceived(void);

void Init_T3(void);
void __attribute__((interrupt, auto_psv)) _T3Interrupt(void);

void senzori(uint16_t Digi[] );

uint8_t command(void);
void kalibracija(const uint8_t komanda, uint16_t rawLow[], uint16_t rawHigh[], const uint16_t digi[], uint8_t *key) ;

void piezo_init(void);
void zvuk(const uint8_t st);
void pistanje(const _Bool st);
void LED(const _Bool st);
void switchBool(_Bool *b);

char IntToCh(const uint8_t number);
void IntToStr(const uint16_t number, char numberStr[]); 
void write13(const uint8_t loop_rate);

void parsiranje_podatka(const uint16_t digi[2], unsigned char data[] );
int8_t main(void) ;

//komandne konstante
static const uint8_t START = 1, STOP = 2, CALMIN1 = 3, CALMAX1 = 4, CALMIN2 = 5, CALMAX2 = 6;

//tajmerske promenljive
static uint16_t brojac_ms=0, stoperica=0, sekund =0;

static _Bool MsgReceived = false;

//! promenljive za argumente funkcija za CAN */
static uint32_t rxId;
static uint8_t len;
static char rxBuf[8];

/// Glavna funkcija
int8_t main(void) 
{    
    //stabilizacija sistema
    __delay_ms((uint16_t) 500);
    
    //inicijalizacija sistema
    initUART1();
    InitSPIPin();
    CAN_init();
    Init_senzori();
    piezo_init();
    Init_T3();
    stoperica = 0;
    
    //definisianje lokalnih variabli
    uint16_t digi[2]={0}; //promenljiva za ocitavanja senzora
    uint8_t komanda = 0; //promenljiva za ocitavanje komandi    
    uint16_t rawHigh[2] = {0}, rawLow[2] = {0}; //promenljive za gornjih i donjih granica kalibracije
    int CorrectedValue1 = 0, CorrectedValue2 = 0, COR=1000; //promenljive za ocitavanje kalibrisanih vrednosti
    char pomocni1, pomocni2;
    uint8_t key = 0, print_key = 0;
    int8_t incr = -1;
    _Bool Bool;
    char broj[6] = {0};
    
    unsigned char data_Ard[4] = {0};
    
    ///while koji se stalno vrti
    while(true)
    {      
        CAN_receive();
        
        ////dodela komande sa CAN-a ako postoji
        komanda = command();
        
        if( komanda != START && komanda != STOP )
        {
            if( getMsgReceived() )
            {
                kalibracija( komanda, rawLow, rawHigh, digi, &key);
            }
        }
        
        if( komanda == START || komanda == STOP )
        {
            if( getMsgReceived() )
            {
                zvuk(komanda);
            }
        }
        ////
        
        ////dominatna kalibrisana vrednost koja odredjuje kakvo ce biti pistanje pieza
        if((CorrectedValue1 < CorrectedValue2  && key == 4) || key == 2) //KALIB OBA ili KALIB SAMO PRVI
        {
            COR = CorrectedValue1;
        }
        if((CorrectedValue1 >= CorrectedValue2 && key == 4) || key == 3) //KALIB OBA ili KALIB SAMO DRUGI
        {
            COR = CorrectedValue2;
        }
        if(key == 1)  //NIJEDAN NIJE KALIB
        {
            COR = 1000; //u suprotnom ne pisti
        }
          
        if(COR < 0) 
        {
            pistanje(true);
            LED(true);
        }
        if(COR > 100)
        {
            pistanje(false);
            LED(false);
        }
        
        if(stoperica == 125 && COR >= 0 && COR <= 50) 
        {
            switchBool(&Bool);
            //Bool = !Bool;         
            pistanje(Bool);
            LED(Bool);
        }
        
        if(stoperica == 250 )
        {
            if( (COR >= 0 && COR <= 50) || (COR > 50 && COR <= 100) ) 
            {
                switchBool(&Bool);
                //Bool = !Bool;   //odjednom ne radi      
                pistanje(Bool);
                LED(Bool);  
            }
            print_key = 0; 
            
            ++incr;  
            if(incr == 2) 
            {
                incr = 0;
            }
            stoperica = 0;
        }
        
        if(print_key == 0 && incr == 1)
        { //ispis na pola sekunde
            WriteUART1(13);
            senzori(digi);
            parsiranje_podatka(digi, data_Ard);
            
            ////dodela korigovanim vrednostima oba senzora, ukoliko je izvrsena kalibracija
            if( (rawHigh[0]-rawLow[0]) != 0) 
            {
                CorrectedValue1 = digi[0] - rawLow[0];
                CorrectedValue1 *= (100 / (rawHigh[0]-rawLow[0]) ) ;
            }

            if( (rawHigh[1]-rawLow[1]) != 0) 
            {
                CorrectedValue2 = digi[1] - rawLow[1];
                CorrectedValue2 *= (100 / (rawHigh[1]-rawLow[1]) ) ;
            }
            ////
            
            if(CorrectedValue1 < 0) 
            {
                pomocni1 = 0;
            }
            else if(CorrectedValue1 > 100) 
            {
                pomocni1 = 100;
            }
            else 
            {
                pomocni1 = CorrectedValue1;
            }
            /////
            
            if(CorrectedValue2 < 0) 
            {
                pomocni2 = 0;
            }
            else if(CorrectedValue2 > 100) 
            {   
                pomocni2 = 100;
            }
            else 
            {
                pomocni2 = CorrectedValue2;
            }
            /////
            
            if(key == 2|| key == 4)   
            {             
                RS232_putst("Kalibracija senzora1: ");
                IntToStr(pomocni1,broj);
                RS232_putst(broj), RS232_putst(" %");
                RS232_putst("                               Donja granica: ");
                IntToStr(rawLow[0], broj); 
                RS232_putst(broj);
                RS232_putst("        Gonja granica: ");
                IntToStr(rawHigh[0], broj); 
                RS232_putst(broj), WriteUART1(13);
            }
            if(key == 3 || key == 4) 
            {
                RS232_putst("Kalibracija senzora2: ");
                IntToStr(pomocni2,broj); 
                RS232_putst(broj); 
                RS232_putst(" %");
                RS232_putst("                               Donja granica: ");
                IntToStr(rawLow[1], broj); 
                RS232_putst(broj);
                RS232_putst("        Gonja granica: ");
                IntToStr(rawHigh[1], broj); 
                RS232_putst(broj); 
                WriteUART1(13);
            }
            WriteUART1(13);
            
            sendMsgBuf(0x00,4,data_Ard);
            print_key = 1;
        }
        
    }//while(1)
    return 0;
}//int main())

void CAN_init(void)
{
    if(begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    {
        setMode(MCP_NORMAL);
        WriteUART1(13);
        RS232_putst("CAN IS OK");
        WriteUART1(13);
        return;
    }
    WriteUART1(13);
    RS232_putst("CAN IS NOT OK");
    WriteUART1(13);
}

void CAN_receive(void) 
{    
    clearMsg();
    
    if(!PORTDbits.RD8) 
    {
        readMsgBuf(&rxId, &len, rxBuf);  

        RS232_putst("Message received!");           
        WriteUART1(13);
        
        MsgReceived = true;
    }   
}

_Bool getMsgReceived(void)
{
    if(MsgReceived)
    {
        MsgReceived = false;
        return true;
    }
    return false;
}

void Init_T3(void)
{
    #define TMR3_period 10000
	TMR3 = 0;
	PR3 = TMR3_period;
	
	T3CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	IPC1bits.T3IP = 7; // T2 interrupt pririty (0-7)
	IFS0bits.T3IF = 0; // clear interrupt flag
	IEC0bits.T3IE = 1; // enable interrupt

	T3CONbits.TON = 1; // T2 on 
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) // svakih 1ms
{
    //IPC1bits.T2IP=0b0;
    TMR3 =0;
   
	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms 
    
    if (brojac_ms==1000)//sek
    {
      brojac_ms=0;
      sekund=1;//fleg za sekundu
    } 
	IFS0bits.T3IF = 0;  
}

void senzori(uint16_t Digi[] )
{
    char broj[6]={0};
    Digi[0]=digi_ocitaj_prvi(); 
    Digi[1]=digi_ocitaj_drugi();
    
    RS232_putst("           Senzor1: ");
    IntToStr(Digi[0],broj);
    RS232_putst(broj);
    RS232_putst(" cm");
    
    RS232_putst("           Senzor2: ");
    IntToStr(Digi[1],broj);
    RS232_putst(broj);
    RS232_putst(" cm");
    WriteUART1(13);
}

uint8_t command(void)
{
    if(rxBuf[0] == 0x10) 
    {
        if(rxBuf[1] == 0x1) 
        {          
            return CALMIN1;
        }
        if(rxBuf[1] == 0x2) 
        {
            return CALMIN2;
        }
    }
    if(rxBuf[0] == 0x11) 
    {
        if(rxBuf[1] == 0x1)
        {    
            return CALMAX1;
        }
        if(rxBuf[1] == 0x2) 
        {
            return CALMAX2;
        }
    }
    if(rxBuf[1] == 0x0) 
    {
        if(rxBuf[0] == 0x1)
        {
            return START;
        }
        if(rxBuf[0] == 0x2)
        {
            return STOP;
        }
    }
    return 0;
}

void kalibracija(const uint8_t komanda, uint16_t rawLow[], uint16_t rawHigh[], const uint16_t digi[], uint8_t *key) 
{
    const char* StringPair1[2] = {"Donja", "Gornja"};
    const char* StringPair2[2] = {"prvog", "drugog"};
    uint8_t mem1, mem2;
    uint16_t temp;
    
    if(komanda == CALMIN1) 
    {
        rawLow[0] = digi[0];
        mem1 = 0;
        mem2 = 0;
        temp = rawLow[0];
    }
    if(komanda == CALMIN2) {
        rawLow[1] = digi[1];
        mem1 = 0;
        mem2 = 1;
        temp = rawLow[1];
    }
    if(komanda == CALMAX1){
        rawHigh[0] = digi[0];
        mem1 = 1;
        mem2 = 0;
        temp = rawHigh[0];
    }
    if(komanda == CALMAX2){
        rawHigh[1] = digi[1];
        mem1 = 1;
        mem2 = 1;
        temp = rawHigh[1];
    }
    
    if(komanda != -1)
    {
        write13(3);
        RS232_putst(StringPair1[mem1]); 
        RS232_putst(" granica "); 
        RS232_putst(StringPair2[mem2]); 
        RS232_putst(" senzora je: ");
        WriteUART2dec2string(temp);
        write13(3);
    }
    
    if( (rawLow[0] >= rawHigh[0] && rawHigh[0]  != 0) || (rawLow[1] >= rawHigh[1] && rawHigh[1]  != 0) )
    {
        write13(3);
        RS232_putst("Donja granica je veca od gornje !?!?");
        WriteUART1(13);
        RS232_putst("Ponovo kalibrisati");
        write13(3);
        
        if(rawLow[0] >= rawHigh[0] && rawHigh[0]  != 0)
        {
            rawLow[0] = 0; 
            rawHigh[0] = 0;
        }
        
        if(rawLow[1] >= rawHigh[1] && rawHigh[1]  != 0) 
        {
            rawLow[1] = 0;
            rawHigh[1] = 0;
        }
    }
    
    if(( rawHigh[0] == 0 || rawLow[0] == 0 )&&( rawHigh[1] == 0 || rawLow[1] == 0 ))
    {
        *key = 1;
    }
    if(( rawHigh[0] != 0 && rawLow[0] != 0 )&&( rawHigh[1] == 0 || rawLow[1] == 0 )) 
    {
        *key = 2;
    }
    if(( rawHigh[0] == 0 || rawLow[0] == 0 )&&( rawHigh[1] != 0 && rawLow[1] != 0 )) 
    {    
        *key = 3;
    }
    if(( rawHigh[0] != 0 && rawLow[0] != 0 )&&( rawHigh[1] != 0 && rawLow[1] != 0 )) 
    {
        *key = 4;
    }
}

void piezo_init(void)
{
    PR2=2499;//odredjuje frekvenciju po formuli
    OC1RS=1800;
    OC1R=1000;//inicijalni pwm pri paljenju samo
    OC1CON = OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
    TRISDbits.TRISD0=0;
}

void zvuk(const uint8_t st) 
{
    if(st == START)
    {
        OC1RS=1800;
        return;
    }
    OC1RS=0;
}

void pistanje(const _Bool st) 
{
    if(st) 
    {
        T2CONbits.TON=1;//ukljucujemo timer koji koristi
        return;
    }
    T2CONbits.TON=0;
}

void LED(const _Bool st) {
    if(st)
    {
        LATDbits.LATD9 = 1;
        return;
    }
    LATDbits.LATD9 = 0;
}

void switchBool(_Bool *b)
{
    if(*b)
    {
        *b = false;
        return;
    }
    *b = true;
}

char IntToCh(const uint8_t number)
{
    if(number >= 0 && number <= 9)
    {
        return ('0'+number);
    }   
    return 0;
}

void IntToStr(const uint16_t number, char numberStr[]) 
{
    uint16_t i;
    uint32_t tmp1000,tmp100,tmp10;
    
    if(number < 0)   
    {
        numberStr[0] = '-'; 
        i = number *(-1);
    }
    else        
    {
        numberStr[0] = '0'; 
        i = number;
    }
    
    if(i >= 0 && i <= 9)
    {
        numberStr[1] = '0'; 
        numberStr[2] = '0'; 
        numberStr[3] = '0'; 
        numberStr[4] = IntToCh(i);
        return;
    }
    if(i > 9  && i <= 99) 
    {
        numberStr[1] = '0';
        numberStr[2] = '0';
        tmp10 = i - (i%10);
        tmp10 /= 10;
        numberStr[3] = IntToCh(tmp10);
        numberStr[4] = IntToCh(i%10);
        return;
    }
    if(i > 99 && i <= 999) 
    {
        numberStr[1] = '0';
        tmp100 = i - (i%100);
        tmp100 /= 100;
        numberStr[2] = IntToCh(tmp100);
        tmp100 *= 100;
        tmp10 = i - tmp100;
        tmp10 -= (i%10);
        tmp10 /= 10;
        numberStr[3] = IntToCh(tmp10);
        numberStr[4] = IntToCh(i%10);
        return;
    }
    if(i > 999 && i <= 9999) 
    {
        tmp1000 = i - (i%1000);
        tmp1000 /= 1000;
        numberStr[1] = IntToCh(tmp1000);
        tmp1000 *= 1000;
        tmp100 = i - tmp1000;
        tmp100 -= (i%100);
        tmp100 /= 100;
        numberStr[2] = IntToCh(tmp100);
        tmp100 *= 100;
        tmp10 = i - tmp1000 - tmp100;
        tmp10 -= i%10;
        tmp10 /= 10;
        numberStr[3] = IntToCh(tmp10);
        numberStr[4] = IntToCh(i%10);
        return;
    }    
}

void write13(const uint8_t loop_rate)
{
    uint8_t tmp;
    for(tmp = 0; tmp < loop_rate; tmp ++)
    {
        WriteUART1(13);
    }
}

void parsiranje_podatka(const uint16_t digi[2], unsigned char data[] )
{
    uint8_t i;
    uint8_t raw[2][2] = { {0,1} , {2,3} };
    
    for(i = 0; i < 2; ++i)
    {
        if(digi[i] > 255)
        {
            data[ raw[i][0] ] = 255;
            data[ raw[i][1] ] = digi[i]-255;
        }
        else
        {
            data[ raw[i][0] ] = digi[i];
            data[ raw[i][1] ] = 0;
        }
    }
}
