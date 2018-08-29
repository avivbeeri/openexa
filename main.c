#include <stdio.h>
#include "exa.c"

uint8_t assembleOpcode(uint8_t opcode, uint8_t flags) {
  return (flags << 5) | opcode;
}

void assembleAndSetNumber(EXA* exa, int16_t value, uint8_t position) {
  uint8_t byte1 = value & 0xFF;
  uint8_t byte2 = ((value & 0xFF00) >> 8);
  exa->instructions[position] = byte1;
  exa->instructions[position+1] = byte2;
}

int main() {
  EXA exa;
  EXA_init(&exa, "XA");

  /*
  exa.regX = TEXT_VAL("WORD");
  exa.regT = NUMBER_VAL(42);
  */

  printf("%s\n", exa.name);
  printf("%d\n", AS_NUMBER(exa.registers[REG_X]));

  exa.instructions[0] = assembleOpcode(EXA_COPY, 1);
  assembleAndSetNumber(&exa, 42, 1);
  exa.instructions[3] = REG_X;

  exa.instructions[4] = assembleOpcode(EXA_JUMP, 0);
  assembleAndSetNumber(&exa, 7, 5);

  exa.instructions[7] = assembleOpcode(EXA_COPY, 0);
  exa.instructions[8] = REG_X;
  exa.instructions[9] = REG_T;

  exa.instructions[10] = assembleOpcode(EXA_ADDI, 7);
  assembleAndSetNumber(&exa, 42, 11);
  assembleAndSetNumber(&exa, 0, 13);
  exa.instructions[15] = REG_X;
  exa.instructions[16] = assembleOpcode(EXA_TEST, 0);
  exa.instructions[17] = REG_X;
  exa.instructions[18] = REG_T;

  exa.instructions[19] = assembleOpcode(EXA_HALT, 1);

  bool alive = true;
  while (alive) {
    alive = EXA_tick(&exa);
    EXA_dumpState(&exa);
    // TODO: Handle context changes (file, memory)
  }

  return 0;
}

