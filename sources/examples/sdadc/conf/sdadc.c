#include <pl/conf/sdadc.h>

// @ 256Hz
PL_CONF_SDADC = {
  .REFCTRL = {
    ._ONREFBUF,
    ._REFRANGE._LT_1_4V, // DAC ~0.6V
    ._REFSEL._DAC,
  },

  .CTRLB = {
    ._SKPCNT = 2,
    ._OSR._1024,
    ._PRESCALER = 0,
  },

  .EVCTRL = {
    ._STARTEI,
  },

  .INTENSET._RESRDY,
  .DBGCTRL._DBGRUN,

  .INPUTCTRL._MUXSEL._AIN0,

  // Calibration Stuff
  // (RESULT + OFFSETCORR) * (GAINCORR/2^SHIFTCORR)
  .OFFSETCORR = 172000,
  .GAINCORR   = 517,
  .SHIFTCORR  = 15,

  .CTRLA = { ._ENABLE },
};