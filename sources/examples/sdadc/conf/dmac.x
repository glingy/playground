#include "../main.h"

#include <pl/conf/dmac.h>

DmacDescriptor dma_wrb[1];

typedef struct {
  uint8_t test;
} Readings_t;
volatile Readings_t readings = { 0 };

DmacDescriptor dma_base[] = {
  {
    .BTCNT.reg = 2,
    .BTCTRL.reg
    = DMAC_BTCTRL_BEATSIZE_HWORD
    | DMAC_BTCTRL_VALID
    | DMAC_BTCTRL_BLOCKACT_INT
    | DMAC_BTCTRL_DSTINC,
    .SRCADDR.reg  = (uint32_t) &ADC0->RESULT.reg,
    .DSTADDR.reg  = (uint32_t) (&readings + 1),
    .DESCADDR.reg = (uint32_t) &dma_base[0],
  },
};

// @ 256Hz
PL_CONF_DMAC = {
  .CH0 = {
    ._CHCTRLA = { ._ENABLE, ._RUNSTDBY },
    ._CHCTRLB = {
      ._CMD._NOACT,
      ._TRIGSRC._ADC0_RESRDY,
      ._TRIGACT._BEAT,
      ._LVL._0,
    },
    ._CHINTENSET._TCMPL,
  },

  .CTRL = { ._DMAENABLE, ._LVLEN0 },
};