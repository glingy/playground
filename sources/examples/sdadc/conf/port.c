#include "../main.h"

#include <pl/can/types.h>
#include <pl/conf/port.h>

PL_CONF_PORT = {
  PL_CAN0_PINS,

  // Fans always on
  .FAN_CONTROL._OUTPUT_HIGH,

  // MCU Fault start low
  // Set high when OK
  .MCU_FAULT._OUTPUT_LOW,

  // Negative Relay, closed first
  .NEG_RELAY._OUTPUT_LOW,

  // Positive Relay, closed second
  .POS_RELAY._OUTPUT_LOW,

  // Powerboard, closed third
  .POWERBOARD_EN._OUTPUT_HIGH,

  // Precharge (Motor Relay)
  .PRECHARGE_EN._OUTPUT_HIGH,

  // Charge relay
  // Closed when Pilot and charging
  .CHARGE_RELAY._OUTPUT_LOW,

  // Analog Fault (Active Low)
  .ANALOG_FAULT_LINE = {
    ._INPUT,
    ._MUX(MUX_PA14A_EIC_EXTINT14),
  },

  // External Kill (Active High)
  .EXTERNAL_KILL = {
    ._INPUT,
    ._MUX(MUX_PA16A_EIC_EXTINT0),
  },

  // Pack hall effect sensor
  .PACK_HALL._MUX(MUX_PA03B_ADC0_AIN1),

  // Array hall effect sensor
  //.ARRAY_HALL._MUX(MUX_PA02B_ADC0_AIN0),
  .ARRAY_HALL._OUTPUT_LOW,

  .PA15._OUTPUT_LOW,

  .PA04._MUX(MUX_PA04B_SDADC_VREFP),
  .PA06._MUX(MUX_PA06B_SDADC_INN0),
  .PA07._MUX(MUX_PA07B_SDADC_INP0),
};