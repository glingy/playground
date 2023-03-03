#include "main.h"

#include <pl.h>
#include <pl/clocks.h>
#include <pl/conf.h>

PL_BOARD_ID(PL_BOARD_HEADNODE_ID);

void check_pack() {}

void TC0_Handler() {
  TC0->COUNT8.INTFLAG.reg = TC_INTFLAG_OVF;
  check_pack();
}

int main() {
  // hall_check();
  pl_init();
  __NVIC_EnableIRQ(SDADC_IRQn);
  __enable_irq();
  pl_busy_wait_ms(48000, 200);

  __NVIC_EnableIRQ(TC0_IRQn);

  while (1) {
    __WFI();
  }
}