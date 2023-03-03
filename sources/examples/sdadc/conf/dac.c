#include <pl/conf/dac.h>

#define MAX_CURRENT 100
#define A_PER_V     200
#define REF_V       1.024

// DAC for SDADC ref (~0.625V)
PL_CONF_DAC = {
  .CTRLB = {
    ._REFSEL._INT1V,
    ._IOEN, // Internal output (SDADC ref)
  },

  // 125A max @ 5mV/A, 5V ref, 10-bit DAC.
  .DATA = (MAX_CURRENT / (REF_V * A_PER_V)) * 0x3FF,

  .CTRLA = { ._ENABLE },
};