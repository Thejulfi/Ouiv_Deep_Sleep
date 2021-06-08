#include "ArduinoLowPower.h"

#define interruptPin 1
#define Rled 2
//#define Jled 3
//#define Bled 4


bool exec_routine = false;


void setup() {
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial to initialize

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  pinMode(Rled, OUTPUT);
//  pinMode(Jled, OUTPUT);
//  pinMode(Bled, OUTPUT);

  LowPower.attachInterruptWakeup(interruptPin, wakeup_handler, FALLING );
}


void loop() {
  routine();
  
  if(exec_routine){
    LowPower.deepSleep();
  }
}

void wakeup_handler() {
  exec_routine = false;
  delay(500);

}

void routine(){
  for(int i =0;i<10;i++){
    digitalWrite(Rled, HIGH);
    delay(500);
    digitalWrite(Rled, LOW);
    delay(500);
  }
  exec_routine = true;
}
