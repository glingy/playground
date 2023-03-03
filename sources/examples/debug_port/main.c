#include <pl.h>

/**
 * SWDP Packet:
 * Start | AP/DP | R/W | ADDR | PARITY | STOP | PARK | TURNRND
 *   1   |  1/0  | 1/0 | 0-3  | Even   |  0   |  1   | Tri-state
 *
 **/

#define REQ_START      1
#define REQ_APACC      (1 << 1)
#define REQ_READ       (1 << 2)
#define REQ_ADDR(addr) (addr << 3)
#define REQ_PARITY     (1 << 5)
#define REQ_PARK       (1 << 7)

#define DP_DPIDR         REQ_ADDR(0)
#define DP_CTRLSTAT      REQ_ADDR(1)
#define DP_CTRL_PWRUPREQ ((1 << 28) | (1 << 30))
#define DP_CTRL_PWRUPACK ((1 << 29) | (1 << 31))

#define DP_SELECT              REQ_ADDR(2)
#define DP_SELECT_APBANKSEL(n) (n << 4)
#define DP_SELECT_DPBANKSEL(n) (n)
#define DP_SELECT_APSEL(n)     (n << 24)

#define DP_RDBUFF REQ_ADDR(3)

#define AP_CSW     REQ_ADDR(0)
#define AP_CSW_SEL REQ_ADDR(0)
#define AP_TAR     REQ_ADDR(1)
#define AP_DRW     REQ_ADDR(3)


#define ACK_OK    1
#define ACK_WAIT  2
#define ACK_FAULT 4

#define SWDIO PORT_PB08
#define SWCLK PORT_PB09
#define RST   PORT_PA20

uint32_t ap = 0;

static inline void clock() {
  PORTB->OUTTGL.reg = SWCLK;
  __NOP();
  PORTB->OUTTGL.reg = SWCLK;
}

static inline uint8_t __parityReq(const uint8_t req) {
  return ((req << 1) ^ (req << 2) ^ (req << 3) ^ (req << 4)) & (1 << 5);
}

#define HIGH()     PORTB->OUTSET.reg = SWDIO
#define LOW()      PORTB->OUTCLR.reg = SWDIO
#define TRISTATE() PORTB->DIRTGL.reg = SWDIO
#define SWDIN()    (!!(PORTB->IN.reg & SWDIO))

void sendByte(uint8_t req) {
  for (int i = 0; i < 8; i++) {
    if (req & 1) {
      HIGH();
    } else {
      LOW();
    }
    clock();
    req >>= 1;
  }
}

static inline void request(const uint8_t req) {
  sendByte(req | REQ_START | REQ_PARK | __parityReq(req));
  TRISTATE();
  clock();
}

// Read first, then clock for each bit.
uint_fast8_t readAck() {
  uint_fast8_t data = 0;
  for (uint_fast8_t i = 0; i < 3; i++) {
    data >>= 1;
    data |= SWDIN() << 2;
    clock();
  }

  return data;
}

__NO_RETURN
void fail() {
  for (int i = 0; i < 5; i++) clock();
  while (1) {}
}

uint32_t req_read(const uint8_t req) {
  request(REQ_READ | req);

  if (!(readAck() & ACK_OK)) {
    fail();
  }

  uint32_t data       = 0;
  uint_fast8_t parity = 0;
  for (uint_fast8_t i = 0; i < 32; i++) {
    data >>= 1;
    parity ^= SWDIN();
    data |= SWDIN() << 31;
    clock();
  }

  parity ^= SWDIN();
  clock();
  TRISTATE();
  clock();
  if (parity) fail();

  return data;
}

void req_write(uint8_t req, uint32_t data) {
  request(req);

  if (!(readAck() & ACK_OK)) {
    fail();
  }

  TRISTATE();
  clock();

  uint_fast8_t parity = 0;
  for (uint_fast8_t i = 0; i < 32; i++) {
    if (data & 1)
      HIGH();
    else
      LOW();

    parity ^= data & 1;
    data >>= 1;
    clock();
  }

  if (parity)
    HIGH();
  else
    LOW();
  clock();
}

uint32_t getAP() {
  for (uint32_t i = 0; i <= 0xFF; i++) {
    req_write(DP_SELECT, DP_SELECT_APSEL(i) | DP_SELECT_APBANKSEL(0xF));
    req_read(REQ_APACC | REQ_ADDR(3));
    req_read(DP_RDBUFF);
    // if ((req_read(DP_RDBUFF) & 0x0FFF0000) == 0x04770000) {
    //   req_write(DP_SELECT, DP_SELECT_APSEL(i) | DP_SELECT_APBANKSEL(0));
    //   return i;
    // }
    req_read(DP_CTRLSTAT);
  }
  fail();
}

void writeMemAddr(uint32_t addr, uint32_t value) {
  /**
   * Sequence:
   *  - DPACC AP SELECT (0x0)
   *  - APACC CSW?
   *  - APACC TAR
   *  - APACC DRW
   *  - DPACC RDBUFF?
   *  -
   **/
  req_write(REQ_APACC | AP_TAR, addr);
  req_write(REQ_APACC | AP_DRW, value);
}

uint32_t readMemAddr(uint32_t addr) {
  req_write(REQ_APACC | AP_TAR, addr);
  req_read(REQ_APACC | AP_DRW);
  return req_read(DP_RDBUFF);
}

void connect() {
  PORTA->OUTSET.reg = RST;
  pl_busy_wait_ms(48000, 5);
  PORTA->OUTCLR.reg = RST;
  pl_busy_wait_ms(48000, 5);
  PORTA->OUTSET.reg = RST;
  pl_busy_wait_ms(48000, 5);
  LOW();
  clock();
  // Reset DAP by sending 50CLK with DIO set
  HIGH();
  for (int i = 0; i < 50; i++) clock();

  // Send JTAG -> SWD command
  sendByte(0x9E);
  sendByte(0xE7);

  // Reset DAP by sending 50CLK with DIO set
  HIGH();
  for (int i = 0; i < 50; i++) clock();


  LOW();
  clock();
  clock();
  req_read(DP_DPIDR);

  // Power Up before AP Access
  req_write(DP_CTRLSTAT, DP_CTRL_PWRUPREQ);
  while ((req_read(DP_CTRLSTAT) & DP_CTRL_PWRUPACK) != DP_CTRL_PWRUPACK) {}

  // Find the first MEM AP
  ap = getAP();

  // Write the clear bit
  writeMemAddr(&REG_DSU_CTRL, DSU_CTRL_CE);
  while (!(readMemAddr(&REG_DSU_CTRL) & (DSU_STATUSA_DONE << 8))) {}

  PORTA->OUTTGL.reg = PORT_PA15;
  while (1) {
  }
}


// Will chip-erase. Just plug in swdio, swclk, and rst.
int main() {
  PORTB->DIRSET.reg = SWDIO | SWCLK;
  PORTA->DIRSET.reg = RST | PORT_PA15;

  // PORTA->OUTCLR.reg = PORT_PA14;
  // for(int i = 0; i < 0x200000; i++) { __NOP(); }
  PORTA->OUTSET.reg = PORT_PA15;
  // while (1) {}

  pl_cpu_init_48MHz();
  PORTB->PINCFG[8].reg = PORT_PINCFG_INEN;
  connect();
}