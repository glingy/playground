# *Board Name* (Software)

### Goal:
 - *What's the goal of this board?*

### Code Notes:

PACK_TRIGGER -> Anded into relay logic, set high only.
FAN_CONTROL -> PWM for fans, or just on/off
PACK_EN -> 
ARRAY_EN ->

FAN_CONTROL -> PWM for fans, or just on/off

NEGATIVE_RELAY -> close first, after initial ok
200ms delay
POSITIVE_RELAY -> close second, after initial ok
200ms delay
POWERBOARD_EN -> Powerboard should switch vicors, after big pos/neg closed
200ms delay
PRECHARGE_EN -> Motor relay

CHARGE_RELAY -> Closed when charge pilot, open otherwise. Sends charge relay

MCU_Fault -> Low on fault, high after initial ok

ARRAY_EN -> N/I for now.




### Messages In
 - *What CAN messages does this board consume?*

### Messages Out
 - Fatal Error (#1)
 - Warning (#2)
 - Startup (#3)
 - *What other CAN messages does this board send?*

### Resources
 - [Atmel SAM C21 Data Sheet](https://ww1.microchip.com/downloads/en/DeviceDoc/SAMC20_C21_%20Family_Data_%20Sheet_DS60001479D.pdf)