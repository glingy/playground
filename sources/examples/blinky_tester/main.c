#include <sam.h>

#define PORTA (&(PORT->Group[0]))
#define PORTB (&(PORT->Group[1]))

void main() {
  PORTB->DIRSET.reg = PORT_PB04;
  while (1) {
    for (int i = 0; i < 0x100000; i++) {
      asm volatile("nop");
    }
    PORTB->OUTTGL.reg = PORT_PB04;
  }
}