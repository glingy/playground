Method of operation:
 - Pin begins to change. EIC triggers asynchronous interrupt from deep sleep mode.
 - Pin change is recorded, TC started for debouncing (1ms timeout), return from interrupt.
  - If timer running, don't enter deep sleep yet, but stay awake.
 - If pins continue to change, EIC will continue to trigger, pushing the timeout further back, resetting the TC.
 - TC interrupts after 1ms with no change, message is recalculated (might have missed a change during the EIC interrupt) and sent.

 Issues: pin change during TC? - disable interrupts during TC handler.
 
