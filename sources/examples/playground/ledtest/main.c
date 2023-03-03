#include "notes.h"

#include <sam.h>
#include <stdbool.h>

#define PORTA (&(PORT->Group[0]))
#define PORTB (&(PORT->Group[1]))

#define PORT_BTN1 PORT_PB04

#define PORT_D1 PORT_PA02
#define PORT_D2 PORT_PA03
#define PORT_D3 PORT_PA18
#define PORT_D4 PORT_PA19
#define PORT_D5 PORT_PA22
#define PORT_D6 PORT_PA23
#define PORT_D7 PORT_PA24
#define PORT_D8 PORT_PA25

#define PORT_D PORT_D1 | PORT_D2 | PORT_D3 | PORT_D4 | PORT_D5 | PORT_D6 | PORT_D7 | PORT_D8

#define PORT_SEGA  PORT_PA04
#define PORT_SEGB  PORT_PA05
#define PORT_SEGC  PORT_PA10
#define PORT_SEGD  PORT_PA11
#define PORT_SEGE  PORT_PA14
#define PORT_SEGF  PORT_PA15
#define PORT_SEGG  PORT_PA16
#define PORT_SEGDP PORT_PA17

#define PORT_SEG PORT_SEGA | PORT_SEGB | PORT_SEGC | PORT_SEGD | PORT_SEGE | PORT_SEGF | PORT_SEGG | PORT_SEGDP

#define PORT_BUZZER PORT_PA06

#define PORT_LEDS PORT_D | PORT_SEG

static const int leds[] = {
  PORT_D1,
  PORT_D2,
  PORT_D3,
  PORT_D4,
  PORT_D5,
  PORT_D6,
  PORT_D7,
  PORT_D8,
  PORT_SEGA,
  PORT_SEGB,
  PORT_SEGC,
  PORT_SEGD,
  PORT_SEGE,
  PORT_SEGF,
  PORT_SEGG,
  PORT_SEGDP,
};

static int led_index = 0;

void led_run() {
  led_index = (led_index + 1) % (sizeof(leds) / sizeof(leds[0]));
  if (led_index < 8) {
    PORTA->OUTCLR.reg = PORT_D;
    PORTA->OUTSET.reg = PORT_SEG | leds[led_index];
  } else {
    PORTA->OUTSET.reg = PORT_SEG;
    PORTA->OUTCLR.reg = PORT_D | leds[led_index];
  }
}

void led_init() {
  PORTA->DIRSET.reg = PORT_D | PORT_SEG;
}

void buzzer_init() {
  PORTA->PMUX[3].reg = PORT_PMUX_PMUXE_E;

  // Power on TCC1
  PM->APBCMASK.bit.TCC1_ = 1;

  GCLK->GENCTRL.reg
    = GCLK_GENCTRL_ID(3)
    | GCLK_GENCTRL_SRC_DFLL48M
    | GCLK_GENCTRL_GENEN;

  GCLK->GENDIV.reg
    = GCLK_GENDIV_ID(3)
    | GCLK_GENDIV_DIV(3);

  GCLK->CLKCTRL.reg
    = GCLK_CLKCTRL_GEN_GCLK3
    | GCLK_CLKCTRL_ID_TCC0_TCC1
    | GCLK_CLKCTRL_CLKEN;

  TCC1->CTRLBSET.reg
    = TCC_CTRLBSET_LUPD;


  TCC1->CTRLA.reg
    = TCC_CTRLA_PRESCALER_DIV1
    | TCC_CTRLA_ENABLE;

  TCC1->CC[0].reg = 64;
  TCC1->PER.reg   = 128;
  TCC1->WAVE.reg
    = TCC_WAVE_WAVEGEN_NPWM;
}

void buzzer_tone(uint_fast16_t count) {
  TCC1->PER.reg        = count;
  TCC1->CC[0].reg      = count / 2;
  PORTA->PINCFG[6].reg = PORT_PINCFG_PMUXEN;
}

void buzzer_off() {
  PORTA->PINCFG[6].reg = 0;
}

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


void osc_init() {
  PORTA->DIRSET.reg     = PORT_PA15;
  PORTA->PMUX[7].reg    = PORT_PMUX_PMUXO_H;
  PORTA->PINCFG[15].reg = PORT_PINCFG_PMUXEN;

  // NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_DUAL;
  // GCLK->GENCTRL.reg      = GCLK_GENCTRL_ID(1) | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_OE | GCLK_GENCTRL_GENEN;
}

void btn_init() {
  PORTB->PINCFG[4].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
  PORTB->OUTSET.reg    = PORT_PB04;
}

bool pressed = false;
int n        = 0;

typedef struct {
  uint32_t note;
  uint8_t duration;
} note_t;

static const note_t song[] = {
  { NOTE_C4, 16 },
  { NOTE_E4, 16 },
  { NOTE_G4, 16 },
  { 0, 16 },
};

void dmac_init(note_t * song, uint32_t length) {
  PM->APBBMASK.bit.DMAC_ = 1;

  DMAC->CHID.reg = 1;
}

void rgb_init() {
  PM->APBCMASK.bit.SERCOM0_ = 1;

  PORTA->PMUX[4].reg   = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C;
  PORTA->PINCFG[8].reg = PORT_PINCFG_PMUXEN;
  PORTA->PINCFG[9].reg = PORT_PINCFG_PMUXEN;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM0_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;

  SERCOM0->SPI.CTRLA.reg
    = SERCOM_SPI_CTRLA_FORM(0) | SERCOM_SPI_CTRLA_DOPO(0) | SERCOM_SPI_CTRLA_MODE_SPI_MASTER | SERCOM_SPI_CTRLA_ENABLE;

  SERCOM0->SPI.DATA.reg = 0;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = 0;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = 0;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = 0;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
}

void rgb_send(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b) {
  SERCOM0->SPI.DATA.reg = 0xFF;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = r & 0xFF;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = g & 0x00;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
  SERCOM0->SPI.DATA.reg = b & 0xFF;
  while (!SERCOM0->SPI.INTFLAG.bit.DRE) continue;
}

void main() {
  osc48m_init();
  cpu_48m();
  led_init();
  buzzer_init();
  osc8m_init();
  // osc_init();
  btn_init();
  rgb_init();

  uint32_t n = 0;
  while (1) {
    for (int i = 0; i < 0x80000; i++) {
      asm volatile("nop");
    }
    led_run();

    if (PORTB->IN.reg & PORT_PB04) {
      buzzer_off();
    } else {
      // n = (n + 1) % 50;
      buzzer_tone(NOTE_C4);
    }

    n = (n + 1) % 8;
    rgb_send(n, n, n);

    /*
        if (!(PORTB->IN.reg & PORT_PB04) && !pressed) {
          pressed = true;
          // PORTA->OUTSET.reg = PORT_SEGDP;
          buzzer_tone(128);
        } else if ((PORTB->IN.reg & PORT_PB04) && pressed) {
          pressed = false;
          // PORTA->OUTCLR.reg = PORT_SEGDP;
          buzzer_off();
        }
        */
  }
}