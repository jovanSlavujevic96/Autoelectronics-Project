// Arduino CAN Send // PROJEKAT AUTO

#include <mcp_can.h>
#include <SPI.h>
#define CMDBUFFER_SIZE 32

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10

int incrementer;

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  incrementer = 0;
}

void serialEvent();
void reset(char *string) ;
char processCharInput(char* cmdBuffer, const char c);
void slanje_data(byte data[], char *string);
void prijem_loop(int &i);

byte data[2] = {0};

unsigned char rxBuf[4] = {0};

void loop()
{
 serialEvent();
 prijem_loop(incrementer);
}

void serialEvent()
{
 static char cmdBuffer[CMDBUFFER_SIZE] = "";
 char c;
 while(Serial.available()) 
 {
   c = processCharInput(cmdBuffer, Serial.read());
   if (c == '\n') 
   {
     //Full command received. Do your stuff here!      
     if (strcmp("START", cmdBuffer) == 0)  // STARTTTTTTTTTTTTTT
     {
          data[0]=0x01;
          data[1]=0;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }
     if (strcmp("STOP", cmdBuffer) == 0)    // STOPPPPP
     {
          data[0]=0x02;
          data[1]=0;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }
     if (strcmp("CALMIN1", cmdBuffer) == 0)    // KALIBRACIJA min
     {
          data[0]=0x10;
          data[1]=0x1;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }
     if (strcmp("CALMAX1", cmdBuffer) == 0)    // KALIBRACIJA max
     {
          data[0]=0x11;
          data[1]=0x1;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }

     if (strcmp("CALMIN2", cmdBuffer) == 0)    // KALIBRACIJA min
     {
          data[0]=0x10;
          data[1]=0x2;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }
     if (strcmp("CALMAX2", cmdBuffer) == 0)    // KALIBRACIJA max
     {
          data[0]=0x11;
          data[1]=0x2;
          slanje_data(data,cmdBuffer);
          reset(cmdBuffer);
          return;
     }
     
     reset(cmdBuffer);
     return;
   }
 }
}

void slanje_data(byte data[], char *string)
{
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 2, data);
  if(sndStat == CAN_OK){
    Serial.print("Message ");
    Serial.print(string);
    Serial.println(" Sent Successfully!");
    } else {
    Serial.println("Error Sending Message...");
    }
    delay(100);   // send data per 100ms
}

void reset(char *string) 
{
  for(int i=0;i<strlen(string);i++) 
    string[i]=0;
}

char processCharInput(char* cmdBuffer, const char c)
{
 //Store the character in the input buffer
 if (c >= 47 && c <= 123) //Ignore control characters and special ascii characters 38   126
 {
   if (strlen(cmdBuffer) < CMDBUFFER_SIZE) 
   { 
     strncat(cmdBuffer, &c, 1);   //Add it to the buffer
   }
   else  
   {   
     return '\n';
   }
 }
 else if ((c == 8 || c == 127) && cmdBuffer[0] != 0) //Backspace
 {
   cmdBuffer[strlen(cmdBuffer)-1] = 0;
 }

 return c;
}

void prijem_loop(int &i)
{
  int printkey = 1;
  if(!digitalRead(CAN0_INT) )
  {
    unsigned long int tmp1; 
    unsigned char tmp2;
    CAN0.readMsgBuf(&tmp1, &tmp2, rxBuf);
    printkey = 0;  
  }
  if(!printkey)
  { 
    Serial.println(); 
    Serial.print(i);
    Serial.print(" Senzor 1: ");
    Serial.print( (int)(rxBuf[0]+rxBuf[1]) );
    Serial.print(" cm        ");
    Serial.print("Senzor 2: ");
    Serial.print( (int)(rxBuf[2]+rxBuf[3]) );
    Serial.println(" cm");
    Serial.println();
    
    ++i;
    printkey = 1;
  }
    
}
