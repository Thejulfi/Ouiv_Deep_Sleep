/*
  SigFox First Configuration

  This sketch demonstrates the usage of MKRFox1200 SigFox module.
  Since the board is designed with low power in mind, it depends directly on ArduinoLowPower library

  This example code is in the public domain.
*/
/*
 * Result of power consumption is quite bad 2017-09-07
 * 1.1 mA residual current in sleep mode - multimeter in serie
 * 110 mV measured accross a serial 100 Ohms resistor - Multimeter measures voltage : 120 mVolt drop
 * If schematics are not buggy (tbc), perhaps Arduino Low Power library ddoesn't do enough in software ...
 */
// 1. Initialisation
//******************************************************************
#include <Arduino.h>

#include <SigFox.h>
#include <ArduinoLowPower.h>

#define SIGFOX_SPI

#define attach_interrupt 1
#define Rled 2


// 2. Setup
//******************************************************************

void setup() {
  delay(5000); //délais pour laisser le temps de charger un code arduino
  pinMode(Rled, OUTPUT);
  
  SigFox.begin();
  delay(200);
  SigFox.end();
  delay(200);
 
  //------------------------------------------------------------------
  //  2.1. Low power configuration
 
  pinMode(attach_interrupt, INPUT_PULLUP);
  LowPower.attachInterruptWakeup(attach_interrupt, dummy, FALLING);

}



// 3. Loop
//******************************************************************
void loop() {


  LedBlink(500,10); //10 clignotements de la LED avec un délai de 500ms

  SigFox.begin();
  delay(200);
  SigFox.end();
  delay(200);
  
  LowPower.deepSleep();
 
}


void dummy() {
  // Fonction appelé au réveil du système
  // Ne pas mettre de délai et/ou des fonctions qui mettraient du temps à s'exécuter, car c'est une fonction qui se lance dans le contexte d'une interruption
}


void LedBlink(int TiBk,int NbBk) {
  int i=0;
  while (i++<NbBk) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(TiBk);
    digitalWrite(LED_BUILTIN, LOW);
    delay(TiBk);
  }
  }
