#include <samc21.h>

static inline void dmac_init(DmacDescriptor * dmac_desc, DmacDescriptor * dmac_wrb) {
  // Init DMAC
  /**
   * Configure DMAC to copy ADC readings into SRAM on every completed read.
   **/
  DMAC->BASEADDR.reg = (uint32_t) dmac_desc;
  DMAC->WRBADDR.reg  = (uint32_t) dmac_wrb;
  DMAC->CHID.reg     = DMAC_CHID_ID(0);
  DMAC->CHCTRLB.reg
    = DMAC_CHCTRLB_TRIGACT_BEAT
    | DMAC_CHCTRLB_TRIGSRC(ADC0_DMAC_ID_RESRDY)
    | DMAC_CHCTRLB_LVL(0);
  DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TCMPL;
  DMAC->CHCTRLA.reg    = DMAC_CHCTRLA_ENABLE | DMAC_CHCTRLA_RUNSTDBY;
  DMAC->CTRL.reg       = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN0;
}