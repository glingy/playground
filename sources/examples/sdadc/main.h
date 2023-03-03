#ifndef MAIN_H
#define MAIN_H

#include <pl.h>

#define NUM_MODULES              35
#define COM_FAULT_MISSES         8
#define CHARGE_CURRENT_THRESHOLD 0
#define CHARGE_TEMP_THRESHOLD    450

// Thinking of a multiplier of 100 for current stuff
// May need to set pack
#define PACK_OVER_CURRENT_WARNING_VALUE  7000
#define PACK_UNDER_CURRENT_WARNING_VALUE -4500
#define PACK_OVER_CURRENT_FAULT_VALUE    7500
#define PACK_UNDER_CURRENT_FAULT_VALUE   -5000

// Fans always on
#define FAN_CONTROL PL_PIN(PA25)

// MCU Fault start low
// Set high when OK
#define MCU_FAULT PL_PIN(PA13)

// Negative Relay, closed first
#define NEG_RELAY PL_PIN(PA10)

// Positive Relay, closed second
#define POS_RELAY PL_PIN(PA09)

// Powerboard, closed third
#define POWERBOARD_EN PL_PIN(PB10)

// Precharge (Motor Relay)
#define PRECHARGE_EN PL_PIN(PA27)

// Charge relay
// Closed when Pilot and charging
#define CHARGE_RELAY PL_PIN(PA11)

// Analog Fault (Active Low)
#define ANALOG_FAULT_LINE PL_PIN(PA14)

// External Kill (Active High)
#define EXTERNAL_KILL PL_PIN(PA16)

// Pack hall effect sensor
#define PACK_HALL PL_PIN(PA03)

// Pack hall effect sensor
#define ARRAY_HALL PL_PIN(PA02)

enum {
  TX_ID_WARNING,
  TX_ID_STARTUP,
  TX_ID_BPS_FAULT,
  TX_ID_BPS_WARNING,
  TX_ID_BPS_CURRENT,
  TX_ID_BPS_PACK_TEMP,
  TX_ID_BPS_INFO,
  TX_ID_CHARGE,
};

typedef struct {
  PLMsgModule_t data;
  uint32_t contact;
} ModuleData_t;

extern volatile PLMsgBPSCurrent_t * bps_current;
extern volatile PLMsgBPSInfo_t * bps_info;
extern volatile PLMsgBPSFault_t * bps_fault;
extern ModuleData_t modules[NUM_MODULES];

void fault(bool send);
void hall_check();
void temp_check();

#endif