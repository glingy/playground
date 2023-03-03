#include <sam.h>

#define PORTA (&(PORT->Group[0]))
#define PORTB (&(PORT->Group[1]))

void main() {
  PORTA->DIRSET.reg = PORT_PA17;
  while (1) {
    for (int i = 0; i < 0x10000; i++) {
      asm volatile("nop");
    }
    PORTA->OUTTGL.reg = PORT_PA17;
  }
}