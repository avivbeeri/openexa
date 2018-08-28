#include "exa.h"
#include <string.h>

#define READ_BYTE() (*exa->ip++)

char* opToStr(uint8_t opcode) {
  switch (opcode) {
    case EXA_HALT: return "HALT"; break;
    case EXA_NOOP: return "NOOP"; break;
    case EXA_COPY: return "COPY"; break;
    default: return "UNKNONW"; break;
  }
}

void EXA_init(EXA* exa, char* name) {
  memset(exa, 0, sizeof(EXA));
  strncpy(exa->name, name, 8);
  exa->name[8] = '\0';
  exa->ip = exa->instructions;
}

// Returns true if the exa is still alive
bool EXA_tick(EXA* exa) {
  uint8_t byte = EXA_fetch(exa);
  uint8_t opcode = byte & 0x1F;
  uint8_t regFlags = (byte & 0xE0) >> 5;
  printf("Decoding opcode: %s\n", opToStr(opcode));
  printf("RegFlags: %d\n", regFlags);

  switch (opcode) {
    case EXA_COPY: {
      // COPY R/N R
      // Are we reading a register or number (or string?)
      Value value = NUMBER_VAL(0);
      uint8_t operand1 = READ_BYTE();
      if ((regFlags & 0x1) == 1) {
        // Literal: Number
        value = NUMBER_VAL((READ_BYTE() << 8) | operand1);
      } else {
        // Register
        value = exa->registers[operand1];
      }

      // Always a register operand
      uint8_t operand2 = READ_BYTE();
      exa->registers[operand2] = value;

      break;
    }
    case EXA_NOOP: break;
    case EXA_HALT:
    default: {
      if (opcode != EXA_HALT) {
        printf("EXA halted due to unknown opcode %d", opcode);
      }
      return false;
    }
  }
  return true;
}

uint8_t EXA_fetch(EXA* exa) {
  return READ_BYTE();
}
