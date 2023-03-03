#include "main.h"

uint32_t sqrtlu(uint32_t a) {
  DIVAS->SQRNUM.reg = a;
  while (DIVAS->STATUS.reg) {}
  return DIVAS->RESULT.reg;
}

uint32_t divlu(uint32_t a, uint32_t b) {
  DIVAS->CTRLA.reg    = 0; // Unsigned math
  DIVAS->DIVIDEND.reg = a;
  DIVAS->DIVISOR.reg  = b;
  while (DIVAS->STATUS.reg) {}
  return DIVAS->RESULT.reg;
}

uint32_t squarel(int32_t a) {
  return a * a;
}

// Mean temperature in 1/1000 of ˚C
uint32_t temp_mean() {
  uint32_t sum = 0;
  for (int i = 0; i < NUM_MODULES; i++) {
    sum += modules[i].data.decicelsius;
  }

  return divlu(sum * 100, NUM_MODULES);
}

// Standard deviation of temperature in 1/1000 ˚C
uint32_t temp_stddev(uint32_t mean) {
  uint32_t sum;
  for (int i = 0; i < NUM_MODULES; i++) {
    sum += squarel(((int32_t) modules[i].data.decicelsius * 100) - (int32_t) mean);
  }
  return sqrtlu(divlu(sum, (NUM_MODULES - 1)));
}

void temp_check() {
  uint32_t mean       = temp_mean();
  uint32_t limit      = divlu(mean + (temp_stddev(mean) * 3), 100);
  bps_info->ota_limit = limit;
  for (int i = 0; i < NUM_MODULES; i++) {
    if (modules[i].data.decicelsius > limit) {
      bps_fault->code   = BPS_FAULT_OVER_TEMP_AVERAGE;
      bps_fault->module = i + 1;
      bps_fault->value  = modules[i].data.decicelsius;
      fault(true);
      break;
    }
  }
}