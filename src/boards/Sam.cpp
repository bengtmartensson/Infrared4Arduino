// Support routines for SAM processor boards

// Largely based on https://forum.arduino.cc/index.php?topic=346731.0


#include "IrReceiverSampler.h"
#include "boarddefs.h" // includes sam.h

#if (defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)) & ! defined(ARDUINO_SAM_DUE)

// "Idiot check"
#ifdef USE_DEFAULT_ENABLE_IR_IN
#error Must undef USE_DEFAULT_ENABLE_IR_IN
#endif

// following based on setup from GitHub jdneo/timerInterrupt.ino
void Sam::setTimerFrequency(frequency_t frequencyHz) {
    int compareValue = (F_CPU / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
    //Serial.println(compareValue);
    TcCount16* TC = (TcCount16*) TC3;
    // Make sure the count is in a proportional position to where it was
    // to prevent any jitter or disconnect when changing the compare value.
    TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
    TC->CC[0].reg = compareValue;
    //Serial.print("COUNT.reg ");
    //Serial.println(TC->COUNT.reg);
    //Serial.print("CC[0].reg ");
    //Serial.println(TC->CC[0].reg);
    while (TC->STATUS.bit.SYNCBUSY == 1);
}

void Sam::startTimer() {
    REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3);
    while (GCLK->STATUS.bit.SYNCBUSY == 1); // wait for sync

    TcCount16* TC = (TcCount16*) TC3;

    TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Use the 16-bit timer
    TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Use match mode so that the timer counter resets when the count matches the compare register
    TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    // Set prescaler to 1024
    //TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
    TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

    setTimerFrequency(1000000 / USECPERTICK);

    // Enable the compare interrupt
    TC->INTENSET.reg = 0;
    TC->INTENSET.bit.MC0 = 1;

    NVIC_EnableIRQ(TC3_IRQn);

    TC->CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}

// was IRrecv::enableIRIn() in IRremote
void IrReceiverSampler::enable() {
    Sam::startTimer();
    reset();
    // Set pin mode was done in super class' constructor
}

void interruptServiceRoutine(); // Defined in IRRemote as ISR(TIMER_INTR_NAME)

// Is used!!
void TC3_Handler() {
    TcCount16* TC = (TcCount16*) TC3;
    // If this interrupt is due to the compare register matching the timer count
    // we toggle the LED.
    if (TC->INTFLAG.bit.MC0 == 1) {
        TC->INTFLAG.bit.MC0 = 1;
        interruptServiceRoutine();
    }
}

// Output 30kHz PWM on timer TCC1/W0[0] (D8) and TCC0/WO[6] (D11) (10-bit resolution)
void Sam::pwm_init(frequency_t hz) {
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                     GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                      GCLK_GENCTRL_GENEN |         // Enable GCLK4
                      GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                      GCLK_GENCTRL_ID(4);          // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  // Feed GCLK4 to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4
                      GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                      GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK4 to TCC0 and TCC1
  while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

  // Enable the port multiplexer for the PWM channel on pin D8 and D11
  PORT->Group[g_APinDescription[8].ulPort].PINCFG[g_APinDescription[8].ulPin].bit.PMUXEN = 1;
  //PORT->Group[g_APinDescription[11].ulPort].PINCFG[g_APinDescription[11].ulPin].bit.PMUXEN = 1;

  // Connect the TCC1 timer to the port outputs - port pins are paired odd PMUXO and even PMUXE
  // F & E peripherals specify the timers: TCC0, TCC1 and TCC2
  PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
  //PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;

  // Normal (single slope) PWM operation: timers continuously count up to PER register value and then is reset to 0
  TCC1->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;         // Setup single slope PWM on TCC1
  while (TCC1->SYNCBUSY.bit.WAVE);                 // Wait for synchronization
//  TCC0->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;         // Setup single slope PWM on TCC0
//  while (TCC0->SYNCBUSY.bit.WAVE);                 // Wait for synchronization

  // Each timer counts up to a maximum or TOP value set by the PER register,
  // this determines the frequency of the PWM operation: 1600 = 30kHz
  uint32_t period = F_CPU / (timer_prescale * hz) - 1;
  TCC1->PER.reg = period;                            // Set the frequency of the PWM on TCC1 to 30kHz
  while (TCC1->SYNCBUSY.bit.PER)                  // Wait for synchronization
      ;
//  TCC0->PER.reg = period;                            // Set the frequency of the PWM on TCC0 to 30kHz
//  while (TCC0->SYNCBUSY.bit.PER);                  // Wait for synchronization

    // The CCx register value corresponds to the pulsewidth in microseconds (us)
    uint32_t pulseWidth = dutyCyclePercent * period / 100;
    TCC1->CC[0].reg = pulseWidth;                           // Set the duty cycle of the PWM on TCC1 to 50%
    //TCC1->CC[1].reg = pulseWidth;
    while (TCC1->SYNCBUSY.bit.CC1)                  // Wait for synchronization
        ;
//    TCC0->CC[2].reg = pulseWidth;                           // Set the duty cycle of the PWM on TCC0 to 50%
//    while (TCC0->SYNCBUSY.bit.CC0);                  // Wait for synchronization
}

void Sam::enable_pwm() {
    TCC1->CTRLA.bit.ENABLE = 1;                     // Enable TCC1 timer
    while (TCC1->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
//    TCC0->CTRLA.bit.ENABLE = 1;                     // Enable TCC0 timer
//    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
}

void Sam::disable_pwm() {
    TCC1->CTRLA.bit.ENABLE = 0;                     // Disable TCC1 timer
    while (TCC1->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
//    TCC0->CTRLA.bit.ENABLE = 0;                     // Disable TCC0 timer
//    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization
}

#endif // defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)