#include <avr/io.h>

void main() {
  DDRB |= (1 << DDB5);
  while (1) {
    for (uint32_t i = 0; i < 0x100000; i++) {
      asm volatile("nop");
    }
    PINB |= (1 << PORTB5);
  }
}