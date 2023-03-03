#include <pl/conf/clocks.h>

// Enable peripheral clocks
PL_CONF_MCLK = {};

// Note:
//  * Setting GCLK0 to > 24MHz will set NVMCTRL RWS to 1 so flash can keep up.
//    - If you start lower, but later decide to increase to >24MHz, you will have
//      to do that yourself.
//

// Configure clock sources:
PL_CONF_GCLK = {
  // Main clock @ 48MHz
  .GCLK0.FREQ = 48000000UL,
};