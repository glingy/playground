#include <pl.h>
#include <pl/clocks.h>
#include <pl/conf.h>

int main() {
  pl_init();

  while (1) {
    pl_busy_wait_ms(48000, 500);
    PORTA->OUTSET.reg = PORT_PA15;
    pl_busy_wait_ms(48000, 100);
    PORTA->OUTCLR.reg = PORT_PA15;
  }
}