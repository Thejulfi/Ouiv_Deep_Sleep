#define interruptPin 1
#define Rled 2
#define Jled 3
//#define Bled 4


volatile bool SLEEP_FLAG;

void EIC_ISR(void) {
  //  digitalWrite(Bled, HIGH);
  
  SLEEP_FLAG ^= true;  // toggle SLEEP_FLAG by XORing it against true


//  Serial.print("EIC_ISR SLEEP_FLAG = ");
//  Serial.println(SLEEP_FLAG);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  while (!Serial) {}  // wait for Serial to initialize
  
  delay(1000); // wait for console opening


  pinMode(Rled, OUTPUT);
  pinMode(Jled, OUTPUT);
//  pinMode(Bled, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, CHANGE);  // Attach interrupt to pin 6 with an ISR and when the pin state CHANGEs

  SYSCTRL->XOSC32K.reg |=  (SYSCTRL_XOSC32K_RUNSTDBY | SYSCTRL_XOSC32K_ONDEMAND); // set external 32k oscillator to run when idle or sleep mode is chosen
  REG_GCLK_CLKCTRL  |= GCLK_CLKCTRL_ID(GCM_EIC) |  // generic clock multiplexer id for the external interrupt controller
                       GCLK_CLKCTRL_GEN_GCLK1 |  // generic clock 1 which is xosc32k
                       GCLK_CLKCTRL_CLKEN;       // enable it
  while (GCLK->STATUS.bit.SYNCBUSY);              // write protected, wait for sync

  EIC->WAKEUP.reg |= EIC_WAKEUP_WAKEUPEN7;        // Set External Interrupt Controller to use channel 4 (pin 1)

  
  PM->SLEEP.reg |= PM_SLEEP_IDLE_CPU;  // Enable Idle0 mode - sleep CPU clock only
  //PM->SLEEP.reg |= PM_SLEEP_IDLE_AHB; // Idle1 - sleep CPU and AHB clocks
  //PM->SLEEP.reg |= PM_SLEEP_IDLE_APB; // Idle2 - sleep CPU, AHB, and APB clocks

  // It is either Idle mode or Standby mode, not both. 
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;   // Enable Standby or "deep sleep" mode

  SLEEP_FLAG = true; // begin awake

  // Built-in LED set to output and high
  //PORT->Group[g_APinDescription[Rled].ulPort].DIRSET.reg = (uint32_t)(1<<g_APinDescription[Rled].ulPin);  // set pin direction to output
  //PORT->Group[g_APinDescription[Rled].ulPort].OUTSET.reg = (uint32_t)(1<<g_APinDescription[Rled].ulPin); // set pin mode to high
  digitalWrite(Rled, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (SLEEP_FLAG == true) {
    PORT->Group[g_APinDescription[Rled].ulPort].OUTCLR.reg = (uint32_t)(1<<g_APinDescription[Rled].ulPin); // set pin mode to low
    Serial.println("I'm going to sleep now.");
    Serial.end();
      __WFI();  // wake from interrupt
     SLEEP_FLAG = false;
     digitalWrite(Jled, HIGH);
     delay(600);
     digitalWrite(Jled, LOW);
//     Serial.println("Ok, I'm awake");
//     Serial.println();
  }
//  Serial.print("SLEEP_FLAG = ");
//  Serial.println(SLEEP_FLAG);
  

  PORT->Group[g_APinDescription[Rled].ulPort].OUTTGL.reg = (uint32_t)(1<<g_APinDescription[Rled].ulPin);  // toggle output of built-in LED pin
  delay(1000);

}
