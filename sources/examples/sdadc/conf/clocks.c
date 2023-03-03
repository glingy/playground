#include <pl/conf/clocks.h>

// Enable peripheral clocks
PL_CONF_MCLK = {
  ._TC4,
  ._TC0,
  ._DAC,
  ._EVSYS,
  ._SDADC,
};

// Note:
//  * Setting GCLK0 to > 24MHz will set NVMCTRL RWS to 1 so flash can keep up.
//    - If you start lower, but later decide to increase to >24MHz, you will have
//      to do that yourself.
//

// Configure clock sources:
PL_CONF_GCLK = {
  // Main clock @ 48MHz
  .GCLK0.FREQ = 48000000UL,

  .GCLK1 = {
    .FREQ = PL_CAN_DEFAULT_CLOCK_FREQ,

    ._CAN0,
  },

  .GCLK2 = {
    .FREQ = 256,

    ._TC0,
  },

  .GCLK3 = {
    .FREQ = 32768,

    ._TC4,
    ._TC2,
  },

  .GCLK4 = {
    .FREQ = 48000000UL,

    ._DAC,
    ._SDADC,
  }
};