#include <pl_defs.h>
#include <sam.h>

__attribute__((interrupt)) void NonMaskableInt_Handler() {
  while(1) {
    for(int i = 0; i < 0x8000; i++) {
      __NOP();
    }

    PORTA.OUTTGL.reg = PORT_PA15;
  }
}

int main() {
  EIC->NMICTRL.reg = EIC_NMICTRL_NMISENSE_RISE;
  EIC->CTRLA.reg   = EIC_CTRLA_CKSEL | EIC_CTRLA_ENABLE;
  PORTA.DIRSET.reg = PORT_PA15;
  PORTA.OUTSET.reg = PORT_PA15;
  //NonMaskableInt_Handler();

  __enable_irq();
  __NVIC_EnableIRQ(NonMaskableInt_IRQn);
  PORTA.PINCFG[8].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
  PORTA.PMUX[4].reg   = PORT_PMUX_PMUXE(PINMUX_PA08A_EIC_NMI);
}