#include <pl/conf/tc.h>

// SDADC Current timer (1024 Hz)
PL_CONF_TC4 = {
  .COUNT8 = {
    .CTRLBSET._LUPD, // Disable double buffering

    .PER = 31,

    .WAVE._WAVEGEN._NPWM,

    .EVCTRL._OVFEO,

    .CTRLA = {
      ._MODE._COUNT8,
      ._RUNSTDBY,
      ._ENABLE,
    },
  }
};

// Pack check (4Hz)
PL_CONF_TC0 = {
  .COUNT8 = {
    .CTRLBSET._LUPD, // Disable double buffering

    .PER = 63,
    .CC1 = 64,

    .WAVE._WAVEGEN._NPWM,

    .INTENSET._OVF,

    .CTRLA = {
      ._MODE._COUNT8,
      ._RUNSTDBY,
      ._ENABLE,
    },
  }
};