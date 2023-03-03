#include <pl/conf/evsys.h>

// @ 256Hz
PL_CONF_EVSYS = {
  .CHANNEL0 = {
    ._EVGEN._TC4_OVF,
    ._PATH._ASYNCHRONOUS,
    ._ONDEMAND,
    ._RUNSTDBY,
  },

  .USER_SDADC_START._CHANNEL0,
};