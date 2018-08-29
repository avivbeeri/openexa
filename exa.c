#include "exa.h"
#include <string.h>

#define READ_BYTE() (*exa->ip++)

bool testFlagSet(uint8_t field, uint8_t n) {
  return ((field >> n) & 0x1) == 1;
}

char* opToStr(uint8_t opcode) {
  switch (opcode) {
    case EXA_HALT: return "HALT"; break;
    case EXA_NOOP: return "NOOP"; break;
    case EXA_COPY: return "COPY"; break;
    case EXA_ADDI: return "ADDI"; break;
    case EXA_MULI: return "MULI"; break;
    case EXA_SUBI: return "SUBI"; break;
    case EXA_DIVI: return "DIVI"; break;
    case EXA_MODI: return "MODI"; break;
    case EXA_JUMP: return "JUMP"; break;
    default: return "UNKNOWN"; break;
  }
}
char* regToStr(uint8_t opcode) {
  switch (opcode) {
    case REG_X : return "X"; break;
    case REG_T: return "T"; break;
    case REG_M: return "M"; break;
    case REG_F: return "F"; break;
    default: return "UNKNOWN"; break;
  }
}

void EXA_init(EXA* exa, char* name) {
  memset(exa, 0, sizeof(EXA));
  strncpy(exa->name, name, 8);
  exa->name[8] = '\0';
  exa->ip = exa->instructions;
  exa->instructions[255] = EXA_HALT;
}

// Returns true if the exa is still alive
bool EXA_tick(EXA* exa) {
  uint8_t byte = EXA_fetch(exa);
  uint8_t opcode = byte & 0x1F;
  uint8_t regFlags = (byte & 0xE0) >> 5;
  printf("Decoding opcode: %s(%d)\n", opToStr(opcode), opcode);
  printf("RegFlags: %d\n", regFlags);

  switch (opcode) {
    case EXA_COPY: {
      // COPY R/N R
      // Are we reading a register or number (or string?)
      Value value = NUMBER_VAL(0);
      uint8_t operand1 = READ_BYTE();
      printf("Copying ");
      if (testFlagSet(regFlags, 0)) {
        // Literal: Number
        value = NUMBER_VAL((READ_BYTE() << 8) | operand1);
        printf("%d to ", AS_NUMBER(value));
      } else {
        // Register
        value = exa->registers[operand1];
        printf("%s to ", regToStr(operand1));
      }

      // Always a register operand
      uint8_t operand2 = READ_BYTE();
      printf("%s\n", regToStr(operand2));
      exa->registers[operand2] = value;
      break;
    }
    case EXA_ADDI: {
      // ADDI R/N R/N R
      // Are we reading a register or number (or string?)
      Value value1 = NUMBER_VAL(0);
      Value value2 = NUMBER_VAL(0);
      printf("Adding ");
      uint8_t operand = READ_BYTE();
      if (testFlagSet(regFlags, 1)) {
        // Literal: Number
        value1 = NUMBER_VAL((READ_BYTE() << 8) | operand);
        printf("%d and ", AS_NUMBER(value1));
      } else {
        // Register
        value1 = exa->registers[operand];
        printf("%s and ", regToStr(operand));
      }

      operand = READ_BYTE();
      if (testFlagSet(regFlags, 1)) {
        // Literal: Number
        value2 = NUMBER_VAL((READ_BYTE() << 8) | operand);
        printf("%d to ", AS_NUMBER(value2));
      } else {
        // Register
        value2 = exa->registers[operand];
        printf("%s to ", regToStr(operand));
      }

      // Always a register operand
      uint8_t operand3 = READ_BYTE();
      printf("%s\n", regToStr(operand3));
      exa->registers[operand3] = NUMBER_VAL(AS_NUMBER(value1) + AS_NUMBER(value2));

      break;
    }
    case EXA_MULI: {
      // MULI R/N R/N R
      // Are we reading a register or number (or string?)
      Value value1 = NUMBER_VAL(0);
      Value value2 = NUMBER_VAL(0);
      printf("Multiplying ");
      uint8_t operand = READ_BYTE();
      if (testFlagSet(regFlags, 1)) {
        // Literal: Number
        value1 = NUMBER_VAL((READ_BYTE() << 8) | operand);
        printf("%d and ", AS_NUMBER(value1));
      } else {
        // Register
        value1 = exa->registers[operand];
        printf("%s and ", regToStr(operand));
      }

      operand = READ_BYTE();
      if (testFlagSet(regFlags, 1)) {
        // Literal: Number
        value2 = NUMBER_VAL((READ_BYTE() << 8) | operand);
        printf("%d to ", AS_NUMBER(value2));
      } else {
        // Register
        value2 = exa->registers[operand];
        printf("%s to ", regToStr(operand));
      }

      // Always a register operand
      uint8_t operand3 = READ_BYTE();
      printf("%s\n", regToStr(operand3));
      exa->registers[operand3] = NUMBER_VAL(AS_NUMBER(value1) * AS_NUMBER(value2));

      break;
    }
    case EXA_JUMP: {
      // JUMP L -> JUMP N
      int16_t operand1 = READ_BYTE();
      int16_t value = (READ_BYTE() << 8) | operand1;
      if (value < 0) {
        value = 0;
      } else if (value > 255) {
        value = 255;
      }
      printf("Jumping by: %d\n", value);
      exa->ip = exa->instructions + value;
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

void EXA_set(EXA* exa, Registers reg, Value value) {
  exa->registers[reg] = value;
}

Value EXA_get(EXA* exa, Registers reg) {
  return exa->registers[reg];
}

void EXA_dumpState(EXA* exa) {
  printf("--- %s ---\n", exa->name);
  printf("ip: %lu\n", exa->ip - exa->instructions);
  printf("X: %d\n", AS_NUMBER(EXA_get(exa, REG_X)));
  printf("T: %d\n", AS_NUMBER(EXA_get(exa, REG_T)));
  printf("M: %d\n", AS_NUMBER(EXA_get(exa, REG_M)));
  printf("F: %d\n", AS_NUMBER(EXA_get(exa, REG_F)));
  printf("-------\n");
}
