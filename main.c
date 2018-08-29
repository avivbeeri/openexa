#include <stdio.h>
#include "exa.c"

uint8_t assembleOpcode(uint8_t opcode, uint8_t flags) {
  return (flags << 5) | opcode;
}

void assembleAndSetNumber(EXA* exa, int16_t value, uint8_t position) {
  uint8_t byte1 = value & 0xFF;
  uint8_t byte2 = ((value >> 8) & 0xFF);
  exa->instructions[position] = byte1;
  exa->instructions[position+1] = byte2;
}

int main() {
  EXA exa;
  EXA_init(&exa, "XA");

  // COPY 2 X
  exa.instructions[0] = assembleOpcode(EXA_COPY, 1);
  assembleAndSetNumber(&exa, 2, 1);
  exa.instructions[3] = REG_X;

  // SUBI X 1 X
  exa.instructions[4] = assembleOpcode(EXA_SUBI, 2);
  exa.instructions[5] = REG_X;
  assembleAndSetNumber(&exa, 1, 6);
  exa.instructions[8] = REG_X;

  // TEST X = 0
  exa.instructions[9] = assembleOpcode(EXA_TEST, 2);
  exa.instructions[10] = REG_X;
  assembleAndSetNumber(&exa, 0, 11);

  // TJMP 0
  exa.instructions[13] = assembleOpcode(EXA_FJMP, 0);
  assembleAndSetNumber(&exa, 4, 14);

  // HALT
  exa.instructions[16] = assembleOpcode(EXA_HALT, 0);

  bool alive = true;
  while (alive) {
    alive = EXA_tick(&exa);
    EXA_dumpState(&exa);
    // TODO: Handle context changes (file, memory)
  }

  return 0;
}

