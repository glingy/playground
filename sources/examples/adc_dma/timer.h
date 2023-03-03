#include <pl_defs.h>
#include <samc21.h>


static inline void timer_init() {
  // TCC Output
  PORTA.PMUX[7].reg    = PORT_PMUX_PMUXO(PINMUX_PA15E_TC4_WO1);
  PORTA.PINCFG[15].reg = PORT_PINCFG_PMUXEN;


  // Init Timer
  TC4->COUNT8.CTRLA.reg
    = TC_CTRLA_MODE_COUNT8;
  TC4->COUNT8.PER.reg      = 127;
  TC4->COUNT8.WAVE.reg     = TC_WAVE_WAVEGEN_NFRQ;
  TC4->COUNT8.CTRLBSET.reg = TC_CTRLBSET_LUPD;
  TC4->COUNT8.EVCTRL.reg   = TC_EVCTRL_MCEO1; // Output event on CC1 match.

  TC4->COUNT8.CTRLA.reg
    = TC_CTRLA_ENABLE
    | TC_CTRLA_MODE_COUNT8
    | TC_CTRLA_RUNSTDBY
    | TC_CTRLA_PRESCALER_DIV64;
}