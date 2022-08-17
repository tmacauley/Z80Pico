/* 
* This example program simulates Grant Searle's 7 chip Z80
* Modifications to Grant's design:
*   RAM shadows the ROM - writing to the ROM addresses will write to the shadow RAM.
*   Added ram_page variable to bank switch ROM to RAM.
*/

#include <Z80Pico.h>
#include "rom.h"

#define Z80_ROM_TOP 8191
#define Z80_RAMSIZE 65536

#define RAM_PAGE_PORT 0x38

#define M6850_CONTROL_REGISTER 0x80
#define M6850_STATUS_REGISTER 0x80
#define M6850_TX_REGISTER 0x81
#define M6850_RX_REGISTER 0x81

#define INTERRUPT1_VECTOR 0x38

#define STEP_DELAY 300

// #define DEBUG

// Create ram
byte Z80ram[Z80_RAMSIZE];

// Allow swapping ROM for RAM
byte ram_page = 0;


/*
 * Five mandatory functions! Compilation will fail if these are
 * not defined. You must implement them yourself.
 */

// Memory write -- write the Value to memory location Addr
void WrZ80(zword Addr, byte Value) {
  #ifdef DEBUG
  Serial.print("  Wr: ");
  Serial.print(Addr, HEX);
  Serial.print(", ");
  Serial.print(Value, HEX);
  Serial.println();
  #endif
  Z80ram[Addr] = Value;
}

// Memory read -- read the value at memory location Addr
byte RdZ80(zword Addr) {

  #ifdef DEBUG
  int Value = ram[Addr];
  Serial.print("  Rd: ");
  Serial.print(Addr, HEX);
  Serial.print(", ");
  Serial.print(Value, HEX);
  Serial.println();
  #endif
  
  if (!ram_page && Addr <= Z80_ROM_TOP) {
    return Z80rom[Addr];
  }
  return Z80ram[Addr];
}

// IO -- output the Value on the Port (currently does nothing)
// This is generally optional unless your code uses IO
// Can be left blank.
void OutZ80(zword Port, byte Value) {

  Port = Port & 0xff;

  switch(Port) {
    case RAM_PAGE_PORT:
      // paging port
      ram_page = Value;
      break;
    case M6850_CONTROL_REGISTER:
      // M6850 Control register
      break;
    case M6850_TX_REGISTER:
      // M6850 Tx Register
      Serial.write(Value);
      break;
  }
}

// IO -- read a value from Port (currently defaults to 0)
// This is generally optional unless your code uses IO
// Can be left empty as long as it returns a byte.
byte InZ80(zword Port) {

  Port = Port & 0xff;
  
  switch(Port) {
    case M6850_STATUS_REGISTER:
      // M6850 Status register
      if (Serial.available() > 0) {
        return (byte)3;
      }
      else {
        return (byte)2;
      }
  
    case M6850_RX_REGISTER:
      // M6850 Rx Register
      return Serial.available() > 0 ? Serial.read() : (byte)0; 
  }
  return (byte)0;
}

// Advanced -- called when an emulator-specific opcode of
// ED FE is encountered. Generally can be left empty.
void PatchZ80(Z80 *R) {}

// create a CPU core object
Z80 cpu;

void setup() {
  Serial.begin(115200);

  // wait for terminal to come online
  while(!Serial.available()) {
    delay(1000);
    Serial.print('.');
  }
  Serial.read();

  // Reset the CPU to 0x00 and zero the regs/flags
  ResetZ80(&cpu);
}

void loop() {

  // If character available on serial port, send interrupt to the Z80
  if (Serial.available()) {
    IntZ80(&cpu, INTERRUPT1_VECTOR);
  }

  // execute single opcode from memory at the current PC
  StepZ80(&cpu);

  // delay before stepping again so you can watch it count up slowly
  //delay(STEP_DELAY);
}
