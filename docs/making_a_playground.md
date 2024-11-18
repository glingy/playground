# Making a Playground!

## Important Links
 * BOM: https://playground.ctrtl.com/bom
 * Schematic: https://playground.ctrtl.com/schematic.pdf
 * Latest Gerber Files: https://github.com/glingy/playground/releases/latest


### Ordering Parts
Download the Gerber files and upload them to your favorite custom PCB manufacturer. JLCPCB is what I've used in the past. Leave all of the fabrication settings at default and click order, they will get back to you within a few days and let you know that your design was approved and is sent to fabrication.

Parts are typically ordered through Digikey. The [Digikey Cart is here](https://www.digikey.com/short/mtbb19ww), adjust the quantities as you see fit. The debugger chip (U4) is not available on Digikey at the time of writing, it can be ordered [directly through Microchip](https://www.microchipdirect.com/product/ATSAMD21E16C-UUT) or another retailer. Make sure to check over your cart for out of stock/backordered items and replace them with equivalent ones.


### Assembling the Board
This is where the soldering fun begins!

#### The Debugger
Playgrounds have an integrated debugger on the back of the board (U4). It is a BGA component, and it should be soldered first. This should be done with a solder reflow oven, reference the Soldering Profile in the SAMD21 datasheet (chapter 44.2) for how to set up the oven.

A few notes on this step:
 * You get one try to put the debugger on, and then the board is toast.
   * BGA components have exactly the right amount of solder on them to attach, and retrying a part means cleaning off the PCB incredibly well and using a fancy reballing machine to replace the solder on the part.
 * Make sure the debugger is on square, flat to the board, and in the right orientation.
 * Use a generous amount of flux.
 * You can tell if it worked or not pretty easily -- bad parts will be shifted off to one side, leaving exposed pads.

#### Soldering the Rest
Use the [BOM website](https://playground.ctrtl.com/bom) to see where components go and how many you need to take out. The general rule is solder parts from smallest to largest to make sure you have enough space to work. Leaded solder is nice here, it lets you rework parts easily.


### Testing
Your board is done, let's make sure it works!

1. Find another debugger to program your board with. I used a blackmagic probe.
2. Connect to the Playground debugger through ***J3*** using jumper wires.
   * Top to bottom, the pins are GND, SWCLK, SWDIO, Reset. Reset doesn't need to be hooked up.
3. Power the Playground by plugging in the USB cable.
4. Connect the debugger to a PC and find the device for the debugger. On my PC it's COM4.
5. Download `blackmagic.elf`, open a terminal, and `cd` into its directory.
6. Run the following in GDB (I used `gdb-multiarch`, you can also use `gdb-arm-none-eabi`):
```
target extended-remote [device] // If GDB lags here, you picked the wrong device. Try again
monitor swd_scan                // You should see a device show up here. If you don't, check your connections
attach 1
monitor unlock_bootprot
file blackmagic.elf
load
compare-sections                // If this fails, something wasn't programmed correctly. Try again
kill
```
7. Now that your Playground has a debugger, unplug your secondary debugger and playground from your PC. Plug the Playground back in and load an example. Make sure everything lights up and works.
8. Go write your own projects!

#### Alternate GDB Commands
Everything in GDB can be abbreviated, here's the short version:
```
tar ext [device]
mon s
att 1
mon unlock_bootprot
file blackmagic.elf
load
compare-sections
kill
```
