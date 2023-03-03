#include "notes.h"

#include <sam.h>
#include <stdbool.h>

#define PORTA (&(PORT->Group[0]))
#define PORTB (&(PORT->Group[1]))

#define FUSE_DFLL48M_COARSE_CAL (*(uint32_t *) FUSES_DFLL48M_COARSE_CAL_ADDR >> FUSES_DFLL48M_COARSE_CAL_Pos)
#define FUSE_DFLL48M_FINE_CAL   (*(uint32_t *) FUSES_DFLL48M_FINE_CAL_ADDR >> FUSES_DFLL48M_FINE_CAL_Pos)

void osc8m_init() {
  // Remove prescaler from OSC8M so it actually runs at 8MHz.
  SYSCTRL->OSC8M.bit.PRESC = 0;
}

void osc48m_init() {
  // Workaround errata 1.2.1 (Must disable ON_DEMAND before configuring)
  SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;

  // Set calibration values from NVM FUSES (Current datasheet doesn't document FINE calibration values, but it does exist)
  SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(FUSE_DFLL48M_COARSE_CAL) | SYSCTRL_DFLLVAL_FINE(FUSE_DFLL48M_FINE_CAL);
}

void cpu_48m() {
  // Increase the number of read-wait states so the flash can keep up. DUAL is required at 48MHz.
  NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_DUAL;

  // Configure MCLK
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_GENEN;
}

void uart_init(void) {
  PM->APBCMASK.bit.SERCOM3_ = 1;

  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4) | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_GENEN;
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM3_CORE | GCLK_CLKCTRL_GEN_GCLK4 | GCLK_CLKCTRL_CLKEN;

  PORTA->PMUX[12].reg   = PORT_PMUX_PMUXE(MUX_PA24C_SERCOM3_PAD2) | PORT_PMUX_PMUXO(MUX_PA25C_SERCOM3_PAD3);
  PORTA->PINCFG[24].reg = PORT_PINCFG_PMUXEN;
  // No RX yet...

  SERCOM3->USART.BAUD.reg  = 7672;
  SERCOM3->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN;
  SERCOM3->USART.CTRLA.reg = SERCOM_USART_CTRLA_TXPO(1) | SERCOM_USART_CTRLA_SAMPR(4) | SERCOM_USART_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_ENABLE;
}

void uart_send(uint8_t data) {
  SERCOM3->USART.DATA.reg = data;
  while (!SERCOM3->USART.INTFLAG.bit.DRE)
    continue;
}

void btn_init() {
  PORTB->PINCFG[4].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
  PORTB->OUTSET.reg    = PORT_PB04;
}

void main(void) {
  osc48m_init();
  cpu_48m();
  btn_init();

  uart_init();

  bool pressed = false;
  while (1) {
    if (!pressed && (PORTB->IN.reg & PORT_PB04)) {
      pressed = true;
      uart_send('A');
    } else if (!(PORTB->IN.reg & PORT_PB04)) {
      pressed = false;
      for (int i = 0; i < 0x80000; i++) {
        asm volatile("nop");
      }
    }
  }
}