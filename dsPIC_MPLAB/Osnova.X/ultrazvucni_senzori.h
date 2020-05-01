#ifndef ULTRAZVUCNI_SENZORI_H
#define ULTRAZVUCNI_SENZORI_H

#include "globals.h" 
#include <libpic30.h>


#define TMR1_period 60000 // 6ms
#define impuls 100 // 10uS

void Init_Digi(void);
void Init_T1(void);
uint16_t digi_ocitaj_prvi(void);

void Init_Digi2(void);
void Init_T5(void);
uint16_t digi_ocitaj_drugi(void);

void Init_senzori(void);

#endif
