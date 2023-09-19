# Atmel SAM D21 Playground!

## Important Links
 * Documentation: WIP
 * BOM: https://playground.ctrtl.com/bom
 * Schematic: https://playground.ctrtl.com/schematic.pdf
 * ATSAMD21E15L Datasheet: https://playground.ctrtl.com/SAMD21.pdf

This repository contains the development kit for building code for the Atmel SAM D21 playground board. This board was developed to help members of the Solar Car Club (Prisum) at Iowa State University to learn how to program very low level embedded code. This repository is a reduced version of the development environment used by the team.

### Getting Started
 - Download VS Code (https://code.visualstudio.com)
 - Install Git (`build-essential` package on Ubuntu, Run `xcode-select --install` in a terminal on macOS, or install from https://git-scm.com/downloads for Windows)
 - Clone this repository to your computer
 - Open the folder with VS Code (Either drag the folder onto the VS Code application or open VS Code and select `Open Folder...`)
 - VS Code will prompt you to install some recommended extensions. Do so, it should install PlatformIO, C/C++ tools, multisrc, and Explorer Exclude.
   - This environment is based on PlatformIO (https://platformio.org) with several modifications to make it easier to use. 
 
### What is all this stuff?
 - Several of these are hidden by default, look in the `Hidden Items` tab in the explorer sidebar to see them. If it's hidden, you can ignore it for the most part.
 - `.vscode` - VSCode configuration. Suggested extensions, file type associations, configuration of what's hidden, etc.
 - `boards` - PlatformIO board definition for the playground board.
 - `include` - Shared header files across all projects (See PlatformIO documentation)
 - `lib` - Shared library code across all projects (See PlatformIO documentation)
 - `scripts` - Linker scripts and C startup files. You probably won't need to touch this, but feel free to explore
 - `sources` - The code folder! To create a new project, create a new folder in here. To be detected as a valid project, it must contain at least once `.c` file.
 - `.clang-format` - Auto-formatter config file. It's who you blame if your code magically reorganizes whenever you save the file.
 - `.gitignore` - Tells git to ignore specific files which should not be tracked.
 - `.multisrc` - Tells the multisrc extension to activate and allow you to select source folders.
 - `README.md` - This file!
 - `platformio.ini` - PlatformIO config file.
 - `multisrc.ini` - Generated PlatformIO config file by multisrc

### Bottom Status Bar Overview
PlatformIO adds a lot of buttons to the bottom status bar in VSCode. From left to right:
 - The first button is the checkmark, clicking that will build your code, but not run it. Good for checking syntax or verifying code when you don't have a board with you to run it on.
 - The second is the right arrow, that's the run button. It will build and run your code using the selected debugger.
 - The third is the trash can, that will delete everything from the build folder. If there's a strange linker error for some reason, you can try cleaning, but I've found that it's usually your fault and not because the build folder is dirty.
 - The serial monitor (plug) is not used yet.
 - The terminal will open a local terminal window with the `pio` command set up for debugging the development environment.
 - The next is the selected debugger. By default, it will assume the playground's internal debugger, but you can select another in `local.ini`. See `platform.ini` for example debugger definitions. 
 - When editing, you may see a database icon and/or a fire symbol to the left of the line and column numbers. That indicates that intellisense is parsing the file, so if syntax highlighting or linting looks broken, wait for those to disappear, and if it's not fixed by the time they disappear, close and reopen the file.
 - In the bottom right corner of VSCode, `< something >` indicates the source folder currently selected for running / debugging. It will be red if you're looking at a file not in the current project so you don't compile the wrong project on accident. Click that to select a different project

### Running
 - Select the program. Select the debugger. Press the arrow.

### Debugging
 - On the left sidebar, there's a bug with a play button, press the green arrow at the top of that sidebar to debug.
   - `Variables` are the global (and local) variables that were not optimized away. You can also hover over a variable to see its value. If it says `<optimized away>`, the compiler decided that variable wasn't worth keeping around, so you'll have to either declare it `volatile` and re-download (forces compiler to not optimize it out; remove `volatile` when done) or look around to find its value from nearby functions or calculations.
   - `Watch` lets you type C expressions and see the results. This is extremely useful also for optimized math, you can put any simple C expression (including math) in there, `&var` or `&var + len` is often useful for checking addresses of different things.
   - `Call Stack` shows which functions you're currently inside. If you go above `main`, you're now in the `scripts/startup.c` file. Click on an entry to jump to that function.
   - `Peripherals` shows the current status of every register in the processor. Extremely useful, especially if something's just broken and you have no idea why. Sometimes, register values won't update unless you click on them to expand them, if you see it flash or a loading bar at the top flash by, it probably just updated. Most useful part of debugging.
   - `Breakpoints` is self-explanatory. Click next to a line number to add a breakpoint. Note: Breakpoints listed there are for *all* projects, not just the current one, so it might be useful to remove breakpoints as soon as you're done with one.
   - `Registers` is not very useful for us, it's the current value of every register in the processor.
   - `Memory` enter a memory address and offset to see the hex value of that chunk of memory.
   - `Disassembly` if it works, it will disassemble a function to let you see the assembly form it is currently running. I've had issues with that in the past.
 - You can press the run button or the debug button again while debugging the program, different debuggers behave differently, some get angry, some stop the current debug process while making vscode think you're still debugging, some just ignore you. I suggest you press the stop debugging button before starting a new program.
 - The reload button will restart the current program, but *will not recompile nor re-download*. If you want to download a new version, stop and restart.

### Troubleshooting
**Help!** LED doesn't turn on!
 - Check wiring direction of external pullup/pulldown resistors (0/1)
 - Check output direction

**Help!** A peripheral isn't working!
 - Is it enabled in APB? - Did you wait after enabling it for the clock to enable?
 - Does it need AHB enabled?
 - Is it enabled?
 - Is its GCLK enabled?

**Help!** I need to force wipe and reset the board!
 - TBD

**Help!** It's just not working!
 - Yup. Welcome to software. Take a break and come back later. The answer's probably staring you in the face.


