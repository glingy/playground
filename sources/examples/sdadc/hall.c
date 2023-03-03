#include "main.h"

#include <pl.h>

#define NUM_READINGS 256

volatile static int32_t readings[NUM_READINGS] = { 0 };
static int64_t sum                             = 0;
static int read_id                             = 0;
float hall_offset                              = 0;

#define ADC_MAX_VALUE        65535
#define HALL_CALIB_THRESHOLD 20000

// -5V: -7970520
// 0: 22000
// 5V: 8105790

void SDADC_Handler() {
  SDADC->INTFLAG.reg = SDADC_INTFLAG_RESRDY;

  int32_t res = SDADC->RESULT.reg;
  if (res & 0x800000) res |= ~0xffffff;
  sum += res;
  read_id++;
  if (read_id == NUM_READINGS) {
    read_id              = 0;
    volatile int32_t avg = sum / NUM_READINGS;
    sum                  = 0;
  }

  /*
    readings[read_id++] = res;
    if (read_id == NUM_READINGS) {
      read_id   = 0;
      float avg = 0;
      for (int i = 0; i < NUM_READINGS; i++) {
        avg += readings[i];
      }
      avg /= NUM_READINGS;
      // Save first nonzero value as calibration
      if (hall_offset == 0) {
        hall_offset = avg;
        return;
      }
      bps_current->pack_current = (int16_t) ((hall_offset - avg) * 100);

      int fault_code = 0;
      if (bps_current->pack_current < PACK_UNDER_CURRENT_FAULT_VALUE) {
        fault_code = BPS_FAULT_PACK_UNDER_CURRENT;
      } else if (bps_current->pack_current > PACK_OVER_CURRENT_FAULT_VALUE) {
        fault_code = BPS_FAULT_PACK_OVER_CURRENT;
      } else if (bps_current->pack_current < PACK_UNDER_CURRENT_WARNING_VALUE) {
        fault_code = BPS_WARNING_PACK_UNDER_CURRENT;
      } else if (bps_current->pack_current > PACK_OVER_CURRENT_WARNING_VALUE) {
        fault_code = BPS_WARNING_PACK_OVER_CURRENT;
      }

      if (fault_code) {
        bps_fault->code   = fault_code;
        bps_fault->module = 0;
        bps_fault->value  = bps_current->pack_current;
        fault(true);
      }

      bps_current->pack_power = (bps_current->pack_current * bps_info->voltage);
      pl_can_tx_send(CAN0, TX_ID_BPS_CURRENT);
    }*/
}

// Call before pl_init() to check if hall effect sensor is connected at all.
// Hope instead.
void hall_check() {
  PORTA->PINCFG[3].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
  if (!(PORTA->IN.reg & PORT_PA03)) {
    bps_info->state   = BPS_STATE_FAULT;
    bps_fault->code   = BPS_FAULT_PACK_HALL_CONNECTION;
    bps_fault->module = 0;
    bps_fault->value  = hall_offset;
  }
}