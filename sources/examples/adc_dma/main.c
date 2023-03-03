#include "adc.h"
#include "dmac.h"
#include "timer.h"

#include <pl_clocks.h>
#include <pl_defs.h>
#include <samc21.h>

volatile uint8_t adc_results[2] = { 0 };

__ALIGNED(4)
DmacDescriptor dmac[1] = {
  {
    .BTCTRL.reg
    = DMAC_BTCTRL_BEATSIZE_BYTE
    | DMAC_BTCTRL_DSTINC
    | DMAC_BTCTRL_STEPSIZE_X1
    | DMAC_BTCTRL_BLOCKACT_INT
    | DMAC_BTCTRL_EVOSEL_DISABLE
    | DMAC_BTCTRL_VALID,
    .BTCNT.reg    = 2,
    .SRCADDR.reg  = (uint32_t) &ADC0->RESULT.reg,
    .DSTADDR.reg  = (uint32_t) &adc_results[2],
    .DESCADDR.reg = (uint32_t) &dmac[0],
  },
};

__ALIGNED(4)
DmacDescriptor dmac_wrb[1];

/**
 * GCLK0 -> 48MHz
 * GCLK3 -> 32/4 = 8.192kHz for TC
 * GCLK4 -> 32.768kHz for ADC0
 * 
 * 
 * 
 * 
 * 
 * 
 **/
static inline void clocks_init() {
  GCLK->GENCTRL[0].reg
    = GCLK_GENCTRL_DIV(0)
    | GCLK_GENCTRL_GENEN
    | GCLK_GENCTRL_SRC_OSCULP32K;

  GCLK->GENCTRL[3].reg
    = GCLK_GENCTRL_DIV(0)
    | GCLK_GENCTRL_GENEN
    | GCLK_GENCTRL_SRC_OSCULP32K;

  // ADC takes N + 1 cycles to calculate an N bit number.
  GCLK->PCHCTRL[ADC0_GCLK_ID].reg
    = GCLK_PCHCTRL_CHEN
    | GCLK_PCHCTRL_GEN_GCLK3;

  GCLK->PCHCTRL[TC4_GCLK_ID].reg
    = GCLK_PCHCTRL_GEN_GCLK3
    | GCLK_PCHCTRL_CHEN;
}

void DMAC_Handler() {
  // Clear the flag
  DMAC->INTPEND.reg = DMAC_INTPEND_TCMPL;
  PORTA.OUTSET.reg  = PORT_PA14;
  for(int i = 0; i < 0x500; i++) {
    // Do math
    __NOP();
  }
  PORTA.OUTTGL.reg = PORT_PA13;
  PORTA.OUTCLR.reg = PORT_PA14;
}


int main() {
  pl_cpu_init();
  MCLK->APBCMASK.bit.TC4_   = 1;
  MCLK->APBCMASK.bit.ADC0_  = 1;
  MCLK->AHBMASK.bit.DMAC_   = 1;
  MCLK->APBCMASK.bit.EVSYS_ = 1;

  PORTA.DIRSET.reg = PORT_PA13 | PORT_PA14 | PORT_PA15;

  clocks_init();
  dmac_init(dmac, dmac_wrb);
  adc_init();

  // Event system init
  EVSYS->CHANNEL[0].reg
    = EVSYS_CHANNEL_EVGEN(0)
    | EVSYS_CHANNEL_PATH_ASYNCHRONOUS
    | EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_TC4_MCX_1)
    | EVSYS_CHANNEL_RUNSTDBY;
  EVSYS->USER[EVSYS_ID_USER_ADC0_START].reg
    = EVSYS_USER_CHANNEL(1); // Channel = # + 1

  // Asynchronous events don't need a clock to work, so turn off the event system.
  MCLK->APBCMASK.bit.EVSYS_ = 0;

  timer_init();

  PM->STDBYCFG.reg = PM_STDBYCFG_BBIASHS(0);
  PM->SLEEPCFG.reg = PM_SLEEPCFG_SLEEPMODE_STANDBY;
  __enable_irq();
  NVIC_EnableIRQ(DMAC_IRQn);
  __DSB();

  while(1) {
    __WFI();
  }
}
