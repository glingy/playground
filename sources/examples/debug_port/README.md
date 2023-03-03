# Compute Tester (Software)

### Goal:
 - The goal of the Compute Tester is to allow for quick diagnostics, unit tests, and un-bricking of computes on the fly on rayce.
 - It will also contain a library of programs on the car, so you can easily flash a new compute without needing any software experience.

### Code Notes:
 - This is very in-development. Since the SWD protocol isn't supported by the ATSAMC21, all of the debugging protocol is manually bit-banged over two pins. See [Resources](#resources) for the documents used in interfacing with the debug port on the computes.


### Resources
 - [ARM®v6-M Architecture Reference Manual](https://documentation-service.arm.com/static/5f8ff05ef86e16515cdbf826)
 - [ARM® Debug Interface Architecture Specification](https://documentation-service.arm.com/static/5f900b1af86e16515cdc0642)
 - [Cortex-M0+ Technical Reference Manual](https://documentation-service.arm.com/static/60411750ee937942ba301773)
 - [Atmel SAM C21 Data Sheet](https://ww1.microchip.com/downloads/en/DeviceDoc/SAMC20_C21_%20Family_Data_%20Sheet_DS60001479D.pdf)
