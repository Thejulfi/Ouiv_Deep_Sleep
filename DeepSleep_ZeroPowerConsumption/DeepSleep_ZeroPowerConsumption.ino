#include  "ZeroPowerManager.h"

const byte interruptPin = 1;
//FLags de gestion du mode de fonctionnement
volatile int SLEEP_FLAG; //1 si autorisé à dormir, 0 sinon
#define Rled 2


void blink() {
  SLEEP_FLAG = 0;
}

void setup()
{    
    zpmRTCInit();   
    pinMode(Rled, OUTPUT);
    
    /*
     * To get to datasheet power levels the I/O must be disabled
     */
    zpmPortDisableDigital();
    zpmPortDisableSPI();
    zpmPortDisableUSB();
    
       
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
    SLEEP_FLAG = 0;

}



void loop()
{    
    /*
     * Stay in each power state for about a second.
     */
     if(SLEEP_FLAG){
        zpmSleep();
     }else{
      for(int i = 0;i<10;i++){
        digitalWrite(Rled, HIGH);
        delay(500);
        digitalWrite(Rled, LOW);
        delay(500);
      }
      SLEEP_FLAG = 1;
     }
//    zpmCPUClk48M();
//    delay(1000);
//  
//    zpmCPUClk8M();    
//    delay(1000);
//    
//    zpmCPUClk32K(); 
//    delay(1000);
//    
    /* 
     * There is no interrupt handler (callback = NULL). Execution will
     * resume from the sleep instruction.
     */ 
    //uint32_t now = zpmRTCGetClock();
    //zpmRTCInterruptAt(now + 1000, NULL);
}
